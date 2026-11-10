#include "SkillBase.h"

#include "../../Manager/TimeScale/TimeScale.h"

#include "../Common/Collider/SphereCollider.h"

SkillBase::SkillBase
(
	const Transform& userTrans,
	SKILL_USER user
):
	ActorBase(),
	user(user),
	mpCost(0),
	scale(SCALE::None),
	distance(DISTANCE::None),
	step(STEP::None),
	moveDir(),
	skillAliveTime(60.0f),
	skillAliveCount(0.0f),
	userTrans(userTrans)
{
}

bool SkillBase::Activate(void)
{
	//発動中は重ねて発動しない
	if (IsActive()) { return false; }

	skillAliveCount = 0.0f;

	//派生先の開始処理（位置・向きの決定など）
	//発動した瞬間の使用者の向きを使うため、ここで即座に行う
	SkillStart();

	//当たり判定・描画オン
	SetJudgeFlg(true);
	SetIsDraw(true);

	step = STEP::Active;

	return true;
}

void SkillBase::SetUpSkillObject(void)
{
	//移動オン
	SetDynamicFlg(true);

	//重力押し出しオフ
	SetGravityFlg(false);
	SetPushFlg(false);

	//発動するまでは描画しない
	SetIsDraw(false);
}

void SkillBase::AddSkillCollider(float radius)
{
	//使用者に合わせてタグを決める
	const COLLIDER_TAG tag =
		(user == SKILL_USER::Enemy) ? COLLIDER_TAG::EnemySkill : COLLIDER_TAG::PlayerSkill;

	//コライダーの追加
	SphereCollider* coll = new SphereCollider(tag, radius, trans.pos);

	//属性の設定
	coll->SetElement(element);

	AddCollider(coll);

	//当たり判定オフ（コライダー追加後でないと効かない）
	SetJudgeFlg(false);
}

void SkillBase::Finish(void)
{
	//発動中のみ終了へ
	if (step == STEP::Start || step == STEP::Active) { step = STEP::End; }
}

bool SkillBase::IsTarget(COLLIDER_TAG otherTag) const
{
	switch (user)
	{
	case SKILL_USER::Player:
	case SKILL_USER::PlayerFriend:
		return otherTag == COLLIDER_TAG::Enemy;

	case SKILL_USER::Enemy:
		return otherTag == COLLIDER_TAG::Player;

	default:
		return false;
	}
}

void SkillBase::SubUpdate(void)
{
	switch (step)
	{
	case STEP::None:
	{
		//未使用中は何もしない
		break;
	}
	case STEP::Start:
	{
		//開始処理はActivate()で行うので、ここでは発動中へ進めるだけ
		step = STEP::Active;

		break;
	}
	case STEP::Active:
	{
		//派生先の更新処理
		SkillUpdate();

		//生存時間を過ぎたら終了
		skillAliveCount += TimeScale::Get();
		if (skillAliveCount >= skillAliveTime) { step = STEP::End; }

		break;
	}
	case STEP::End:
	{
		//派生先の終了処理
		SkillEnd();

		//当たり判定・描画オフ
		SetJudgeFlg(false);
		SetIsDraw(false);

		//待機に戻す（1回だけ通る）
		step = STEP::None;

		break;
	}
	}
}
