#include "ThornWait.h"

ThornWait::ThornWait(const Vector3& thornPos, 
	const Vector3*& playerPos, 
	std::function<void(void)> changeStateAttack, 
	std::function<void(void)> playAnimationWait)
	: thornPos(thornPos),
	playerPos(playerPos),
	changeStateAttack(changeStateAttack),
	playAnimationWait(playAnimationWait)
{
}

void ThornWait::Enter()
{
	//アニメーションループ再生
	playAnimationWait();
}

void ThornWait::Update()
{

	//プレイヤーとのベクトルを取得
	Vector3 vec = *playerPos - thornPos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//距離が近い場合ステートを切り替え
	if (vec.Length() < 500.0f)
	{
		changeStateAttack();
	}
}
