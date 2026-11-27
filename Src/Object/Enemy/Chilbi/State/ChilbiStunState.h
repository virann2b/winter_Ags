#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"


class ChilbiStunState : public CharacterStateBase
{
public:

	ChilbiStunState(
		std::function<void(void)> playAnimeHit,
		std::function<float(void)> getStunFrame,
		std::function<float(void)> getStunRemain,
		std::function<void(float)> countStun,
		std::function<float(void)> getAnimeRatio,
		std::function<float(void)> getAnimeStep,
		std::function<void(float)> setAnimeStep,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> changeStateIdle,
		std::function<void(void)> endStun
	);

	~ChilbiStunState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// アニメーションを止める再生比率（0.0f～1.0f）
	static constexpr float STOP_RATIO = 0.4f;

#pragma region 受け取る参照変数・関数

	// HITアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeHit;
	// 硬直させる時間（フレーム）の取得関数ポインタ
	const std::function<float(void)> getStunFrame;
	// 弱点硬直時間の残り割合取得関数ポインタ
	const std::function<float(void)> getStunRemain;
	// 硬直時間のカウント関数ポインタ
	const std::function<void(float)> countStun;
	// アニメーション再生比率取得関ポインタ
	const std::function<float(void)> getAnimeRatio;
	// アニメーションの再生位置取得関数ポインタ
	const std::function<float(void)> getAnimeStep;
	// アニメーションの再生位置設定関数ポインタ
	const std::function<void(float)> setAnimeStep;
	// アニメーション終了取得関数ポインタ
	const std::function<bool(void)> isAnimeEnd;
	// 待機状態に遷移する関数ポインタ
	const std::function<void(void)> changeStateIdle;
	// 硬直の終了関数ポインタ
	const std::function<void(void)> endStun;

#pragma endregion

	// アニメーションを止めているか
	bool isStop = false;
	// 止めている再生位置
	float stopStep = 0.0f;
};
