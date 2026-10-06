#include "GameCamera.h"

#include <algorithm>
#include "../../../Utility/Utility.h"
#include "../../../Application/Application.h"

#include "../../Input/InputManager.h"
#include "../../TimeScale/TimeScale.h"

GameCamera::GameCamera(
	const Vector3* targetPos,
	const Vector3* focusPos,
	const Vector3& cameraOffset,
	const Vector3& lookAtOffset,
	float ROT_POWER,
	const Vector3& initAngle,
	float TARGET_DISTANCE_MIN, float TARGET_DISTANCE_MAX,
	float fov,
	bool initialModeRemote,
	KEY_TYPE toggleKey,
	KEY_TYPE gunKey
) :
	CameraBase(Vector3(), Vector3(), fov),

	targetPos(targetPos),
	focusPos(focusPos),

	cameraOffset(cameraOffset),
	lookAtOffset(lookAtOffset),
	lookAtPos(),

	TARGET_DISTANCE_MIN(TARGET_DISTANCE_MIN),
	TARGET_DISTANCE_MAX(TARGET_DISTANCE_MAX),

	ROT_POWER(ROT_POWER),
	controlAngle(initAngle),

	toggleKey(toggleKey),
	gunKey(gunKey),

	mode(initialModeRemote ? Mode::Remote : Mode::Auto),
	baseFov(fov)
{
	if (mode == Mode::Remote && targetPos != nullptr) {
		SnapRemote();
		if (ROT_POWER != 0.0f) { Input::GetIns().SetMouseFixed(true); }
	}
	else {
		// Auto モード初期更新して位置を決める
		UpdateAuto();
	}
}

bool GameCamera::SetMode(Mode m)
{
	if (mode == m) { return true; }

	// Auto / Gun は対象がいないと成立しない
	if ((m == Mode::Auto || m == Mode::Gun) && focusPos == nullptr) { return false; }

	// Gun は Auto からのみ入れる
	if (m == Mode::Gun && mode != Mode::Auto) { return false; }

	// Remote から出るときはマウス固定を戻す
	if (mode == Mode::Remote && ROT_POWER != 0.0f) {
		Input::GetIns().SetMouseFixed(false);
	}

	mode = m;

	// Remote に入るときは位置を合わせてマウス固定
	if (mode == Mode::Remote) {
		SnapRemote();
		if (ROT_POWER != 0.0f) { Input::GetIns().SetMouseFixed(true); }
	}

	return true;
}

void GameCamera::ToggleTarget(void)
{
	// Remote -> Auto、それ以外(Auto/Gun) -> Remote
	SetMode(mode == Mode::Remote ? Mode::Auto : Mode::Remote);
}

void GameCamera::ToggleGun(void)
{
	if (mode == Mode::Auto) { SetMode(Mode::Gun); }
	else if (mode == Mode::Gun) { SetMode(Mode::Auto); }
	// Remote 中は何もしない（ターゲットON時のみ銃モード可）
}

void GameCamera::SnapRemote(void)
{
	if (targetPos == nullptr) { return; }

	const Vector3 rot = Vector3::XYonly(controlAngle.x, controlAngle.y);
	pos = *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ rot }));
	lookAtPos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ rot }));
	angle = CalcCameraAngle(pos, lookAtPos);
}

void GameCamera::NormalUpdate(void)
{
	if (targetPos == nullptr) { return; }

	// ターゲット対象が消えていたら強制的に Remote へ
	if (mode != Mode::Remote && focusPos == nullptr) {
		SetMode(Mode::Remote);
	}

	// 入力（カメライベント中は NormalUpdate 自体が呼ばれないので受け付けない）
	if (Input::GetIns().GetInfo(toggleKey).down) { ToggleTarget(); }
	if (Input::GetIns().GetInfo(gunKey).down) { ToggleGun(); }

	// 視野角：Gun中は絞り、それ以外は通常へ戻す
	const float targetFov = (mode == Mode::Gun) ? GUN_FOV : baseFov;
	fov += (targetFov - fov) * FOV_LERP_RATE * TimeScale::Get();

	switch (mode) {
	case Mode::Remote: UpdateRemote(); break;
	case Mode::Auto:   UpdateAuto();   break;
	case Mode::Gun:    UpdateGun();    break;
	}
}

void GameCamera::UpdateRemote(void)
{
	Vector3 rotInput = Vector3();
	if (RotationInput(rotInput)) {

		controlAngle += (rotInput * ROT_POWER) * TimeScale::Get();

		// 回転の数値制御
		if (controlAngle.y <= Deg2Rad(0.0f)) { controlAngle.y += Deg2Rad(360.0f); }
		if (controlAngle.y >= Deg2Rad(360.0f)) { controlAngle.y -= Deg2Rad(360.0f); }
		if (controlAngle.x < Deg2Rad(-85.0f)) { controlAngle.x = Deg2Rad(-85.0f); }
		if (controlAngle.x > Deg2Rad(85.0f)) { controlAngle.x = Deg2Rad(85.0f); }
	}

	const Vector3 rot = Vector3::XYonly(controlAngle.x, controlAngle.y);
	SmoothCameraMove(pos, *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ rot })));
	SmoothCameraMove(lookAtPos, *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ rot })));

	angle = CalcCameraAngle(pos, lookAtPos);
}

void GameCamera::UpdateAuto(void)
{
	if (targetPos == nullptr || focusPos == nullptr) { return; }

	// ２点間ベクトル
	Vector3 atToTarget = *targetPos - *focusPos;

	// fovから必要距離を計算（縦fov基準。Gun中のfov変化の影響を避けるため baseFov を使う）
	float needDist = std::clamp((atToTarget.Length() * 0.5f) / tanf(baseFov * 0.5f), TARGET_DISTANCE_MIN, TARGET_DISTANCE_MAX);

	// 対象から見て自機のさらに先にカメラを置く
	Vector3 backDir = atToTarget.Normalized();

	// 目標カメラ位置
	Vector3 desiredPos = *targetPos + backDir * needDist;

	// 高さ補正
	desiredPos.y += std::clamp(((*targetPos - *focusPos) * 0.5f).Length(), 250.0f, 400.0f);

	SmoothCameraMove(pos, desiredPos);
	if (pos.y <= CAMERA_DOWN) { pos.y = CAMERA_DOWN; }

	// 注視点
	lookAtPos = (*targetPos + *focusPos) * 0.5f;
	if (lookAtPos.y <= FOCUS_DOWN) { lookAtPos.y = FOCUS_DOWN; }

	angle = CalcCameraAngle(pos, lookAtPos);
}

void GameCamera::UpdateGun(void)
{
	if (targetPos == nullptr || focusPos == nullptr) { return; }

	// 自機→対象の水平方向
	Vector3 dir = *focusPos - *targetPos;
	dir.y = 0.0f;
	if (dir.LengthSq() <= 0.000001f) { return; }
	dir = dir.Normalized();

	// DxLib(左手系)での右方向
	Vector3 right(dir.z, 0.0f, -dir.x);

	// 自機の後ろ・右肩・少し上
	Vector3 desiredPos = *targetPos - dir * GUN_BACK_DIST + right * GUN_SHOULDER;
	desiredPos.y += GUN_HEIGHT;

	SmoothCameraMove(pos, desiredPos);
	if (pos.y <= CAMERA_DOWN) { pos.y = CAMERA_DOWN; }

	// 注視点は対象
	lookAtPos = *focusPos;
	if (lookAtPos.y <= FOCUS_DOWN) { lookAtPos.y = FOCUS_DOWN; }

	angle = CalcCameraAngle(pos, lookAtPos);
}

void GameCamera::SubRelease(void)
{
	// Remote で固定したマウスを戻す（終了時）
	if (mode == Mode::Remote && ROT_POWER != 0.0f) {
		Input::GetIns().SetMouseFixed(false);
	}
}
