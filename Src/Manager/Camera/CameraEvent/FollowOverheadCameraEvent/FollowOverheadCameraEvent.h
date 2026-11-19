#pragma once

#include <algorithm>
#include <cmath>

#include "../CameraEventBase.h"
#include "../../CameraBase.h"
#include "../../../TimeScale/TimeScale.h"

/// <summary>
/// 対象の上から見下ろしながら追いかけるカメライベント
/// 開始時のカメラの横向きを保ったまま、対象の真上より少し後ろに付く
/// 自動では終わらないので、終わらせる時は CameraBase::EndEvent() を呼ぶ
/// </summary>
class FollowOverheadCameraEvent : public CameraEventBase
{
public:

	/// <param name="target">追いかける対象の座標（参照で持ち続けるので、ずっと存在する変数を渡す）</param>
	/// <param name="height">対象からの高さ</param>
	/// <param name="back">対象から後ろへのずらし（0だと真上になり向きが定まらないので少しずらす）</param>
	/// <param name="lookHeight">対象のどの高さを見るか</param>
	/// <param name="fov">視野角（ラジアン）</param>
	/// <param name="blendFrame">今のカメラから見下ろし位置へ移るフレーム数</param>
	FollowOverheadCameraEvent(const Vector3& target, float height, float back, float lookHeight, float fov, float blendFrame) :
		CameraEventBase(),
		target(target),
		height(height), back(back), lookHeight(lookHeight),
		endFov(fov),
		blendFrame(blendFrame),
		currentFrame(0.0f),
		startPos(), startAngle(), startFov(0.0f),
		yaw(0.0f)
	{
	}

	~FollowOverheadCameraEvent() override = default;

	void Start(CameraBase& camera) override
	{
		startPos = camera.GetPos();
		startAngle = camera.GetAngle();
		startFov = camera.GetFov();

		//今のカメラの横向きを保つ
		yaw = startAngle.y;

		currentFrame = 0.0f;
	}

	void Update(CameraBase& camera) override
	{
		//見下ろし位置と角度（対象が動いても毎フレーム追いかける）
		const Vector3 forward(sinf(yaw), 0.0f, cosf(yaw));
		const Vector3 desiredPos = target - forward * back + Vector3(0.0f, height, 0.0f);
		const Vector3 lookPos = target + Vector3(0.0f, lookHeight, 0.0f);
		Vector3 desiredAngle = CalcAngle(desiredPos, lookPos);

		//横回転が遠回りしないように、差を -π～π に収める
		float diffY = desiredAngle.y - startAngle.y;
		while (diffY > DX_PI_F) { diffY -= DX_TWO_PI_F; }
		while (diffY < -DX_PI_F) { diffY += DX_TWO_PI_F; }
		desiredAngle.y = startAngle.y + diffY;

		//開始直後は今のカメラから補間して移る
		float t = 1.0f;
		if (blendFrame > 0.0f)
		{
			currentFrame += TimeScale::Get();
			t = std::clamp(currentFrame / blendFrame, 0.0f, 1.0f);
			//イージング
			t = t * t * (3.0f - 2.0f * t);
		}

		camera.SetPos(startPos + (desiredPos - startPos) * t);
		camera.SetAngle(startAngle + (desiredAngle - startAngle) * t);
		camera.SetFov(startFov + (endFov - startFov) * t);
	}

	//自動では終了しない
	bool IsEnd(void) const override { return false; }

private:

	// 追いかける対象
	const Vector3& target;

	// 対象からの位置
	const float height;
	const float back;
	const float lookHeight;

	// 目標の視野角
	const float endFov;

	// 移るフレーム数
	const float blendFrame;
	float currentFrame;

	// 開始時カメラ情報
	Vector3 startPos;
	Vector3 startAngle;
	float startFov;

	// 開始時の横向き
	float yaw;

	// 位置と注視点から角度を割り出す
	static Vector3 CalcAngle(const Vector3& pos, const Vector3& lookAtPos)
	{
		Vector3 vec = lookAtPos - pos;
		if (vec.LengthSq() <= 0.000001f) { return Vector3(); }
		vec.Normalize();
		return Vector3(-asinf(std::clamp(vec.y, -1.0f, 1.0f)), atan2f(vec.x, vec.z), 0.0f);
	}
};
