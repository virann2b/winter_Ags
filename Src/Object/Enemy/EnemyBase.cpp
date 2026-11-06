#include "EnemyBase.h"

#include "../../Manager/TimeScale/TimeScale.h"

#include "../Common/Collider/ColliderBase.h"

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

	EnemyInit();
}

void EnemyBase::SubUpdate(void)
{
	//無敵時間の更新
	if (invincibleCounter > 0.0f)
	{ invincibleCounter -= TimeScale::Get(); }
	else 
	{ invincibleCounter = 0.0f; }

	EnemyUpdate();
}

void EnemyBase::SubDraw(void)
{
	EnemyDraw();
}
