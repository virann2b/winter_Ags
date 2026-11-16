#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class PlayerJumpState : public CharacterStateBase
{
public:

	PlayerJumpState(
		float JUMP_POWER,

		float& VelocityY,
		const bool& IsGround,
		bool& IsMaxJump,

		std::function<void(const Vector3&)> MoveAccel,

		std::function<void(void)> PlayAnimeJumpStart,
		std::function<void(void)> PlayAnimeJumpLoop,
		std::function<void(void)> PlayAnimeStamp,

		std::function<bool(void)> IsAnimeEnd,

		std::function<void(void)> ChangeStateIdle
	);

	~PlayerJumpState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:

#pragma region 定数

	// ジャンプ力
	const float JUMP_POWER;

	// ジャンプ回数の最大値
	static constexpr int MAX_JUMP_COUNT = 2;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// Y軸加速度
	float& VelocityY;

	// 地面と設置しているか
	const bool& IsGround;

	// ジャンプカウント最大フラグ
	bool& IsMaxJump;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> MoveAccel;

	// ジャンプ開始アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAnimeJumpStart;
	// ジャンプ滞空アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAnimeJumpLoop;
	// 着地アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAnimeStamp;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

#pragma endregion

	// ステップ
	enum class STEP {

		// ジャンプ開始
		Start,

		// 滞空
		Loop,

		// 着地
		Stamp,
	};

	// 現在ステップ
	STEP step;

	// ジャンプカウント
	int jumpCount;
	bool isAirborne;

	void AlwaysUpdate(void)override;
};