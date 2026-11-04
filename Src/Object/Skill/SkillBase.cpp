#include "SkillBase.h"

SkillBase::SkillBase
(
	const Transform& userTrans
):
	ActorBase(),
	userTrans(userTrans),
	distance(DISTANCE::None),
	scale(SCALE::None)
{
}

void SkillBase::SetUpSkillObject(void)
{
	//移動オン
	SetDynamicFlg(true);

	//重力押し出しオフ
	SetGravityFlg(false);
	SetPushFlg(false);

	//当たり判定オフ
	SetJudgeFlg(false);

}
