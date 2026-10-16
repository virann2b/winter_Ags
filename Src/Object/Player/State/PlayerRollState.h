#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"
#include "../../Common/Transform/Transform.h"
struct Vector3;

class PlayerRollState : public CharacterStateBase
{
public:

	PlayerRollState(
		std::function<void(void)> ChangeStateIdle,
		std::function<void(void)> playAnimeRoll,
		std::function<bool(void)> isAnimeEnd,
		const Transform& playerTrans
	);

	~PlayerRollState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	void Exit(void)override;
	// 更新処理
	void Update(void)override;

private:


#pragma region 受け取る参照変数・関数

	// 回避アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeRoll;
	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

#pragma endregion




};