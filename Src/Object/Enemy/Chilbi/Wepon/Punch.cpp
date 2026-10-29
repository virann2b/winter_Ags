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
	AddCollider(new SphereCollider(COLLIDER_TAG::Enemy_ChilbiPunch, 50, trans.pos));

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
	trans.pos.x = chilbiTrans.pos.x;
	trans.pos.y = chilbiTrans.pos.y;
	trans.pos.z = chilbiTrans.pos.z + 50;

}