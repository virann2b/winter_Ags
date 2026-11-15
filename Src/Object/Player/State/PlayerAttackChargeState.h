#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"
struct Vector3;

class PlayerAttackChargeState : public CharacterStateBase
{
public:

	PlayerAttackChargeState(
		std::function<void(void)> PlayerAttackChargeStart,
		std::function<void(void)> PlayerAttackChargeEnd,
		std::function<void(void)> playAnimeAttackCharge_1,
		std::function<void(void)> playAnimeAttackCharge_2,
		std::function<void(void)> playAnimeAttackCharge_3,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> ChangeStateIdle
	);

	~PlayerAttackChargeState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:

	enum class STEP
	{
		None = -1,

		Charge,

		Attack,

		Max
	};

#pragma region 定数

	//最大コンボ回数
	static constexpr int COMBO_MAX = 3;

	//受付開始入力比率
	static constexpr float INPUT_RATIO_START = 0.4f;

#pragma endregion


#pragma region 受け取る参照変数・関数

	//強タメ攻撃開始アニメーション
	std::function<void(void)> PlayerAttackChargeStart;
	//強タメ攻撃終了アニメーション
	std::function<void(void)> PlayerAttackChargeEnd;

	//生成アニメーション関数テーブル
	const std::function<void(void)> PLAY_ANIM_TABLE[(int)COMBO_MAX];

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;
	// 通常攻撃ごとのアニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion

	STEP step;

	// 現在のコンボ段階 (0始まり)
	int  comboIndex_;		// 現在のコンボ段階 (0始まり)
	bool nextInput_;		// 次段への先行入力

	//次回コンボ入力受付(再生比率で判断)
	const bool IsComboInputWindow(void);

};