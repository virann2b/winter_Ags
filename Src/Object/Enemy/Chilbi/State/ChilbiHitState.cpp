#include "ChilbiHitState.h"

#include "../../../../Manager/Input/InputManager.h"

ChilbiHitState::ChilbiHitState(
	std::function<void(void)> playAnimeHit,
	std::function<void(void)> ChangeStateInit,
	std::function<bool(void)> isAnimeEnd)
	:
	playAnimeHit(playAnimeHit),
	ChangeStateInit(ChangeStateInit),
	isAnimeEnd(isAnimeEnd)
{
}

void ChilbiHitState::OwnStateConditionUpdate(void)
{
	//入力
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).down) { OwnChangeState(); }
}

void ChilbiHitState::Enter(void)
{
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
