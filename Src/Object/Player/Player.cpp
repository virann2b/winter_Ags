#include "Player.h"

#include "../../Utility/Utility.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Camera/CurrentCamera.h"

#include "../../Scene/Common/GameSpace/GameSpaceController.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "State/PlayerMoveState.h"

void Player::Load(void)
{
	// モデルをロード
	trans.LoadModel("Player/Player");

#pragma region 当たり判定情報設定

	// メインのカプセルコライダーを設定
	ColliderCreate(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			GetParameterToVector3("Collider", "StartPos"),
			GetParameterToVector3("Collider", "EndPos"),
			GetParameter("Collider", "Radius")
		)
	);

#pragma endregion

#pragma region 状態初期設定

	// 移動状態を追加
	AddState(
		STATE::Move,
		new PlayerMoveState(
			GetGameSpaceController(),
			GetSpaceConstraint(),
			trans.pos,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			isGround,
			velocity.y
		)
	);

#pragma endregion
}

void Player::CharacterInit(void)
{
	// モデルの角度のズレを設定
	trans.localAngle.y = Deg2Rad(GetParameter("Init", "angle"));

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 3.0f;
	// 加速最大値を設定
	ACCEL_MAX = 15.0f;

	// 初期状態を設定
	ChangeState(STATE::Move);
}

void Player::CharacterUpdate(void)
{
	if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

	if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

	if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

	if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }
}

void Player::CharacterDraw(void)
{
}

void Player::CharacterAlphaDraw(void)
{
}

void Player::CharacterUiDraw(void)
{
}

void Player::CharacterRelease(void)
{
}