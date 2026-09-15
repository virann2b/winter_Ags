#include "PlayerMoveState.h"

#include "../../../Common/Vector3.h"
#include "../../../Common/Vector2.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../../Scene/Common/GameSpace/GameSpaceController.h"

PlayerMoveState::PlayerMoveState(
	const GameSpaceController& gameSpace,
	const SPACE_CONSTRAINT& playerSpaceConstraint,

	const Vector3& playerPos,

	std::function<void(const Vector3& vec)> playerMoveAccel,

	const bool& playerIsGround,
	float& playerVelocityY
):
	CharacterStateBase(),

	gameSpace(gameSpace),
	playerSpaceConstraint(playerSpaceConstraint),

	playerPos(playerPos),

	playerMoveAccel(playerMoveAccel),

	playerIsGround(playerIsGround),
	playerVelocityY(playerVelocityY)
{
}

void PlayerMoveState::OwnStateConditionUpdate(void)
{
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now ||
		Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) {

		OwnChangeState();
	}
}

void PlayerMoveState::Update(void)
{
	playerMoveAccel(GetMoveDirection());

	// ジャンプと重力・接地判定を確認する
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down && playerIsGround) {

		playerVelocityY = 14.0f;

		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down && playerIsGround) {

			playerVelocityY = 14.0f;
		}
	}
}

Vector3 PlayerMoveState::GetMoveDirection(void) const
{
	if (gameSpace.IsStopInput()) { return Vector3(); }

	Vector2 input = Input::GetIns().GetLeftStickVec();

	// コントローラー入力がない場合はキーボードを使う
	if (input == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { input.x += 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { input.x -= 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { input.y += 1.0f; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { input.y -= 1.0f; }


		if (input != 0.0f) { input.Normalize(); }
	}

	return gameSpace.ConvertMoveInput(input, playerPos, CurrentCamera::Get().GetPos(), playerSpaceConstraint);
}