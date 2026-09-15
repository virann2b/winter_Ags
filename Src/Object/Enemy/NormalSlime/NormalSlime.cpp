#include "NormalSlime.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/NormalSlimeMoveState.h"

void NormalSlime::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/NormalSlime/NormalSlime");

	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, 50.0f));
	// モデルの角度のズレを設定
	//trans.localAngle.y = Deg2Rad(90.0f);
	//サイズ設定
	trans.scale = 1.3f;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	// 移動状態を追加
	AddState(
		STATE::Move,
		new NormalSlimeMoveState(
			trans.pos,
			std::bind(&NormalSlime::MoveAccel, this, std::placeholders::_1)
			));

#pragma endregion
}

void NormalSlime::CharacterInit(void)
{
	ChangeState(STATE::Move);

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 3.0f;
	// 加速最大値を設定
	ACCEL_MAX = 7.0f;
}

void NormalSlime::CharacterUpdate(void)
{
}

void NormalSlime::CharacterDraw(void)
{
}

void NormalSlime::CharacterAlphaDraw(void)
{
}

void NormalSlime::CharacterUiDraw(void)
{
}

void NormalSlime::CharacterRelease(void)
{
}
