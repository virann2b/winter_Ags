#include "Sword.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"


Sword::Sword(
	const Transform& playerTrans)
	:ActorBase("Data/Parameter/Wepon/"),
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

	// その回のパラメーター名
	std::string parameterName = "JustPos";
	// そのパラメータが存在するかどうか
	if (!IsParameterExist("SwordPos", parameterName)) { return; }
	justPos =  GetParameterToVector3("SwordPos", parameterName);

}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Sword::SubUpdate(void)

{	// ボーンは名前で検索(名前はモデルに合わせて変更)
	const int handFrame = MV1SearchFrame(playerTrans.model, "smartrig:RightHand");
	if (handFrame < 0) { return; }

	MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	// ========================================
	// 回転行列を取得
	// ========================================

	const VECTOR handScale =MGetSize(hand);

	MATRIX handRotationMat =MGetRotElem(hand);

	// スケールを除去
	handRotationMat =
		MMult(
			handRotationMat,
			MGetScale(VGet(1.0f / handScale.x,1.0f / handScale.y,1.0f / handScale.z)));


	// ========================================
	// 座標
	// ========================================

	const Vector3 handPos =
		Vector3(MGetTranslateElem(hand));

	trans.pos = trans.pos + justPos;

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
