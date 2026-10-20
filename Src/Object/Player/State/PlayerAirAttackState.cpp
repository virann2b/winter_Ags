#include "PlayerAirAttackState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"


PlayerAirAttackState::PlayerAirAttackState(
	const bool& isGround, 
	std::function<void(void)> playAnimAirAttackStart, 
	std::function<void(void)> playAnimAirAttackEnd, 
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> ChangeStateIdle)
	:
	isGround(isGround),
	playAnimAirAttackStart(playAnimAirAttackStart),
	playAnimAirAttackEnd(playAnimAirAttackEnd),
	isAnimeEnd(isAnimeEnd),
	ChangeStateIdle(ChangeStateIdle),
	isChangeAnim(false)
{
}

void PlayerAirAttackState::Enter(void)
{
	playAnimAirAttackStart();
	//isChangeAnim = false;

	step = STEP::Start;
}

void PlayerAirAttackState::OwnStateConditionUpdate(void)
{
	//入力
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down ) { OwnChangeState(); }
}

void PlayerAirAttackState::Update(void)
{
	switch (step)
	{
	case STEP::Start:
	{
		// 開始モーションの途中で着地したら、そのまま着地モーションへ
		if (isGround)
		{
			playAnimAirAttackEnd();
			step = STEP::End;
			break;
		}
		// 開始モーションが終わったら空中状態へ
		if (isAnimeEnd())
		{
			step = STEP::Air;
		}
		break;
	}
	case STEP::Air:
	{
		// 着地を待つ(開始モーションの最終ポーズを保持)
		if (isGround)
		{
			playAnimAirAttackEnd();
			step = STEP::End;
		}
		break;
	}
	case STEP::End:
	{
		// 着地モーションが終わったら Idle へ
		if (isAnimeEnd())
		{
			ChangeStateIdle();
		}
		break;
	}
	}
}
