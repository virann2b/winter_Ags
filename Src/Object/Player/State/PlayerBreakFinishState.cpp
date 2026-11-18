#include "PlayerBreakFinishState.h"

PlayerBreakFinishState::PlayerBreakFinishState(
	std::function<void(void)> ReadyStart,
	std::function<void(void)> PassStart,
	std::function<void(float)> PassUpdate,
	std::function<void(void)> FinishSlash, 
	std::function<bool(void)> IsAnimeEnd,
	std::function<void(void)> FinishRelease,
	std::function<void(void)> ChangeStateIdle
)
{
}

void PlayerBreakFinishState::Enter(void)
{
}

void PlayerBreakFinishState::Update(void)
{
}

void PlayerBreakFinishState::Exit(void)
{
}
