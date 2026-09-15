#include "SpaceChangeTrigger.h"

#include "../../../Object/Common/Collider/BoxCollider.h"

SpaceChangeTrigger::SpaceChangeTrigger(GameSpaceController& gameSpace,
	ActorBase* target,
	CHANGE_TYPE changeType,
	COLLIDER_TAG enterTag,
	const Vector3& pos,
	const Vector3& size,
	const MovementPlane& plane,
	const SpaceTransitionParameter& parameter) :
	ActorBase(),
	gameSpace(&gameSpace),
	target(target),
	changeType(changeType),
	enterTag(enterTag),
	plane(plane),
	rail(),
	parameter(parameter),
	triggerSize(size),
	isOneShot(true),
	isUsed(false)
{
	trans.pos = pos;
}

SpaceChangeTrigger::SpaceChangeTrigger(GameSpaceController& gameSpace,
	ActorBase* target,
	COLLIDER_TAG enterTag,
	const Vector3& pos,
	const Vector3& size,
	const MovementRail& rail,
	const SpaceTransitionParameter& parameter) :
	ActorBase(),
	gameSpace(&gameSpace),
	target(target),
	changeType(CHANGE_TYPE::Rail),
	enterTag(enterTag),
	plane(),
	rail(rail),
	parameter(parameter),
	triggerSize(size),
	isOneShot(true),
	isUsed(false)
{
	trans.pos = pos;
}

void SpaceChangeTrigger::Load(void)
{
	// Trigger自体は押し出しを行わず、接触通知だけを受け取る
	SetDynamicFlg(false);
	SetGravityFlg(false);
	SetPushFlg(false);
	SetIsDraw(false);
	SetSpaceConstraint(SPACE_CONSTRAINT::None);

	ColliderCreate(new BoxCollider(COLLIDER_TAG::DebugObject, triggerSize));
}

void SpaceChangeTrigger::SubInit(void)
{
}

void SpaceChangeTrigger::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (isUsed && isOneShot) { return; }
	if (other.GetTag() != enterTag) { return; }
	if (gameSpace == nullptr || gameSpace->IsTransition()) { return; }

	ChangeSpace();
	isUsed = true;
}

void SpaceChangeTrigger::ChangeSpace(void)
{
	switch (changeType)
	{
	case CHANGE_TYPE::Side2D:
		gameSpace->StartChangeToSide2D(target, plane, parameter);
		break;

	case CHANGE_TYPE::Plane3D:
		gameSpace->StartChangeToPlane3D(target, plane, parameter);
		break;

	case CHANGE_TYPE::Rail:
		gameSpace->StartChangeToRail(target, rail, parameter);
		break;

	case CHANGE_TYPE::Free3D:
		gameSpace->StartChangeTo3D(target, parameter);
		break;
	}
}
