#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

class PlayerAttackAirState : public CharacterStateBase
{
public:

	enum class STEP
	{
		None = -1,

		Start,
		Air,
		End,

		Max
	};

	PlayerAttackAirState(
		const bool& isGround,
		bool& isMaxJump,
		std::function<void(void)> playAnimAirAttackStart,
		std::function<void(void)> playAnimAirAttackEnd,
		std::function<void(void)> arriveAttackRange,
		std::function<float(void)> animRate,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> ChangeStateIdle);

	~PlayerAttackAirState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

private:

	//範囲攻撃発生比率
	static constexpr float RANGEATTACK_RAITE = 0.2f;


#pragma region 受け取る参照変数・関数

	// 空中攻撃開始アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimAirAttackStart;
	// 空中攻撃終了アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimAirAttackEnd;
	// 範囲攻撃発生ポインタ
	const std::function<void(void)> arriveAttackRange;
	// アニメーション再生
	const std::function<float(void)> animRate;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;
	//アニメーション終了関数ポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 地面と設置しているか
	const bool& isGround;
	// 最大ジャンプ数に達しているか
	bool& isMaxJump;

#pragma endregion


	//アニメーション状態
	STEP step;
};

