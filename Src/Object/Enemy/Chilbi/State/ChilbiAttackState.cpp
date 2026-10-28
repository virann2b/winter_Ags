#include <DxLib.h>
#include "ChilbiAttackState.h"

ChilbiAttackState::ChilbiAttackState(
	std::function<void(void)> playAnimeAttack_1,
	std::function<void(void)> playAnimeAttack_2, 
	std::function<void(void)> ChangeStateIdle,
	std::function<bool(void)> isAnimeEnd)
	:
	playAnimeAttack_1(playAnimeAttack_1),
	playAnimeAttack_2(playAnimeAttack_2),
	ChangeStateIdle(ChangeStateIdle),
	isAnimeEnd(isAnimeEnd)
{
	//パターン１
	patternTable_[(size_t)PATTERN::Pattern_1] =
	{
		this->playAnimeAttack_1,
		this->playAnimeAttack_2
	};

	//パターン２
	patternTable_[(size_t)PATTERN::Pattern_2] =
	{
		this->playAnimeAttack_1,
		this->playAnimeAttack_1
	};
}


void ChilbiAttackState::OwnStateConditionUpdate(void)
{

}

void ChilbiAttackState::Enter(void)
{
	actionIndex_ = 0;
	currentPattern_ = (PATTERN)(GetRand((int)PATTERN::Max - 1));
	PlayCurrentAction();
}

void ChilbiAttackState::Update(void)
{
	if (!isAnimeEnd()) return;

	actionIndex_++;

	const auto& actions = patternTable_[(size_t)currentPattern_];

	if (actionIndex_ < actions.size())
	{
		PlayCurrentAction();
	}
	else
	{
		//パターンを最後までやり終えたら待機へ
		ChangeStateIdle();
	}
}

void ChilbiAttackState::SetPattern(PATTERN pattern)
{
	if (pattern == PATTERN::None || pattern == PATTERN::Max)
	{
		return;
	}
	currentPattern_ = pattern;
}

void ChilbiAttackState::PlayCurrentAction(void)
{
	const auto& actions = patternTable_[(size_t)currentPattern_];

	if (actionIndex_ < actions.size())
	{
		actions[actionIndex_]();
	}
}
