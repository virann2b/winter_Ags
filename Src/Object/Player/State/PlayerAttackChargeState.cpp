#include "PlayerAttackChargeState.h"

#include "../../../Manager/Input/InputManager.h"


PlayerAttackChargeState::PlayerAttackChargeState(
	std::function<void(void)> PlayerAttackChargeStart,
	std::function<void(void)> PlayerAttackChargeEnd,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> ChangeStateIdle
	):
	PlayerAttackChargeStart(PlayerAttackChargeStart),
	PlayerAttackChargeEnd(PlayerAttackChargeEnd),
	isAnimeEnd(isAnimeEnd),
	ChangeStateIdle(ChangeStateIdle),
	step(STEP::None)
{
}

void PlayerAttackChargeState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong).down) { OwnChangeState(); }
}


void PlayerAttackChargeState::Enter(void)
{
	step = STEP::Charge;
	PlayerAttackChargeStart();
}

void PlayerAttackChargeState::Update(void)
{
	switch (step)
	{
	case PlayerAttackChargeState::STEP::Charge:
	{
		//タメアニメ-ションより入力時間が短い場合はそのまま待機状態に戻る
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong).up && !isAnimeEnd())
		{
			ChangeStateIdle();
		}

		//タメアニメーションが終わっていた場合
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong).up && isAnimeEnd())
		{
			PlayerAttackChargeEnd();
			step = STEP::Attack;
		}

		break;
	}
	case PlayerAttackChargeState::STEP::Attack:
	{

		if (isAnimeEnd())
		{
			ChangeStateIdle();
		}
		break;
	}
	}
}
