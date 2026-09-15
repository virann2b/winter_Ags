#pragma once

#include "../ActorBase/ActorBase.h"
#include "../../../Common/Vector2.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"

class DebugObjectBase : public ActorBase
{
public:
	DebugObjectBase(
		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(),
		INIT_POS(pos),
		isOperator(isOperator)
	{
		trans.pos = pos;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	DebugObjectBase(
		const std::string& parameterPath,

		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(parameterPath),
		INIT_POS(pos),
		isOperator(isOperator)
	{
		trans.pos = pos;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	~DebugObjectBase()override = default;

private:
	// 操作可能かどうか
	bool isOperator;

	// 初期座標
	const Vector3 INIT_POS;

	void SubInit(void)override {

		trans.pos = INIT_POS;

		// 移動加速力
		ACCEL_RATE = 3.0f;
		// 移動減速力
		DECEL_RATE = 3.0f;
		// 最大移動速度
		ACCEL_MAX = 15.0f;
	}

	Vector3 GetMoveDirection(void)const {
		const GameSpaceController& gameSpace = GetGameSpaceController();

		if (gameSpace.IsStopInput()) { return Vector3(); }

		Vector2 input = Input::GetIns().GetLeftStickVec();

		// コントローラー入力がない場合はキーボードを使う
		if (input == 0.0f) {
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { input.x += 1.0f; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { input.x -= 1.0f; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { input.y += 1.0f; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { input.y -= 1.0f; }

			if (input.LengthSq() > 1.0f) { input.Normalize(); }
		}

		return gameSpace.ConvertMoveInput(input, trans.pos, CurrentCamera::Get().GetPos(), GetSpaceConstraint());
	}

	void ResetPos(void) { trans.pos = INIT_POS; }

	void SubUpdate(void)override {
		if (!isOperator) { return; }

		if (CheckHitKey(KEY_INPUT_Z) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault); }

		if (CheckHitKey(KEY_INPUT_X) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::FixedPlane); }

		if (CheckHitKey(KEY_INPUT_C) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::Rail); }

		if (CheckHitKey(KEY_INPUT_V) != 0) { SetSpaceConstraint(SPACE_CONSTRAINT::None); }

		MoveAccel(GetMoveDirection());

		// ジャンプと重力・接地判定を確認する
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down && isGround) { velocity.y = 14.0f; }

		if (CheckHitKey(KEY_INPUT_R) != 0) { ResetPos(); }
	}
};