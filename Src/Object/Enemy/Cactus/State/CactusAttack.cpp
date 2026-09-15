#include "CactusAttack.h"


CactusAttack::CactusAttack
(
	const Vector3& cactusPos, 
	const Vector3*& playerPos, 
	std::function<void(void)> changeStateAttack,
	std::function<void(void)> playAnimationWait,
	float& cactusAngle_Y
)
	: cactusPos(cactusPos),
	playerPos(playerPos),
	changeStateWait(changeStateAttack),
	playAnimationAttack(playAnimationWait),
	cactusAngle_Y(cactusAngle_Y)
{
}
void CactusAttack::Update(void)
{
	//プレイヤーとのベクトルを取得
	Vector3 vec = *playerPos - cactusPos;
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
	
	
	//クールタイムがない場合
	if (!isCoolTime)
	{
		isCoolTime = true;
		attackCoolTime = 0;
	}
	else
	{
		//カウント増加
		attackCoolTime++;

		if (attackCoolTime >= ATTACK_COOLTIME_MAX)
		{

			//攻撃アニメーションの再生
			playAnimationAttack();
			//クールタイムリセット
			isCoolTime = false;
		}
	}

}
