#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

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
		std::function<void(void)> playAnimeGun,
		std::function<void(void)> ChangeStateIdle,
		float& targetTime,
		int& bulletCount,
		const std::function<void(void)> playShot
	);

	~PlayerGunState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

private:

	// 銃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeGun;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

	// 発射処理関数のポインタ
	const std::function<void(void)> playShot;

#pragma region 定数

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

	bool gunState;

#pragma endregion

	//状態遷移
	void ChangeState(STATE state);



};

