#include "ChilbiDieState.h"

ChilbiDieState::ChilbiDieState(
	std::function<void(void)> playAnimeDie,
	std::function<bool(void)> isDead,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> dieEnd
):
	playAnimeDie(playAnimeDie),
	isDead(isDead),
	isAnimeEnd(isAnimeEnd),
	dieEnd(dieEnd)
{
}

void ChilbiDieState::OwnStateConditionUpdate(void)
{
	if (!isDead()) { return; }

	OwnChangeState();
}

void ChilbiDieState::Enter(void)
{
	isDieEnd = false;

	playAnimeDie();
}

void ChilbiDieState::Update(void)
{
	//死亡アニメーションが終わったら1回だけ終了処理
	if (isDieEnd || !isAnimeEnd()) { return; }

	isDieEnd = true;
	dieEnd();
}
