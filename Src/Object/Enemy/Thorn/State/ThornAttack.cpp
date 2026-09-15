#include "ThornAttack.h"
#include "../Wepon/Icicle.h"



ThornAttack::ThornAttack(
	const Vector3& thornPos, 
	const Vector3*& playerPos,
	std::function<void(void)> createIcicle,
	std::function<void(void)> changeStateWait,
	float& cactusAngle_Y, Icicle& icicle)
	:
	thornPos(thornPos),
	playerPos(playerPos),
	createIcicle(createIcicle),
	changeStateWait(changeStateWait),
	cactusAngle_Y(cactusAngle_Y),
	icicle_(icicle)
{
}


void ThornAttack::Update(void)
{
	//プレイヤーとのベクトルを取得
	Vector3 vec = *playerPos - thornPos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//距離が遠くなったら場合ステートを切り替え
	if (vec.Length() > 300.0f)
	{
		changeStateWait();
	}

	//目標角度
	float goalAngle_y = atan2f(vec.x, vec.z);

	// 現在角度から目標角度までの角度差
	float diffAngle = goalAngle_y - cactusAngle_Y;

	// -π ～ +π に正規化して最短方向を求める
	while (diffAngle > DX_PI_F) { diffAngle -= DX_TWO_PI_F; }
	while (diffAngle < -DX_PI_F) { diffAngle += DX_TWO_PI_F; }

	// 最短方向に補間
	cactusAngle_Y += diffAngle * 0.15f;



	//アイシクルの状態がNoneになった時に新たに生成
	if (icicle_.Getstate() == Icicle::STATE::None)
	{
		createIcicle();
		icicle_.Start();
	}
	
}


