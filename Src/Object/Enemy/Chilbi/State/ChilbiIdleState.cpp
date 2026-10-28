#include "ChilbiIdleState.h"

#include "../../../../Common/Vector3.h"

ChilbiIdleState::ChilbiIdleState(
	std::function<void(void)> playAnimeIdle,
	std::function<void(void)> changeAttackState
):
	playAnimeIdle(playAnimeIdle),
	changeAttackState(changeAttackState)
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
	if (GetRand(100) >= 10)
	{
		changeAttackState();
	}
}
