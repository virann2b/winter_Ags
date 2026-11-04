#include "Punch.h"

#include "../../../../Utility/Utility.h"

#include "../../../Common/Collider/SphereCollider.h"

Punch::Punch(const Transform& chilbiTrans)
	:
	chilbiTrans(chilbiTrans)
{
}

void Punch::Load(void)
{
#pragma region 初期設定

	//移動判定ON
	SetDynamicFlg(true);
	//重力OFF
	SetGravityFlg(false);
	//コライダー同士の押し出しOFF
	SetPushFlg(false);

	//当たり判定OFF
	SetJudgeFlg(false);


#pragma endregion


	// 生成したコライダーをアクター所有コライダーに追加
	AddCollider(new SphereCollider(COLLIDER_TAG::Enemy_ChilbiPunch, 60, trans.pos));

}

void Punch::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:
	{
		TakeDamage(1);
		JudgeEnd();
	}
	}
}

void Punch::JudgeStart(void)
{
	SetJudgeFlg(true);
}

void Punch::JudgeEnd(void)
{
	SetJudgeFlg(false);
}


void Punch::SubUpdate(void)
{
	//正面の方向に位置を調整
	const Vector3 offset = chilbiTrans.rotation.Rotate(Vector3(0.0f, PUNCH_OFFSET_POS_Y, PUNCH_OFFSET_POS_Z));

	trans.pos = chilbiTrans.pos + offset;

}