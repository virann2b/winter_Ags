#pragma once

#include <functional>

#include "../CameraBase.h"

#include "../../Input/InputManager.h"

class GameCamera : public CameraBase
{
public:

	enum class Mode
	{
		LockOn,		//フリーモード
		Remote,		//追従モード
		Gun			//銃モード
	};

	/// <param name="targetPos">追従対象（自機）</param>
	/// <param name="focusPos">ロックオン対象（追従モード・カメラモードで使用）</param>
	/// <param name="cameraOffset">フリーモードのカメラオフセット</param>
	/// <param name="lookAtOffset">フリーモードの注視点オフセット</param>
	/// <param name="ROT_POWER">フリーモードの回転力（0で回転無効）</param>
	/// <param name="initAngle">フリーモードの初期角度</param>
	/// <param name="TARGET_DISTANCE_MIN">追従モードの最低距離</param>
	/// <param name="TARGET_DISTANCE_MAX">追従モードの最大距離</param>
	/// <param name="fov">通常時の視野角</param>
	/// <param name="initialModeRemote">true:フリーモードで開始 / false:追従モードで開始</param>
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

	// ロックオン対象を途中で変更する（nullptrを許可：対象消失時にフリーモードへ戻すため）
	void FocusChange(const Vector3* focusPos) { this->focusPos = focusPos; }

	// モード切替（カメラモードは 追従モード 中かつ focusPos がある時のみ成功する）
	// 成功したら true
	bool SetMode(Mode m);
	// ターゲットON/OFF（フリーモード <-> 追従モード。カメラモード中はフリーモードへ）
	void ChangeTargetMode(void);
	// 銃モードON/OFF（追従モード <-> カメラモード。フリーモードでは何もしない）
	void ChangeGunMode(void);

	// 構えているかを設定（モードの切り替えはカメラが判断する。毎フレーム呼んでよい）
	//   構えている かつ 追従モード → カメラモード
	//   構えていない かつ カメラモード → 追従モード
	//   フリーモード（ロックオン解除中）は何もしない
	void SetAiming(bool aiming);

	Mode GetMode(void) const { return mode; }
	bool IsGunMode(void) const { return mode == Mode::Gun; }

	// 近くの敵の位置を返す
	using FocusProvider = std::function<bool(Vector3& outPos)>;

	void SetFocusProvider(FocusProvider provider);


private:

#pragma region 定数

	// カメラ最低地上高
	static constexpr float CAMERA_DOWN = 85.0f;
	// 注視点最低地上高
	static constexpr float FOCUS_DOWN = 70.0f;

	// カメラモード用パラメータ
	static constexpr float GUN_BACK_DIST = 140.0f;                    // 自機の後ろ
	static constexpr float GUN_SHOULDER = 100.0f;                      // 右肩へのずらし
	static constexpr float GUN_HEIGHT = 110.0f;                       // 高さ
	static constexpr float GUN_FOV = 55.0f * (DX_PI_F / 180.0f);      // 照準時の視野角
	static constexpr float FOV_LERP_RATE = 0.15f;                     // 視野角の補間率

#pragma endregion
	

#pragma region 変数

	// 追従対象
	const Vector3* targetPos;
	// ロックオン対象
	const Vector3* focusPos;

	// フリーモード 用オフセット
	const Vector3 cameraOffset;
	const Vector3 lookAtOffset;
	// 注視点
	Vector3 lookAtPos;

	// 追従モード 用距離制約
	float TARGET_DISTANCE_MIN;
	float TARGET_DISTANCE_MAX;

	// フリーモード 用
	const float ROT_POWER;
	Vector3 controlAngle;

	// 切替キー
	KEY_TYPE toggleKey;
	KEY_TYPE gunKey;

	// 現在のモード
	Mode mode;

	// 通常時の視野角（カメラモードから切り替わる時に戻す先）
	float baseFov;

	Vector3 gunOffset;

	Vector3 focusPosValue;       
	FocusProvider focusProvider; 

#pragma endregion


	// 更新
	void NormalUpdate(void)override;
	void UpdateRemote(void);
	void UpdateLockOn(void);
	void UpdateGun(void);

	// フリーモードの位置を controlAngle から即座に合わせる
	void SnapRemote(void);

	// 終了処理
	void SubRelease(void)override;

	void UpdateFocus(void);     
};
