#include "Sword.h"

#include "../../../Utility/Utility.h"
#include "../../Common/Collider/CapsuleCollider.h"


Sword::Sword(
	const Transform& playerTrans)
	:ActorBase(),
	playerTrans(playerTrans)
{
}

void Sword::Load(void)
{
	// モデルをロード
	trans.LoadModel("Sword/Sword");


	AddCollider(
		new CapsuleCollider(COLLIDER_TAG::Sword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 0.7f;

	//描画ON
	SetIsDraw(true);
	//判定オフ
	SetJudgeFlg(false);
}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Sword::SubUpdate(void)
{
	trans.pos = playerTrans.pos;
}
