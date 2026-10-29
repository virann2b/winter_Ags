#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

class PlayerIdleState : public CharacterStateBase
{
public:

	PlayerIdleState(
		std::function<void(void)> PlayAnimeIdle,
		const std::function<void(void)> ChangeSwordActive
	);

	~PlayerIdleState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:



#pragma region 受け取る参照変数・関数

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAnimeIdle;
	// 剣の判定変更関数ポインタ
	const std::function<void(void)> ChangeSwordActive;

#pragma endregion

};