#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Scene/Common/GameSpace/SpaceDefine.h"

class GameSpaceController;

struct Vector3;

class PlayerMoveState : public CharacterStateBase
{
public:
	PlayerMoveState(
		const GameSpaceController& gameSpace,
		const SPACE_CONSTRAINT& playerSpaceConstraint,

		const Vector3& playerPos,

		std::function<void(const Vector3& vec)> playerMoveAccel,

		const bool& playerIsGround,
		float& playerVelocityY
	);
	~PlayerMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 更新処理
	void Update(void);

private:

#pragma region 受け取る参照

	// ゲーム空間制約
	const GameSpaceController& gameSpace;

	// プレイヤーに適用されている空間制約
	const SPACE_CONSTRAINT& playerSpaceConstraint;

	// プレイヤーの座標
	const Vector3& playerPos;

	// 加速移動関数
	std::function<void(const Vector3& vec)> playerMoveAccel;

	// 接地判定
	const bool& playerIsGround;

	// 縦方向の加速
	float& playerVelocityY;

#pragma endregion

	// 入力方向の取得
	Vector3 GetMoveDirection(void) const;
};