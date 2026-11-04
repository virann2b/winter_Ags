#include "ChilbiIdleState.h"

#include "../../../../Common/Vector3.h"

ChilbiIdleState::ChilbiIdleState(
	std::function<void(void)> playAnimeIdle,
	std::function<void(void)> changeMoveState,
	std::function<void(void)> changeAttackState,
	std::function<bool(void)> isDistanceNear,
	float attackCoolTime
):
	playAnimeIdle(playAnimeIdle),
	changeMoveState(changeMoveState),
	changeAttackState(changeAttackState),
	isDistanceNear(isDistanceNear),
	attackCoolTime(attackCoolTime)
{
}
void ChilbiIdleState::OwnStateConditionUpdate(void)
{

}

void ChilbiIdleState::Enter(void)
{
	playAnimeIdle();
}

void ChilbiIdleState::Update(void)
{
	//クールタイム中かつ攻撃範囲の場合
	if (!attackCoolTime && isDistanceNear()) return;
	
	//攻撃範囲にいない場合移動状態へ
	if (!isDistanceNear())
	{
		changeMoveState();
		return;
	}

	if(attackCoolTime == 0.0f)
	{
		changeAttackState();
	}
}
