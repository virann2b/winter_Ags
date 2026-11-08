#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

class ChilbiIdleState : public CharacterStateBase
{
public:

	ChilbiIdleState(
		std::function<void(void)> playAnimeIdle,
		std::function<void(void)> changeMoveState,
		std::function<void(void)> changeAttackState,
		std::function<bool(void)> isDistanceNear,
		float attackCoolTime
	);

	~ChilbiIdleState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	//更新
	void Update(void) override;

private:



#pragma region 受け取る参照変数・関数

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeIdle;
	//移動状態に遷移する関数ポインタ
	const std::function<void(void)> changeMoveState;
	//攻撃状態に遷移する関数ポインタ
	const std::function<void(void)> changeAttackState;
	//攻撃対象との距離判定ポインタ
	const std::function<bool(void)> isDistanceNear;
	//攻撃クールタイム
	const float attackCoolTime;

#pragma endregion

};

