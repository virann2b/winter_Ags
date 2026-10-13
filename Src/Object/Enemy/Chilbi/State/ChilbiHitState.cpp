#include "ChilbiHitState.h"

ChilbiHitState::ChilbiHitState(std::function<void(void)> playAnimeHit)
	:
	playAnimeHit(playAnimeHit)
{
}

void ChilbiHitState::OwnStateConditionUpdate(void)
{

}

void ChilbiHitState::Enter(void)
{
	playAnimeHit();
}
