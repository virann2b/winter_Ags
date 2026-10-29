#include "ChilbiHitState.h"


ChilbiHitState::ChilbiHitState(
	std::function<void(void)> playAnimeHit,
	std::function<bool(void)> ownState,
	std::function<void(void)> ChangeStateInit,
	std::function<bool(void)> isAnimeEnd)
	:
	playAnimeHit(playAnimeHit),
	ownState(ownState),
	ChangeStateInit(ChangeStateInit),
	isAnimeEnd(isAnimeEnd)
{
}

void ChilbiHitState::OwnStateConditionUpdate(void)
{
	if (!ownState())
	{
		OwnChangeState();
	}
}

void ChilbiHitState::Enter(void)
{
	//HITアニメーション再生
	playAnimeHit();
}

void ChilbiHitState::Update(void)
{
	//アニメーション終了後,状態遷移する
	if (isAnimeEnd())
	{
		ChangeStateInit();
	}
}
