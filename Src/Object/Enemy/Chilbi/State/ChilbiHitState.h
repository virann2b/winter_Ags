#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

class ChilbiHitState : public CharacterStateBase
{
public:

	ChilbiHitState(
		std::function<void(void)> playAnimeHit,
		std::function<bool(void)> ownState,
		std::function<void(void)> ChangeStateInit,
		std::function<bool(void)> isAnimeEnd
		);

	~ChilbiHitState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:


#pragma region 受け取る参照変数・関数

	// HITアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeHit;

	// 待機状態に遷移する関数ポインタ
	const std::function<void(void)> ChangeStateInit;
	
	// 自身のステートに遷移する条件関数ポインタ
	const std::function<bool(void)> ownState;

	// アニメーション終了取得関数ポインタ
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion
};

