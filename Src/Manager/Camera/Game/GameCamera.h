#pragma once

#include "../CameraBase.h"
#include "../../Input/InputManager.h"

	/// <summary>
	/// 3モードを切り替えるカメラ
	///   Remote : ターゲットOFF時のカメラ（自由回転）
	///   Auto   : ターゲットON時のカメラ（自機と対象を画角に収める）
	///   Gun    : 銃モードのカメラ（肩越し。Autoからのみ入れる）
	/// </summary>
	class GameCamera : public CameraBase
{
public:

	enum class Mode
	{
		Auto,
		Remote,
		Gun
	};

	/// <param name="targetPos">追従対象（自機）</param>
	/// <param name="focusPos">ロックオン対象（Auto/Gunで使用）</param>
	/// <param name="cameraOffset">Remoteのカメラオフセット</param>
	/// <param name="lookAtOffset">Remoteの注視点オフセット</param>
	/// <param name="ROT_POWER">Remoteの回転力（0で回転無効）</param>
	/// <param name="initAngle">Remoteの初期角度</param>
	/// <param name="TARGET_DISTANCE_MIN">Autoの最低距離</param>
	/// <param name="TARGET_DISTANCE_MAX">Autoの最大距離</param>
	/// <param name="fov">通常時の視野角</param>
	/// <param name="initialModeRemote">true:Remoteで開始 / false:Autoで開始</param>
	/// <param name="toggleKey">ターゲットON/OFFキー</param>
	/// <param name="gunKey">銃モードON/OFFキー</param>
	GameCamera(
		const Vector3* targetPos,
		const Vector3* focusPos,
		const Vector3& cameraOffset = Vector3::Zonly(-400),
		const Vector3& lookAtOffset = Vector3(),
		float ROT_POWER = 3.0f * (DX_PI_F / 180.0f),
		const Vector3& initAngle = Vector3(),
		float TARGET_DISTANCE_MIN = 350.0f, float TARGET_DISTANCE_MAX = 400.0f,
		float fov = 80.0f * (DX_PI_F / 180.0f),
		bool initialModeRemote = false,
		KEY_TYPE toggleKey = KEY_TYPE::PlayerLockOn,   // ★実際のキー名に置き換えてください
		KEY_TYPE gunKey = KEY_TYPE::PlayerGunMode        // ★実際のキー名に置き換えてください
	);

	~GameCamera()override = default;

	// 追従対象を途中で変更する
	void TargetChange(const Vector3* targetPos) {
		if (targetPos == nullptr) { return; }
		this->targetPos = targetPos;
	}

	// ロックオン対象を途中で変更する（nullptrを許可：対象消失時にRemoteへ戻すため）
	void FocusChange(const Vector3* focusPos) { this->focusPos = focusPos; }

	// モード切替（Gunは Auto 中かつ focusPos がある時のみ成功する）
	// 成功したら true
	bool SetMode(Mode m);
	// ターゲットON/OFF（Remote <-> Auto。Gun中はRemoteへ）
	void ToggleTarget(void);
	// 銃モードON/OFF（Auto <-> Gun。Remoteでは何もしない）
	void ToggleGun(void);

	Mode GetMode(void) const { return mode; }
	bool IsGunMode(void) const { return mode == Mode::Gun; }

private:

	// カメラ最低地上高
	static constexpr float CAMERA_DOWN = 85.0f;
	// 注視点最低地上高
	static constexpr float FOCUS_DOWN = 70.0f;

	// Gun用パラメータ
	static constexpr float GUN_BACK_DIST = 220.0f;                    // 自機の後ろ
	static constexpr float GUN_SHOULDER = 60.0f;                      // 右肩へのずらし
	static constexpr float GUN_HEIGHT = 140.0f;                       // 高さ
	static constexpr float GUN_FOV = 55.0f * (DX_PI_F / 180.0f);      // 照準時の視野角
	static constexpr float FOV_LERP_RATE = 0.15f;                     // 視野角の補間率

	// 追従対象
	const Vector3* targetPos;
	// ロックオン対象
	const Vector3* focusPos;

	// Remote 用オフセット
	const Vector3 cameraOffset;
	const Vector3 lookAtOffset;
	// 注視点
	Vector3 lookAtPos;

	// Auto 用距離制約
	float TARGET_DISTANCE_MIN;
	float TARGET_DISTANCE_MAX;

	// Remote 用
	const float ROT_POWER;
	Vector3 controlAngle;

	// 切替キー
	KEY_TYPE toggleKey;
	KEY_TYPE gunKey;

	// 現在のモード
	Mode mode;

	// 通常時の視野角（Gun終了時に戻す先）
	float baseFov;

	// 更新
	void NormalUpdate(void)override;
	void UpdateRemote(void);
	void UpdateAuto(void);
	void UpdateGun(void);

	// Remoteの位置を controlAngle から即座に合わせる
	void SnapRemote(void);

	// 終了処理
	void SubRelease(void)override;
};
