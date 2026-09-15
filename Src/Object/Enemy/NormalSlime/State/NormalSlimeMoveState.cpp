#include "NormalSlimeMoveState.h"


NormalSlimeMoveState::NormalSlimeMoveState(
	const Vector3& normalSlimePos,
	std::function<void(const Vector3& vec)> normalSlimeMoveAccel
):
	normalSlimePos(normalSlimePos),
	normalSlimeMoveAccel(normalSlimeMoveAccel)
{
}

void NormalSlimeMoveState::OwnStateConditionUpdate(void)
{
}

void NormalSlimeMoveState::Update(void)
{
	//巡回目標座標までのベクトルを取得
	Vector3 vec
		= MOVE_POINT_LIST[nowMovePoint] - normalSlimePos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//巡回目標座標までの距離が近い場合
	if (vec.Length() < 10.0f)
	{
		//巡回番号を更新
		nowMovePoint++;
		if (nowMovePoint >= MOVE_POINT_LIST.size())
		{ nowMovePoint = 0; }
	}
	else
	{
		//巡回目標座標までのベクトルを正規化して加速移動する
		vec.Normalize();
		normalSlimeMoveAccel(vec);
	}

}
