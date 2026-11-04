#include "ChilbiMoveState.h"

#include "../../../../Common/Vector3.h"

ChilbiMoveState::ChilbiMoveState(
	std::function<void(void)> playAnimeMove,
	std::function<void(void)> changeAttackState, 
	std::function<void(const Vector3&)> moveAccel,
	std::function<Vector3(void)> getTargetPos,
	std::function<bool(void)> isDistanceNear
):
	playAnimeMove(playAnimeMove),
	changeAttackState(changeAttackState),
	moveAccel(moveAccel),
	getTargetPos(getTargetPos),
	isDistanceNear(isDistanceNear)
{
}

void ChilbiMoveState::OwnStateConditionUpdate(void)
{
}

void ChilbiMoveState::Enter(void)
{
	//ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	playAnimeMove();
}

void ChilbiMoveState::Update(void)
{
	//UŒ‚‘ÎÛ‚Æ‚Ì‹——£‚ªˆê’èˆÈ‰º‚È‚çUŒ‚‚É‘JˆÚ
	if (isDistanceNear())
	{
		changeAttackState();
		return;
	}

	//UŒ‚‘ÎÛ‚Ì•ûŒü‚ÉˆÚ“®
	moveAccel(getTargetPos());
}
