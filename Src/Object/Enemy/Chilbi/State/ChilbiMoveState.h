#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class ChilbiMoveState : public CharacterStateBase
{
public:

	ChilbiMoveState(
		std::function<void(void)> playAnimeMove,
		std::function<void(void)> changeAttackState,
		std::function<void(const Vector3&)> moveAccel,
		std::function<Vector3(void)> getTargetPos,
		std::function<bool(void)> isDistanceNear);

	~ChilbiMoveState() override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void) override;


private:

#pragma region 受け取る参照変数・関数

	// 移動アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeMove;
	// 攻撃状態に遷移する関数ポインタ
	const std::function<void(void)> changeAttackState;
	// 攻撃対象との距離判定関数ポインタ
	const std::function<bool(void)> isDistanceNear;

	// 移動関数ポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 攻撃対象の座標取得関数ポインタ
	const std::function <Vector3(void)> getTargetPos;

#pragma endregion
};

