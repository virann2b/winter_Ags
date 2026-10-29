#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"


class PlayerHitState : public CharacterStateBase
{
public:

	PlayerHitState(
		std::function<void(void)> PlayAnimeHit,
		std::function<bool(void)> IsAnimeEnd,
		std::function<bool(void)> OwnState,
		std::function<void(void)> ChangeStateIdle
		);

	~PlayerHitState()override = default;

	//自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void) override;
	//更新
	void Update(void) override;


private:

#pragma region 受け取る参照変数・関数

	//HITアニメーション再生
	const std::function<void(void)> PlayAnimeHit;
	//待機状態遷移
	const std::function<void(void)> ChangeStateIdle;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	//自身のステートに遷移する条件関数ポインタ
	const std::function<bool(void)> OwnState;

#pragma endregion

	

};