#pragma once

#include <algorithm>
#include <cmath>

#include "../CameraEventBase.h"
#include "../../CameraBase.h"
#include "../../../TimeScale/TimeScale.h"

/// <summary>
/// 指定した位置から指定した注視点を見続けるカメライベント
/// 自動では終わらないので、終わらせる時は CameraBase::EndEvent() を呼ぶ
/// （別のイベントを StartEvent した場合も自動で終了する）
/// </summary>
class FixedCameraEvent : public CameraEventBase
{
public:

	/// <summary>
	/// </summary>
	/// <param name="pos">カメラの位置</param>
	/// <param name="lookAtPos">注視点</param>
	/// <param name="fov">視野角（ラジアン）</param>
	/// <param name="blendFrame">今のカメラから移動するフレーム数（0で即切り替え）</param>
	FixedCameraEvent(const Vector3& pos, const Vector3& lookAtPos, float fov, float blendFrame = 0.0f) :
		CameraEventBase(),
		startPos(), startAngle(), startFov(0.0f),
		endPos(pos), endAngle(CalcAngle(pos, lookAtPos)), endFov(fov),
		blendFrame(blendFrame),
		currentFrame(0.0f)
	{
	}

	~FixedCameraEvent() override = default;

	void Start(CameraBase& camera) override
	{
		startPos = camera.GetPos();
		startAngle = camera.GetAngle();
		startFov = camera.GetFov();

		//横回転が遠回りしないように、差を -π～π に収める
		float diffY = endAngle.y - startAngle.y;
		while (diffY > DX_PI_F) { diffY -= DX_TWO_PI_F; }
		while (diffY < -DX_PI_F) { diffY += DX_TWO_PI_F; }
		endAngle.y = startAngle.y + diffY;

		currentFrame = 0.0f;

		//即切り替えの場合はこの時点で反映
		if (blendFrame <= 0.0f) { Apply(camera, 1.0f); }
	}

	void Update(CameraBase& camera) override
	{
		//即切り替えの場合はその位置のまま
		if (blendFrame <= 0.0f) { Apply(camera, 1.0f); return; }

		currentFrame += TimeScale::Get();

		float t = std::clamp(currentFrame / blendFrame, 0.0f, 1.0f);
		//イージング
		t = t * t * (3.0f - 2.0f * t);

		Apply(camera, t);
	}

	//自動では終了しない
	bool IsEnd(void) const override { return false; }

private:

	// 開始時カメラ情報
	Vector3 startPos;
	Vector3 startAngle;
	float startFov;

	// 目標のカメラ情報
	const Vector3 endPos;
	Vector3 endAngle;
	const float endFov;

	// 移動フレーム数
	const float blendFrame;
	float currentFrame;

	// 補間した位置・角度・視野角をカメラに反映
	void Apply(CameraBase& camera, float t)
	{
		camera.SetPos(startPos + (endPos - startPos) * t);
		camera.SetAngle(startAngle + (endAngle - startAngle) * t);
		camera.SetFov(startFov + (endFov - startFov) * t);
	}

	// 位置と注視点から角度を割り出す
	static Vector3 CalcAngle(const Vector3& pos, const Vector3& lookAtPos)
	{
		Vector3 vec = lookAtPos - pos;
		if (vec.LengthSq() <= 0.000001f) { return Vector3(); }
		vec.Normalize();
		return Vector3(-asinf(std::clamp(vec.y, -1.0f, 1.0f)), atan2f(vec.x, vec.z), 0.0f);
	}
};
