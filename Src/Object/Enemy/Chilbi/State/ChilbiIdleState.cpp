#include "ChilbiIdleState.h"

#include "../../../../Common/Vector3.h"

ChilbiIdleState::ChilbiIdleState(
	std::function<void(void)> playAnimeIdle)
	:
	playAnimeIdle(playAnimeIdle)
{
}
void ChilbiIdleState::OwnStateConditionUpdate(void)
{

}

void ChilbiIdleState::Enter(void)
{
	playAnimeIdle();
}
