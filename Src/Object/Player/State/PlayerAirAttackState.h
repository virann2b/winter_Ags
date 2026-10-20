#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

class PlayerAirAttackState : public CharacterStateBase
{
public:

	enum class STEP
	{
		None = -1,

		Start,
		Air,
		End,

		Max
	};

	PlayerAirAttackState(
		const bool& isGround,
		std::function<void(void)> playAnimAirAttackStart,
		std::function<void(void)> playAnimAirAttackEnd,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> ChangeStateIdle);

	~PlayerAirAttackState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

private:

	// 空中攻撃開始アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimAirAttackStart;
	// 空中攻撃終了アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimAirAttackEnd;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;
	//アニメーション終了関数ポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 地面と設置しているか
	const bool& isGround;


	STEP step;

	bool isChangeAnim;
};

