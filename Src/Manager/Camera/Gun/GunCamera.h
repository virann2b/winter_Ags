#pragma once
#include "../CameraBase.h"
class GunCamera : public CameraBase
{
public:
	GunCamera(
		const Vector3* targetPos,   // 自機
		const Vector3* focusPos,    // ロックオン対象
		const Vector3& initPos,     // 切替前のカメラ位置（急に飛ばないため）
		const Vector3& initAngle,
		float backDist = 220.0f,    // 自機の後ろ
		float shoulder = 60.0f,     // 右肩へのずらし
		float height = 140.0f,    // 高さ
		float fov = 55.0f * (DX_PI_F / 180.0f)  // 少し狭めて照準感を出す
	);

private:
	void NormalUpdate(void) override;

	const Vector3* targetPos;
	const Vector3* focusPos;
	float backDist, shoulder, height;
};

