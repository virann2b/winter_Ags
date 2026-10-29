#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

class ChilbiDieState : public CharacterStateBase
{
public:

	ChilbiDieState(
		std::function<void(void)> playAnimeDie,
		std::function<bool(void)> isDead,
		std::function<bool(void)> isAnimeEnd
	);

	~ChilbiDieState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:



#pragma region 受け取る参照変数・関数

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeDie;

	// 死亡判定
	const std::function<bool(void)> isDead;

	//アニメーション終了判定
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion

};

