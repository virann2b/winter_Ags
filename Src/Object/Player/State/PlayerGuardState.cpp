#include "PlayerGuardState.h"

#include "../../../Manager/Input/InputManager.h"

PlayerGuardState::PlayerGuardState(
	std::function<void(void)> PlayAnimeGuard, 
	std::function<bool(void)> IsAnimeEnd, 
	std::function<void(void)> ChangeStateIdle
):
	PlayAnimeGuard(PlayAnimeGuard),
	IsAnimeEnd(IsAnimeEnd),
	ChangeStateIdle(ChangeStateIdle)
{
}

void PlayerGuardState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(Input::KEY_TYPE::PlayerGuard).down)
	{
		OwnChangeState();
	}
}

void PlayerGuardState::Enter(void)
{
	//ガードアニメーション再生
	PlayAnimeGuard();
}

void PlayerGuardState::Update(void)
{

	if (Input::GetIns().GetInfo(Input::KEY_TYPE::PlayerGuard).up)
	{
		ChangeStateIdle();
	}

}
