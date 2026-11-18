#include "EnemyBase.h"

#include "../../Manager/TimeScale/TimeScale.h"

#include "../Common/Collider/ColliderBase.h"

#include "../Skill/SkillBase.h"

#include <DxLib.h>

EnemyBase::EnemyBase(const Vector3& initPos)
	:
	CharacterBase("Data/Parameter/Enemy/"),
	initPos(initPos),
	attackCoolTime(0.0f)
{
	//初期耐性
	resistTable.fill(ELEMENT_RESIST::Normal);
}

void EnemyBase::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	//無敵時間中は受け付けない
	if (invincibleCounter > 0) { return; }

	COLLIDER_TAG otherTag = other.GetTag();

	//プレイヤーの攻撃以外は無視
	if (otherTag != COLLIDER_TAG::PlayerSword &&
		otherTag != COLLIDER_TAG::PlayerLargeAttack &&
		otherTag != COLLIDER_TAG::PlayerSkill)
	{
		return;
	}

	// 最後に受けた攻撃の属性取得
	lastHitElement = other.GetElement();

	//ダメージ処理
	TakeDamage(WeakDamage(other.GetDamage()));

	//無敵判定開始
	StartInvincible();

	//攻撃被弾時の処理
	if (IsDead()) 
	{ OnDie(); }
	else
	{
		const bool isStagger = 
			(breakCounter <= 0.0f) && ReduceStagger(other.GetStagger());

		//弱点を突かれたら弱点硬直
		if (IsWeak(lastHitElement)) { WeakStun(); }

		//弱点硬直フラグ
		if (isWeakStun_)
		{
			//弱点硬直はそのまま、スローだけ
			if (isStagger) { OnStaggerBreak(); }
		}
		else if (isStagger)
		{
			//硬直を打ち切って体勢崩れ
			BreakStance();
		}
		else
		{
			//小さい反動だけ
			StartShake(SMALL_SHAKE_RATE);
		}
	}

	//攻撃反動
	if (!IsDead() && isWeakStun_) { StartShake(); }
}

void EnemyBase::Stun(float frame)
{
	//死亡中・すでに硬直中なら受け付けない
	if (IsDead() || isStunned_) { return; }

	StartStun(frame, false);
}

void EnemyBase::WeakStun(void)
{
	//死亡・すでに弱点を疲れている場合は受け付けない
	if (IsDead() || isWeakStun_) { return; }

	StartStun(WEAK_STUN_FRAME, true);

	//弱点硬直を起こした属性を保存
	weakStunElement = lastHitElement;
}

void EnemyBase::TakeGunDamege(int damage)
{
	// すでに倒れている敵には当てない
	if (IsDead()) { return; }

	//ダメージ処理
	TakeDamage(damage);

	// 倒したときだけ死亡処理
	if (IsDead()) { OnDie(); }
}

void EnemyBase::SetupEnemyObject(int pushWeight)
{
	// 動的オブジェクトとしての処理を有効
	SetDynamicFlg(true);

	// 重力を有効
	SetGravityFlg(true);

	// 押し出しによる重みを設定
	SetPushWeight(pushWeight);

	// 当たり判定による押し出しを有効
	SetPushFlg(true);

	// 当たり判定
	SetJudgeFlg(true);
}

void EnemyBase::DieEnd(void)
{
	isDieEnd_ = true;

	//画面から消す
	SetIsDraw(false);
	SetJudgeFlg(false);
}

bool EnemyBase::IsFar(float stopRange) const
{
	if (targetPos == nullptr) { return false; }

	return !IsNear(stopRange);
}

bool EnemyBase::IsNear(float stopRange) const
{
	if (targetPos == nullptr) { return false; }

	const float dx = targetPos->x - trans.pos.x;
	const float dz = targetPos->z - trans.pos.z;

	return std::sqrt(dx * dx + dz * dz) <= stopRange;
}

Vector3 EnemyBase::GetDirectionToTarget(void) const
{
	if (targetPos == nullptr) { return Vector3(); }

	const float dx = targetPos->x - trans.pos.x;
	const float dz = targetPos->z - trans.pos.z;
	const float len = std::sqrt(dx * dx + dz * dz);
	if (len <= 0.0001f) { return Vector3(); }

	return Vector3(dx / len, 0.0f, dz / len);
}

void EnemyBase::LoadResist(const std::string& enemyName)
{
	// その回のパラメータ
	std::string FileName = "ElementResist";

	// そのパラメータが存在するかどうか
	if (!IsParameterExist(FileName, enemyName)) { return; }
	
	//csvから値を取得
	const std::vector<float>& values = GetParameterArray(FileName, enemyName);

	//属性耐性テーブルに登録
	for (int i = 0; i < (int)ELEMENT::Max && i < (int)values.size(); ++i)
	{
		resistTable[i] = static_cast<ELEMENT_RESIST>((int)values[i]);
	}
}

bool EnemyBase::IsWeak(ELEMENT element) const
{
	//属性なし
	if (element == ELEMENT::None || element == ELEMENT::Max)
	{
		return false;
	}

	//属性弱点を取得
	return resistTable[(int)element] == ELEMENT_RESIST::Weak;
}

void EnemyBase::SubInit(void)
{
	//無敵時間リセット
	invincibleCounter = 0.0f;

	//攻撃クールタイム
	attackCoolTime = 0.0f;

	//死亡終了フラグ
	isDieEnd_ = false;

	//硬直
	isStunned_ = false;
	stunFrame = 0.0f;

	EnemyInit();
}

void EnemyBase::SubUpdate(void)
{
	//無敵時間の更新
	if (invincibleCounter > 0.0f)
	{ invincibleCounter -= TimeScale::Get(); }
	else 
	{ invincibleCounter = 0.0f; }

	//必殺技を受けていない間だけ行動の更新
	if (!IsActionStop())
	{
		//スキルのクールタイム更新
		SkillCoolTimeUpdate();

		EnemyUpdate();
	}

	UpdateShake();

	//怯み値の回復
	UpdateStagger();
}

void EnemyBase::SubDraw(void)
{
	EnemyDraw();
}

EnemyBase::DISTANCE EnemyBase::GetTargetDistance(void) const
{
	if (targetPos == nullptr) { return DISTANCE::Out; }

	const float dx = targetPos->x - trans.pos.x;
	const float dz = targetPos->z - trans.pos.z;
	const float dist = std::sqrt(dx * dx + dz * dz);

	if (dist <= NEAR_RANGE) { return DISTANCE::Near; }
	if (dist <= MIDDLE_RANGE) { return DISTANCE::Middle; }
	if (dist <= LONG_RANGE) { return DISTANCE::Long; }
	return DISTANCE::Out;
}

void EnemyBase::AddSkill(SkillBase* skill, DISTANCE minDistance, DISTANCE maxDistance, float coolTime)
{
	if (skill == nullptr) { return; }

	//子アクターとして登録（Loadもここで呼ばれる）
	AddChildActor(skill);

	skillSlots.push_back({ skill, minDistance, maxDistance, coolTime, 0.0f });
}

bool EnemyBase::TryUseSkill(void)
{
	//攻撃対象がいない・すでにスキル発動中なら使わない
	if (targetPos == nullptr || IsSkillActive()) { return false; }

	const DISTANCE dist = GetTargetDistance();
	if (dist == DISTANCE::Out) { return false; }

	//今使えるスキルを集める
	std::vector<SkillSlot*> usable;
	for (SkillSlot& slot : skillSlots)
	{
		//クールタイム中
		if (slot.coolCount > 0.0f) { continue; }
		//距離が範囲外
		if ((int)dist < (int)slot.minDistance || (int)dist > (int)slot.maxDistance) { continue; }

		usable.push_back(&slot);
	}
	if (usable.empty()) { return false; }

	//ランダムに1つ選ぶ
	SkillSlot* use = usable[GetRand((int)usable.size() - 1)];

	//攻撃対象の方向を向いてから撃つ
	const Vector3 dir = GetDirectionToTarget();
	if (dir != 0.0f) { trans.SetRotation(Quaternion::FromRotationY(atan2f(dir.x, dir.z))); }

	if (!use->skill->Activate()) { return false; }

	use->coolCount = use->coolTime;
	return true;
}

bool EnemyBase::IsSkillActive(void) const
{
	for (const SkillSlot& slot : skillSlots)
	{
		if (slot.skill->IsActive()) { return true; }
	}
	return false;
}

void EnemyBase::SkillCoolTimeUpdate(void)
{
	for (SkillSlot& slot : skillSlots)
	{
		if (slot.coolCount > 0.0f) { slot.coolCount -= TimeScale::Get(); }
		if (slot.coolCount < 0.0f) { slot.coolCount = 0.0f; }
	}
}

void EnemyBase::StartStun(float frame, bool isWeak)
{
	//硬直時間
	stunFrame = frame;
	//残り硬直時間
	stunRemain = frame;

	//敵ごとの硬直
	OnStun();

	isStunned_ = true;
	isWeakStun_ = isWeak;
}

int EnemyBase::WeakDamage(int damage) const
{
	//弱点硬直してない場合は何もしない
	if (!isWeakStun_) { return damage; }

	//ダメージ倍率をかけ、四捨五入
	return (int)std::round(damage * WEAK_DAMAGE_RATE);
}

void EnemyBase::UpdateShake(void)
{

	//揺れていなければズレなし
	if (shakeCounter <= 0.0f)
	{
		shakeCounter = 0.0f;
		trans.drawOffset = Vector3();
		return;
	}

	shakeCounter -= TimeScale::Get();

	//経過の割合
	const float progress = 1.0f - (shakeCounter / SHAKE_FRAME);

	//揺れ幅はだんだん小さく
	const float width = SHAKE_WIDTH * shakeRate * (1.0f - progress);

	//左右に往復
	const float wave = sinf(progress * SHAKE_COUNT * DX_TWO_PI_F);

	//敵から見た右方向にずらす
	const Vector3 right = trans.VTrans(Vector3(1.0f, 0.0f, 0.0f));
	trans.drawOffset = right * (width * wave);

}

#pragma region 怯み値

bool EnemyBase::ReduceStagger(int damage)
{
	stagger -= (float)damage;
	if (stagger < 0.0f) { stagger = 0.0f; }
	return stagger <= 0.0f;
}

void EnemyBase::BreakStance(void)
{
	//のけぞり
	OnHit();

	//崩れた時の共通処理
	OnStaggerBreak();
}

void EnemyBase::OnStaggerBreak(void)
{
	//怯み値を最大に戻す
	breakCounter = BREAK_GUARD_FRAME;
	stagger = staggerMax;

	//仮の演出
	TimeScale::GetIns().TemporaryScale(BREAK_SLOW_SCALE, BREAK_SLOW_TIME);

	//受付開始
	breakChanceCounter = BREAK_CHANCE_FRAME;	
}

void EnemyBase::UpdateStagger(void)
{
	//体勢崩れの残り時間
	if (breakCounter > 0.0f) { breakCounter -= TimeScale::Get(); }
	//実時間で減らす
	if (breakChanceCounter > 0.0f) { breakChanceCounter -= 1.0f; }	

	if (stagger > staggerMax) { stagger = staggerMax; }
}

void EnemyBase::LoadStagger(const std::string& enemyName)
{
	//読み込むCSVのファイル名
	const std::string fileName = "Stagger";

	//書かれていなければ初期値のまま
	if (!IsParameterExist(fileName, enemyName)) { return; }

	//最大値, 回復が始まるまで, 1フレームの回復量
	const std::vector<float>& values = GetParameterArray(fileName, enemyName);
	if (values.size() >= 1) { staggerMax = values[0]; }

	stagger = staggerMax;
}

#pragma endregion


void EnemyBase::CountStun(float frame)
{
	stunRemain -= frame;

	if (stunRemain < 0.0f) { stunRemain = 0.0f; }
}

float EnemyBase::GetWeakStunRate(void) const
{
	//弱点硬直でなければ0のまま
	if (!isWeakStun_ || stunFrame <= 0.0f) { return 0.0f; }

	//残り硬直時間の割合
	return stunRemain / stunFrame;
}

void EnemyBase::TakeFinishDamage(int damage)
{
	//ダメージ
	TakeDamage(damage);

	//ダメージ後の処理
	if (IsDead()) { OnDie(); }
	else
	{
		//のけぞり＋反動
		OnHit();
		StartShake();
	}
}
