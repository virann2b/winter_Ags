#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"
#include "../../Common/Transform/Transform.h"
struct Vector3;

class PlayerRollState : public CharacterStateBase
{
public:

	PlayerRollState(
		std::function<void(void)> PlayAnimeRoll,
		std::function<bool(void)> IsAnimeEnd,
		std::function<void(const Vector3&)> MoveAccel,
		float& AccelMax,
		const Transform& PlayerTrans,
		std::function<void(void)> ChangeStateIdle,
		std::function<void(void)> PlayerReset
	);

	~PlayerRollState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	

#pragma region 受け取る参照変数・関数

	// 回避アニメーションの再生関数ポインタ
	const std::function<void(void)> PlayAnimeRoll;
	// 待機状態に遷移する関数ポインタ
	const std::function<void(void)> ChangeStateIdle;

	// アニメーション終了取得関数ポインタ
	const std::function<bool(void)> IsAnimeEnd;
	// 移動関数ポインタ
	const std::function<void(const Vector3&)> MoveAccel;

	// プレイヤーの設定を元に戻す関数ポインタ
	const std::function<void(void)> PlayerReset;

	// プレイヤーのモデル制御情報の参照
	const Transform& PlayerTrans;

	//最大加速度
	float& AccelMax;

#pragma endregion

#pragma region 定数

	//回避で移動する距離
	static constexpr float ROLL_DISTANCE = 600.0f;
	// 転がる時間(秒)
	static constexpr float ROLL_TIME = 0.6f;    

	static constexpr float ARRIVE_RANGE = 10.0f;

	//回避最大速度
	static constexpr float ROLL_ACCEL_MAX = 15.0f;

#pragma endregion

	
#pragma region 変数

	//回避方向(始めの入力で確定させる)
	Vector3 rollDir;

	Vector3 rollMove;
	Vector3 rollTargetPos;

	//回避開始位置
	Vector3 startPos;

	// ここまでに転がった距離
	float rollMoved = 0.0f;  

	// 回避前の加速度保存用
	float prevAccelMax = 0.0f;

#pragma endregion

	// 移動方向入力取得
	Vector3 InputVec(void)const;
};