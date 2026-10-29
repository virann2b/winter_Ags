#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
class ChilbiIdleState : public CharacterStateBase
{
public:

	ChilbiIdleState(
		std::function<void(void)> playAnimeIdle,
		const std::function<void(void)> changeAttackState
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
	const std::function<void(void)> changeAttackState;

#pragma endregion

};

