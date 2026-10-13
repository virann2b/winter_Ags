#include "PlayerGunState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"



PlayerGunState::PlayerGunState(
	std::function<void(void)> playAnimeGun, 
	std::function<void(void)> ChangeStateIdle)
	:
	playAnimeGun(playAnimeGun),
	ChangeStateIdle(ChangeStateIdle)
{
}

void PlayerGunState::OwnStateConditionUpdate(void)
{
	//“ü—Í
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).down) { OwnChangeState(); }
}

void PlayerGunState::Enter(void)
{
	playAnimeGun();
}

void PlayerGunState::Update(void)
{
	
	if (Input::GetIns().GetInfo(KEY_TYPE::Test).down) { ChangeStateIdle(); }
	return;
}
