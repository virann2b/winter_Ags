#include "EnemyBase.h"

#include "../../Manager/TimeScale/TimeScale.h"

#include "../Common/Collider/ColliderBase.h"

#include "../Skill/SkillBase.h"

#include <DxLib.h>

EnemyBase::EnemyBase(const Vector3& initPos)
	:
	CharacterBase(),
	initPos(initPos),
	attackCoolTime(0.0f)
{
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

	//ダメージ処理
	TakeDamage(GetDamage(otherTag));
	StartInvincible();

	//攻撃被弾時の処理
	if (IsDead()) { OnDie(); }
	else { OnHit(); }
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

int EnemyBase::GetDamage(COLLIDER_TAG otherTag) const
{
	return DEFAULT_DAMAGE;
}

void EnemyBase::DieEnd(void)
{
	isDieEnd = true;

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

void EnemyBase::SubInit(void)
{
	//無敵時間リセット
	invincibleCounter = 0.0f;

	//攻撃クールタイム
	attackCoolTime = 0.0f;

	//死亡終了フラグ
	isDieEnd = false;

	EnemyInit();
}

void EnemyBase::SubUpdate(void)
{
	//無敵時間の更新
	if (invincibleCounter > 0.0f)
	{ invincibleCounter -= TimeScale::Get(); }
	else 
	{ invincibleCounter = 0.0f; }

	//スキルのクールタイム更新
	SkillCoolTimeUpdate();

	EnemyUpdate();
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
