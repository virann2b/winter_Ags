#include "PlayerHitState.h"

PlayerHitState::PlayerHitState(
	std::function<void(void)> PlayAnimeHit,
	std::function<bool(void)> IsAnimeEnd,
	std::function<bool(void)> OwnState,
	std::function<void(void)> ChangeStateIdle
):
	PlayAnimeHit(PlayAnimeHit),
	IsAnimeEnd(IsAnimeEnd),
	OwnState(OwnState),
	ChangeStateIdle(ChangeStateIdle)
{
}

void PlayerHitState::OwnStateConditionUpdate(void)
{
	//ダメージをくらうとHITステートに遷移
	//	if (!OwnState())
	//	{
	//		OwnChangeState();
	//	}
}

void PlayerHitState::Enter(void)
{
	PlayAnimeHit();
}

void PlayerHitState::Update(void)
{
	if (IsAnimeEnd())
	{
		ChangeStateIdle();
	}
}
