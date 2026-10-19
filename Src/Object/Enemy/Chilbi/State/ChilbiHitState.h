#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

class ChilbiHitState : public CharacterStateBase
{
public:

	ChilbiHitState(
		std::function<void(void)> playAnimeHit,
		std::function<void(void)> ChangeStateInit,
		std::function<bool(void)> isAnimeEnd
		);

	~ChilbiHitState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	//更新
	void Update(void)override;

private:

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHit;

	// 待機状態に遷移
	const std::function<void(void)> ChangeStateInit;

	//アニメーション終了判定
	const std::function<bool(void)> isAnimeEnd;
};

