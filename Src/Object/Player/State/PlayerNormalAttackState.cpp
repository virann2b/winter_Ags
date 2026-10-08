#include "PlayerNormalAttackState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"



PlayerNormalAttack::PlayerNormalAttack(
	std::function<void(void)> ChangeStateIdle,
	std::function<void(void)> playAnimeNormalAttack_1,
	std::function<void(void)> playAnimeNormalAttack_2,
	std::function<void(void)> playAnimeNormalAttack_3,
	std::function<bool(void)> isAnimeEnd,
	std::function<float(void)> animRate)
	:
	ChangeStateIdle(ChangeStateIdle),
	playAnimeNormalAttack_1(playAnimeNormalAttack_1),
	playAnimeNormalAttack_2(playAnimeNormalAttack_2),
	playAnimeNormalAttack_3(playAnimeNormalAttack_3),
	isAnimeEnd(isAnimeEnd),
	animRate(animRate),
	comboIndex_(0),
	nextInput_(false)
{
}

void PlayerNormalAttack::OwnStateConditionUpdate(void)
{
	//入力
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down) { OwnChangeState(); }
}

void PlayerNormalAttack::Enter(void)
{
	//コンボ回数リセット
	comboIndex_ = 0;
	//入力フラグリセット
	nextInput_ = false;

	//通常攻撃1再生
	playAnimeNormalAttack_1();
}

void PlayerNormalAttack::Update(void)
{
	
	// 入力受付(先に判定する)
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttack).down)
	{
		if (IsComboInputWindow())
		{
			nextInput_ = true;
		}
	}

	// アニメ終了判定
	if (isAnimeEnd())
	{
		if (nextInput_ && comboIndex_ + 1 < COMBO_MAX)
		{
			++comboIndex_;
			//リセット
			nextInput_ = false;         
			PLAY_ANIM_TABLE[comboIndex_]();
		}
		else
		{
			//待機状態に遷移
			ChangeStateIdle();
		}
	}
}

const bool PlayerNormalAttack::IsComboInputWindow(void)
{
	//アニメーション再生比率
	float rate = animRate();   // 0.0~1.0
	return rate >= INPUT_RATIO_START;
}
