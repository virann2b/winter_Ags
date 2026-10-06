#include "GunCamera.h"

GunCamera::GunCamera(const Vector3* targetPos, const Vector3* focusPos,
	const Vector3& initPos, const Vector3& initAngle,
	float backDist, float shoulder, float height, float fov)
	: CameraBase(initPos, initAngle, fov)
	, targetPos(targetPos), focusPos(focusPos)
	, backDist(backDist), shoulder(shoulder), height(height)
{
}

void GunCamera::NormalUpdate(void)
{
	if (targetPos == nullptr || focusPos == nullptr) { return; }

	// 自機→敵の水平方向
	Vector3 dir = *focusPos - *targetPos;
	dir.y = 0.0f;
	dir = dir.Normalized();

	// DxLib(左手系)での右方向
	Vector3 right(dir.z, 0.0f, -dir.x);

	// 自機の後ろ・右肩・少し上
	Vector3 desiredPos = *targetPos - dir * backDist + right * shoulder;
	desiredPos.y += height;

	SmoothCameraMove(pos, desiredPos);

	// 注視点は敵（照準は画面中央）
	Vector3 lookAt = *focusPos;
	angle = CalcCameraAngle(pos, lookAt);
}