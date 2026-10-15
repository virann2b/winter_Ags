#include "Gun.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"

#include "../../Player.h"



Gun::Gun(const Transform& playerTrans, bool isActive)
	:
	ActorBase(),
	playerTrans(playerTrans),
	isActive(isActive)
{
}

void Gun::Load(void)
{
	// モデルをロード
	trans.LoadModel("Gun/Gun");


	AddCollider(
		new CapsuleCollider(COLLIDER_TAG::Sword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 0.3f;

	isActive = false;
	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	SetJudgeFlg(false);
}

void Gun::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Gun::SubUpdate(void)
{

	SetIsDraw(isActive);

	// ボーンは名前で検索(名前はモデルに合わせて変更)
	const int handFrame = MV1SearchFrame(playerTrans.model, "Bone_036");
	if (handFrame < 0) { return; }

	MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	trans.pos = Vector3(hand.m[3][0], hand.m[3][1], hand.m[3][2]);
	trans.rotation = Quaternion::FromMatrix(hand);

	//調整角度
	//trans.AddAngleYDeg(50.0f);
	trans.AddAngleXDeg(50.0f);
	trans.AddAngleZDeg(50.0f);

}
