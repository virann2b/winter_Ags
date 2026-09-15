#pragma once

#include "../CameraBase.h"

class GameSpaceController;
struct Transform;

struct GameSpaceCameraParameter
{
	Vector3 lookAtOffset = Vector3(0.0f, 100.0f, 0.0f);

	float sideDistance = 700.0f;
	float sideHeight = 170.0f;
	float sideLookAhead = 140.0f;

	float planeDistance = 650.0f;
	float planeHeight = 280.0f;
	float planeSideOffset = 180.0f;

	float railDistance = 700.0f;
	float railHeight = 220.0f;
	float railLookAhead = 120.0f;

	float freeDistance = 500.0f;
	float freeHeight = 130.0f;
	float freeRotPower = 3.0f * (DX_PI_F / 180.0f);

	float smooth = 0.12f;

	float fov = 60.0f * (DX_PI_F / 180.0f);
};

class GameSpaceFollowCamera : public CameraBase
{
public:
	GameSpaceFollowCamera(
		const Transform& followAt,
		const GameSpaceController& gameSpace,
		const GameSpaceCameraParameter& parameter = GameSpaceCameraParameter()
	);
	~GameSpaceFollowCamera(void)override = default;

	void DrawDebug(void)const override;

protected:
	void NormalUpdate(void)override;

private:
	void Calculate(Vector3& desiredPos, Vector3& desiredLookAt, Vector3& desiredUp);

	const Transform* followAt;
	const GameSpaceController* gameSpace;
	GameSpaceCameraParameter parameter;
	Vector3 lookAtPos;
	Vector3 cameraUp;

	// ëÄçÏäpìx
	Vector3 controlAngle;

	// èIóπ
	void SubRelease(void)override;
};
