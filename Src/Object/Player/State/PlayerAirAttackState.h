#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"

class PlayerAirAttackState : public CharacterStateBase
{
public:

	PlayerAirAttackState(
		std::function<void(void)> playAnimAirAttack,
		std::function<void(void)> ChangeStateIdle);

	~PlayerAirAttackState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

private:

	// 空中攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimAirAttack;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

};

