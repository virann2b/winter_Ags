#include "Sword.h"

#include "../../../Utility/Utility.h"
#include "../../Common/Collider/CapsuleCollider.h"



Sword::Sword(const Vector3*& playerPos, 
	const Transform& folliowTransform)
	:ActorBase(),
	playerPos(playerPos),
	followTransform(folliowTransform)
{
}

void Sword::Load(void)
{

	// モデルをロード
	trans.LoadModel("Sword/Sword");

	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::Sword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 1.5f;


	//描画オフ
	SetIsDraw(true);
	//判定オフ
	SetJudge(false);
}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Sword::Start(void)
{
}

void Sword::SubUpdate(void)
{

	// 手の位置
	Vector3 handOffset =
		followTransform.VTrans(handLocalPos);

	Vector3 handWorldPos =
		followTransform.pos + handOffset;


	
	// Swordの位置
	Vector3 swordOffset =
		followTransform.VTrans(localPos);

	trans.pos =
		handWorldPos + swordOffset;


	// Swordの回転

	trans.angle =
		followTransform.angle + localRot;
	
}