#include "PlayerBreakFinishState.h"

#include <algorithm>

#include "../../../Manager/TimeScale/TimeScale.h"

PlayerBreakFinishState::PlayerBreakFinishState(
	std::function<void(void)> ReadyStart,
	std::function<void(void)> PassStart,
	std::function<void(float)> PassUpdate,
	std::function<void(void)> FinishSlash, 
	std::function<bool(void)> IsAnimeEnd,
	std::function<void(void)> ReturnStart,
	std::function<bool(void)> IsReturnEnd,
	std::function<void(void)> FinishRelease,
	std::function<void(void)> ChangeStateIdle
)
	:
	ReadyStart(ReadyStart),
	PassStart(PassStart),
	PassUpdate(PassUpdate),
	FinishSlash(FinishSlash),
	IsAnimeEnd(IsAnimeEnd),
	ReturnStart(ReturnStart),
	IsReturnEnd(IsReturnEnd),
	FinishRelease(FinishRelease),
	ChangeStateIdle(ChangeStateIdle),
	step(STEP::None),
	passCounter(0.0f)
{
}

void PlayerBreakFinishState::Enter(void)
{
	//構えから開始
	step = STEP::Ready;
	passCounter = 0.0f;

	ReadyStart();
}

void PlayerBreakFinishState::Update(void)
{
	switch (step)
	{
	case PlayerBreakFinishState::STEP::Ready:
	{
		//構えが終わったら一閃
		if (IsAnimeEnd())
		{
			PassStart();
			passCounter = 0.0f;
			step = STEP::Pass;
		}
		break;
	}
	case PlayerBreakFinishState::STEP::Pass:
	{
		passCounter += TimeScale::Get();

		//すり抜けの進み具合（0～1）
		const float rate = (std::min)(passCounter / PASS_FRAME, 1.0f);

		PassUpdate(rate);

		//背後に到着した瞬間にダメージ
		if (rate >= 1.0f)
		{
			FinishSlash();
			step = STEP::After;
		}
		break;
	}
	case PlayerBreakFinishState::STEP::After:
	{
		//残心のアニメーションが終わったらカメラを戻す
		if (IsAnimeEnd())
		{
			ReturnStart();
			step = STEP::Return;
		}
		break;
	}
	case PlayerBreakFinishState::STEP::Return:
	{
		//カメラが元の位置に戻るまでその場で待つ
		if (IsReturnEnd()) { ChangeStateIdle(); }
		break;
	}
	default:
		break;
	}
}

void PlayerBreakFinishState::Exit(void)
{
	//途中で中断されても必ず後始末する
	FinishRelease();
}
