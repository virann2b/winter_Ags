#include "GameSpaceFollowCamera.h"

#include <algorithm>

#include "../../../Utility/Utility.h"

#include "../../../Object/Common/Transform/Transform.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"
#include "../../Input/InputManager.h"

GameSpaceFollowCamera::GameSpaceFollowCamera(
	const Transform& followAt,
	const GameSpaceController& gameSpace,
	const GameSpaceCameraParameter& parameter
) :
	CameraBase(
		Vector3(),
		Vector3(0.0f, followAt.angle.y, 0.0f),
		parameter.fov
	),
	followAt(&followAt),
	gameSpace(&gameSpace),
	parameter(parameter),
	lookAtPos(),
	cameraUp(Vector3::Yonly(1.0f)),
	controlAngle(0.0f, followAt.angle.y, 0.0f)
{
	this->parameter.smooth = std::clamp(this->parameter.smooth, 0.001f, 1.0f);

	Calculate(pos, lookAtPos, cameraUp);
	angle = CalcCameraAngle(pos, lookAtPos);

	// マウスを固定しておく
	Input::GetIns().SetMouseFixed(true);
}

void GameSpaceFollowCamera::NormalUpdate(void)
{
	// 安全
	if (followAt == nullptr || gameSpace == nullptr) { return; }

	// 目標情報変数定義
	Vector3 desiredPos = pos;
	Vector3 desiredLookAt = lookAtPos;
	Vector3 desiredUp = cameraUp;

	// 実際の目標地点を算出
	Calculate(desiredPos, desiredLookAt, desiredUp);

	// 補間数値
	const float smooth = parameter.smooth;

	// 座標補間
	pos += (desiredPos - pos) * smooth;
	// 注視点補間
	lookAtPos += (desiredLookAt - lookAtPos) * smooth;
	// カメラの上方向補間
	cameraUp += (desiredUp - cameraUp) * smooth;

	if (cameraUp.LengthSq() <= 0.000001f) { cameraUp = Vector3::Yonly(1.0f); }
	else { cameraUp.Normalize(); }

	// 座標と注視点から角度を算出
	angle = CalcCameraAngle(pos, lookAtPos);
}

void GameSpaceFollowCamera::Calculate(Vector3& desiredPos, Vector3& desiredLookAt, Vector3& desiredUp)
{
	const Vector3 targetPos = followAt->pos;
	const Vector3 moveVelocity = followAt->Velocity();
	const MovementPlane& plane = gameSpace->GetPlane();
	Vector3 right = plane.right.Normalized();
	Vector3 depth = plane.depth.Normalized();
	Vector3 up = plane.up.Normalized();
	if (right.LengthSq() <= 0.000001f) { right = Vector3::Xonly(1.0f); }
	if (depth.LengthSq() <= 0.000001f) { depth = Vector3::Zonly(1.0f); }
	if (up.LengthSq() <= 0.000001f) { up = Vector3::Yonly(1.0f); }
	desiredLookAt = targetPos + parameter.lookAtOffset;
	desiredUp = up;

	switch (gameSpace->GetMode()) {
	case GAME_SPACE_MODE::Side2D: {
		const float ahead = std::clamp(moveVelocity.Dot(right) * 8.0f, -parameter.sideLookAhead, parameter.sideLookAhead);
		desiredLookAt += right * ahead;
		desiredPos = desiredLookAt - depth * parameter.sideDistance + up * parameter.sideHeight;
		break;
	}
	case GAME_SPACE_MODE::Plane3D:
		desiredPos = desiredLookAt - depth * parameter.planeDistance + right * parameter.planeSideOffset + up * parameter.planeHeight;
		break;
	case GAME_SPACE_MODE::Rail: {
		Vector3 tangent = gameSpace->GetRail().GetTangent(targetPos);
		if (tangent.LengthSq() <= 0.000001f) { tangent = right; }
		else { tangent.Normalize(); }
		Vector3 side = tangent.Cross(up);
		if (side.LengthSq() <= 0.000001f) { side = depth; }
		else { side.Normalize(); }
		const float ahead = std::clamp(moveVelocity.Dot(tangent) * 8.0f, -parameter.railLookAhead, parameter.railLookAhead);
		desiredLookAt += tangent * ahead;
		desiredPos = desiredLookAt - side * parameter.railDistance + up * parameter.railHeight;
		break;
	}
	case GAME_SPACE_MODE::Free3D: {
		// カメラ回転操作
		Vector3 rotInput;
		if (RotationInput(rotInput)) { controlAngle += rotInput * parameter.freeRotPower; }

		// 上下回転を制限
		controlAngle.x = std::clamp(
			controlAngle.x,
			-20.0f * (DX_PI_F / 180.0f),
			55.0f * (DX_PI_F / 180.0f)
		);

		// 操作角度からカメラ座標を算出
		const float horizontal = cosf(controlAngle.x) * parameter.freeDistance;

		const Vector3 back(
			-sinf(controlAngle.y) * horizontal,
			sinf(controlAngle.x) * parameter.freeDistance + parameter.freeHeight,
			-cosf(controlAngle.y) * horizontal
		);

		desiredPos = desiredLookAt + back;
		desiredUp = Vector3::Yonly(1.0f);
		break;
	}
	}
}

void GameSpaceFollowCamera::DrawDebug(void)const
{
	CameraBase::DrawDebug();
	if (gameSpace != nullptr) { gameSpace->DrawDebug(); }
}

void GameSpaceFollowCamera::SubRelease(void)
{
	// 生成時固定したマウスを戻す
	Input::GetIns().SetMouseFixed(false);
}
