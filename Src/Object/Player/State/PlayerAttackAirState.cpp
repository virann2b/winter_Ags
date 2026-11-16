#include "PlayerAttackAirState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"


PlayerAttackAirState::PlayerAttackAirState(
	const bool& IsGround, 
	bool& IsMaxJump,
	std::function<void(void)> PlayAnimAirAttackStart, 
	std::function<void(void)> PlayAnimAirAttackEnd, 
	std::function<void(void)> ArriveAttackRange,
	std::function<void(void)> EndAttackRange,
	std::function<float(void)> AnimRate,
	std::function<bool(void)> IsAnimeEnd,
	std::function<void(void)> ChangeStateIdle
):
	IsGround(IsGround),
	IsMaxJump(IsMaxJump),
	PlayAnimAirAttackStart(PlayAnimAirAttackStart),
	PlayAnimAirAttackEnd(PlayAnimAirAttackEnd),
	ArriveAttackRange(ArriveAttackRange),
	EndAttackRange(EndAttackRange),
	AnimRate(AnimRate),
	IsAnimeEnd(IsAnimeEnd),
	ChangeStateIdle(ChangeStateIdle),
	step(STEP::None)
{
}

void PlayerAttackAirState::Enter(void)
{
   	PlayAnimAirAttackStart();

	step = STEP::Start;
}

void PlayerAttackAirState::OwnStateConditionUpdate(void)
{
	//入力
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong).down && IsMaxJump) { OwnChangeState(); }
	
}

void PlayerAttackAirState::Update(void)
{
	switch (step)
	{
	case STEP::Start:
	{
		// 開始モーションの途中で着地したら、そのまま着地モーションへ
		if (IsGround)
		{
			PlayAnimAirAttackEnd();
			step = STEP::End;
			break;
		}
		// 開始モーションが終わったら空中状態へ
		if (IsAnimeEnd())
		{
			step = STEP::Air;
		}
		break;
	}
	case STEP::Air:
	{
		// 着地を待つ(開始モーションの最終ポーズを保持)
		if (IsGround)
		{
			PlayAnimAirAttackEnd();
			step = STEP::End;
		}
		break;
	}
	case STEP::End:
	{
		if (AnimRate() >= RANGEATTACK_RAITE)
		{
			//範囲攻撃発生
			ArriveAttackRange();
		}

		// 着地モーションが終わったら Idle へ
		if (IsAnimeEnd())
		{
			ChangeStateIdle();
		}
		break;
	}
	}
}

void PlayerAttackAirState::Exit(void)
{
	//範囲攻撃判定OFF
	EndAttackRange();
}
