#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

class ChilbiHitState : public CharacterStateBase
{
public:

	ChilbiHitState(std::function<void(void)> playAnimeHit);

	~ChilbiHitState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHit;
};

