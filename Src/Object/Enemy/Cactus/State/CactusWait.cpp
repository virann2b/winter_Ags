#include "CactusWait.h"


CactusWait::CactusWait(
	const Vector3& cactusPos,
	const Vector3*& playerPos,
	std::function<void(void)> changeStateAttack,
	std::function<void(void)> playAnimationWait

	)
	: cactusPos(cactusPos),
		playerPos_(playerPos),
	changeStateAttack(changeStateAttack),
	playAnimationWait(playAnimationWait)
{
}
void CactusWait::Enter()
{
	//アニメーションループ再生
	playAnimationWait();
}
void CactusWait::Update(void)
{

	//プレイヤーとのベクトルを取得
	Vector3 vec = *playerPos_ - cactusPos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//距離が近い場合ステートを切り替え
	if (vec.Length() < 100.0f)
	{
		changeStateAttack();
	}
}
