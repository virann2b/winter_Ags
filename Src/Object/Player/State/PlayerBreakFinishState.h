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

		Max 
	};

	PlayerBreakFinishState(
		std::function<void(void)> ReadyStart,	
		std::function<void(void)> PassStart,	
		std::function<void(float)> PassUpdate,	
		std::function<void(void)> FinishSlash,	
		std::function<bool(void)> IsAnimeEnd,
		std::function<void(void)> FinishRelease,	
		std::function<void(void)> ChangeStateIdle
	);

	void Enter(void) override;
	void Update(void) override;
	void Exit(void) override;

private:


#pragma region 受け取る参照変数・関数
	//すり抜けにかけるフレーム数
	static constexpr float PASS_FRAME = 6.0f;

	//構え開始
	std::function<void(void)> ReadyStart;
	//一閃開始
	std::function<void(void)> PassStart;
	//一閃中の移動
	std::function<void(float)> PassUpdate;
	//到着：ダメージ・残心アニメ
	std::function<void(void)> FinishSlash;

	std::function<bool(void)> IsAnimeEnd;

	//後始末
	std::function<void(void)> FinishRelease;
	std::function<void(void)> ChangeStateIdle;

	//ステップ
	STEP step;

	float passCounter;


};

