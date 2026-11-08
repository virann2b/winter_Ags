#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class PlayerGunState : public CharacterStateBase
{
public:


	PlayerGunState(
		std::function<void(void)> PlayAnimeGun,
		std::function<void(void)> ChangeStateIdle,
		std::function<void(const Vector3&)> MoveAccel,
		std::function<void(void)> PlayShot,
		float ACCEL_MAX,
		float& accelMax,
		std::function<bool(void)> IsLockOn
	);

	~PlayerGunState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

	//後始末
	void Exit(void)override;

private:

#pragma region 受け取る参照変数・関数

	// 銃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAnimeGun;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

	// 発射処理関数のポインタ
	const std::function<void(void)> PlayShot;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> MoveAccel;

	// ロックオンしているか（銃に入れる条件・構えを続ける条件）
	const std::function<bool(void)> IsLockOn;

	// 横軸加速度の最大値の参照
	float& accelMax;

#pragma endregion

#pragma region 定数

	// 移動時の最大移動速度
	const float ACCEL_MAX;

#pragma endregion

	//入力方向
	Vector3 InputVec(void)const;

};

