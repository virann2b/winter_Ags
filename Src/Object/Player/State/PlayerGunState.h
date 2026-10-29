#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class PlayerGunState : public CharacterStateBase
{
public:

	//銃状態
	enum class STATE
	{
		None = -1,

		Target,

		Shot,

		Relod,

		Max,
	};

	PlayerGunState(
		std::function<void(void)> PlayAnimeGun,
		std::function<void(void)> ChangeStateIdle,
		std::function<void(const Vector3&)> MoveAccel,
		float ACCEL_MAX,
		float& accelMax,
		float& targetTime,
		int& bulletCount,
		const std::function<void(void)> PlayShot
	);

	~PlayerGunState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

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

	// 横軸加速度の最大値の参照
	float& accelMax;

#pragma endregion

#pragma region 定数


	// 移動時の最大移動速度
	const float ACCEL_MAX;

	//一度に撃てる最大弾数
	static constexpr int  MAX_BULLET = 6;

	//最大弾所持数
	static constexpr int  MAX_POSSESSION_BULLET = 48;

	//発射数増加時間
	static constexpr float BULLET_COUNT_TIME = 15.0f;

	//発射数増加最大時間
	static constexpr float BULLET_COUNT_MAX_TIME = 90.0f;

#pragma endregion


#pragma region 変数

	//銃状態
	STATE state;

	//敵を捉えた時間
	float& targetTime;
	//発射数
	int& bulletCount;

#pragma endregion

	//状態遷移
	void ChangeState(STATE state);
	
	Vector3 InputVec(void)const;

};

