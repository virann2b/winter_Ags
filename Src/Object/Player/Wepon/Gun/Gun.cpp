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
	const int handFrame = MV1SearchFrame(playerTrans.model, "smartrig:RightHand");
	if (handFrame < 0) { return; }

	MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	// ========================================
	// 回転行列を取得
	// ========================================

	const VECTOR handScale = MGetSize(hand);

	MATRIX handRotationMat = MGetRotElem(hand);

	// スケールを除去
	handRotationMat =
		MMult(
			handRotationMat,
			MGetScale(VGet(1.0f / handScale.x, 1.0f / handScale.y, 1.0f / handScale.z)));


	// ========================================
	// 座標
	// ========================================

	const Vector3 handPos =
		Vector3(MGetTranslateElem(hand));

	trans.pos = handPos;


	// ========================================
	// 回転
	// ========================================

	trans.SetRotation(
		Quaternion::FromMatrix(
			handRotationMat
		)
	);
}
