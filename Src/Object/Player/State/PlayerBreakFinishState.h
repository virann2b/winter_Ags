#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

class PlayerBreakFinishState : public CharacterStateBase
{
public:

	enum class STEP 
	{ 
		None = -1,

		Ready, 
		Pass, 
		After,
		Return,

		Max 
	};

	PlayerBreakFinishState(
		std::function<void(void)> ReadyStart,	
		std::function<void(void)> PassStart,	
		std::function<void(float)> PassUpdate,	
		std::function<void(void)> FinishSlash,	
		std::function<bool(void)> IsAnimeEnd,
		std::function<void(void)> ReturnStart,
		std::function<bool(void)> IsReturnEnd,
		std::function<void(void)> FinishRelease,	
		std::function<void(void)> ChangeStateIdle
	);

	// 状態遷移後1度行う初期化処理
	void Enter(void) override;
	// 更新処理
	void Update(void) override;
	// 状態遷移前1度行う終了処理
	void Exit(void) override;

private:


#pragma region 受け取る参照変数・関数
	//すり抜けにかけるフレーム数
	static constexpr float PASS_FRAME = 6.0f;

	// 構え開始関数ポインタ
	std::function<void(void)> ReadyStart;
	// 一閃開始関数ポインタ
	std::function<void(void)> PassStart;
	// 一閃中の移動関数ポインタ
	std::function<void(float)> PassUpdate;
	// 到着時のダメージ・残心アニメーション関数ポインタ
	std::function<void(void)> FinishSlash;

	// アニメーション終了取得関数ポインタ
	std::function<bool(void)> IsAnimeEnd;

	// カメラを元に戻し始める関数ポインタ
	std::function<void(void)> ReturnStart;
	// カメラが元の位置に戻ったかを返す関数ポインタ
	std::function<bool(void)> IsReturnEnd;

	// 一閃の後始末関数ポインタ
	std::function<void(void)> FinishRelease;
	// 待機状態に遷移する関数ポインタ
	std::function<void(void)> ChangeStateIdle;

	//ステップ
	STEP step;

	// すり抜け開始からの経過時間
	float passCounter;


};

