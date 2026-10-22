#include "ChilbiDieState.h"

ChilbiDieState::ChilbiDieState(
	std::function<void(void)> playAnimeDie,
	std::function<bool(void)> isDead,
	std::function<bool(void)> isAnimeEnd
):
	playAnimeDie(playAnimeDie),
	isDead(isDead),
	isAnimeEnd(isAnimeEnd)
{
}

void ChilbiDieState::OwnStateConditionUpdate(void)
{
	if (!isDead()) { return; }

	OwnChangeState();
}

void ChilbiDieState::Enter(void)
{
	playAnimeDie();
}
