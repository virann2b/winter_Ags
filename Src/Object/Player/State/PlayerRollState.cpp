#include "PlayerRollState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerRollState::PlayerRollState(
	std::function<void(void)> ChangeStateIdle,
	std::function<void(void)> playAnimeRoll,
	std::function<bool(void)> isAnimeEnd
):
	ChangeStateIdle(ChangeStateIdle),
	playAnimeRoll(playAnimeRoll),
	isAnimeEnd(isAnimeEnd)
{
}

void PlayerRollState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerRoll).down) { OwnChangeState(); }
}

void PlayerRollState::Enter(void)
{
	playAnimeRoll();
}

void PlayerRollState::Update(void)
{
	if (isAnimeEnd())
	{
		ChangeStateIdle();
	}
}
