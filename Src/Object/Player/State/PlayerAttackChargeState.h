#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"
struct Vector3;

class PlayerAttackChargeState : public CharacterStateBase
{
public:

	PlayerAttackChargeState(
		std::function<void(void)> PlayerAttackChargeStart,
		std::function<void(void)> PlayerAttackChargeEnd,
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



#pragma endregion


#pragma region 受け取る参照変数・関数

	std::function<void(void)> PlayerAttackChargeStart;

	std::function<void(void)> PlayerAttackChargeEnd;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

	// 通常攻撃ごとのアニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion

	STEP step;

};