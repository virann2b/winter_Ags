#include "PlayerAttackChargeState.h"

#include"../../../Manager/TimeScale/TimeScale.h"

#include "../../../Manager/Input/InputManager.h"


PlayerAttackChargeState::PlayerAttackChargeState(
	std::function<void(void)> PlayerAttackChargeStart,
	std::function<void(void)> PlayerAttackChargeEnd,
	std::function<void(void)> playAnimeAttackCharge_1,
	std::function<void(void)> playAnimeAttackCharge_2,
	std::function<void(void)> playAnimeAttackCharge_3,
	std::function<float(void)> AnimRate,
	std::function<void(bool)> SwordJugge,
	std::function<bool(void)> IsAnimeEnd,
	std::function<void(void)> ChangeStateIdle
	):
	PlayerAttackChargeStart(PlayerAttackChargeStart),
	PlayerAttackChargeEnd(PlayerAttackChargeEnd),
	PLAY_ANIM_TABLE
	{
		playAnimeAttackCharge_1,
		playAnimeAttackCharge_2,
		playAnimeAttackCharge_3,
	},
	AnimRate(AnimRate),
	SwordJugge(SwordJugge),
	IsAnimeEnd(IsAnimeEnd),
	ChangeStateIdle(ChangeStateIdle),
	step(STEP::None),
	nextInput_(false),
	comboIndex_(-1)
{
}

void PlayerAttackChargeState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong).down) { OwnChangeState(); }
}


void PlayerAttackChargeState::Enter(void)
{
	//コンボ回数リセット
	comboIndex_ = 0;
	//入力フラグリセット
	nextInput_ = false;

	holdCounter = 0.0f;
	chargeCounter = 0.0f;

	//入力判定待ち
	step = STEP::Wait;
	
}

void PlayerAttackChargeState::Update(void)
{

	const auto& key = Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackStrong);

	switch (step)
	{
	case PlayerAttackChargeState::STEP::Wait:
	{
		if (key.up)
		{
			//強コンボ攻撃
			SwordJugge(true);
			PLAY_ANIM_TABLE[comboIndex_]();
			step = STEP::Combo;

			break;
		}

		//長押し時間を取得
		holdCounter += TimeScale::Get();
		//一定時間を超えた場合タメに変更
		if (holdCounter >= HOLD_THRESHOLD)
		{
			//タメアニメーション再生
			PlayerAttackChargeStart();
			step = STEP::Charge;
		}

		break;

	}
	case PlayerAttackChargeState::STEP::Charge:
	{
		chargeCounter += TimeScale::Get();

		//タメアニメ-ションより入力時間が短い場合はそのまま待機状態に戻る
		if (key.up)
		{
			if (chargeCounter >= CHARGE_TIME)
			{
				//タメ攻撃へ
				SwordJugge(true);
				PlayerAttackChargeEnd();
				step = STEP::ChargeAttack;
				break;
			
			}
			else
			{
				//タメ時間が足りなかった場合は待機状態へ
				ChangeStateIdle();
			}
		}

		break;
	}
	case PlayerAttackChargeState::STEP::ChargeAttack:
	{
		if (IsAnimeEnd())
		{
			//タメ攻撃の再生終了後、待機状態に遷移
			ChangeStateIdle();
		}
		break;
	}
	case PlayerAttackChargeState::STEP::Combo:
	{
		// 入力受付(先に判定する)
		if (key.down)
		{
			if (IsComboInputWindow())
			{
				nextInput_ = true;
			}
		}

		// アニメ終了判定
		if (IsAnimeEnd())
		{
			if (nextInput_ && comboIndex_ + 1 < COMBO_MAX)
			{
				++comboIndex_;
				//リセット
				nextInput_ = false;
				PLAY_ANIM_TABLE[comboIndex_]();

				//コンボ継続
				step = STEP::Combo;
			}
			else
			{
				//待機状態に遷移
				ChangeStateIdle();
			}
		}

		break;
	}
	}

}

void PlayerAttackChargeState::Exit(void)
{
	step = STEP::Wait;
}

const bool PlayerAttackChargeState::IsComboInputWindow(void)
{
	//アニメーション再生比率
	float rate = AnimRate();  
	return rate >= INPUT_RATIO_START;
}