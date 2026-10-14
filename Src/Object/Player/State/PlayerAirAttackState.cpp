#include "PlayerAirAttackState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"

PlayerAirAttackState::PlayerAirAttackState(
	std::function<void(void)> playAnimAirAttack,
	std::function<void(void)> ChangeStateIdle)
	:
	playAnimAirAttack(playAnimAirAttack),
	ChangeStateIdle(ChangeStateIdle)
{
}

void PlayerAirAttackState::Enter(void)
{
	playAnimAirAttack();
}

void PlayerAirAttackState::OwnStateConditionUpdate(void)
{
	//“ü—Í
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down ) { OwnChangeState(); }
}

void PlayerAirAttackState::Update(void)
{
}
