#include "Sword.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"


Sword::Sword(const Transform& playerTrans, bool isActive)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans),
	isActive(isActive)
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

	//角度の調整
	trans.SetLocalRotation(Quaternion::FromRotationZ(Deg2Rad(-180.0f)));

	//サイズ設定
	trans.scale = 0.7f;

	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	SetJudgeFlg(isActive);

	// その回のパラメーター名
	std::string parameterName = "JustPos";
	// そのパラメータが存在するかどうか
	if (!IsParameterExist("SwordPos", parameterName)) { return; }
	justPos =  GetParameterToVector3("SwordPos", parameterName);

	SetJudgeFlg(false);

}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Sword::SubUpdate(void)
{	
	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	//SetJudgeFlg(isActive);

	// ボーンは名前で検索(名前はモデルに合わせて変更)
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

	// OFFSETは「回転だけ」で変換する
	const Vector3 offset =
		Vector3(
			VTransform(
				justPos.ToVECTOR(),
				handRotationMat
			)
		);


	trans.pos = handPos + offset;
	

	// ========================================
	// 回転
	// ========================================

	trans.SetRotation(
		Quaternion::FromMatrix(
			handRotationMat
		)
	);
}

void Sword::SubDraw(void)
{
	// 右手のボーンを取得
	int frameIndex = MV1SearchFrame(
		playerTrans.model,
		"mixamorig:RightHand"
	);

	if (frameIndex == -1)
	{
		return;
	}

	// 右手のワールド行列
	MATRIX handMatrix =
		MV1GetFrameLocalWorldMatrix(
			playerTrans.model,
			frameIndex
		);

	// 右手の位置
	VECTOR handPos = VGet(
		handMatrix.m[3][0],
		handMatrix.m[3][1],
		handMatrix.m[3][2]
	);

	// XYZの回転軸を取得
	VECTOR xAxis = VGet(
		handMatrix.m[0][0],
		handMatrix.m[0][1],
		handMatrix.m[0][2]
	);

	VECTOR yAxis = VGet(
		handMatrix.m[1][0],
		handMatrix.m[1][1],
		handMatrix.m[1][2]
	);

	VECTOR zAxis = VGet(
		handMatrix.m[2][0],
		handMatrix.m[2][1],
		handMatrix.m[2][2]
	);

	// 軸の長さ
	float axisLength = 30.0f;

	// X軸：赤
	DrawLine3D(
		handPos,
		VAdd(handPos, VScale(xAxis, axisLength)),
		GetColor(255, 0, 0)
	);

	// Y軸：緑
	DrawLine3D(
		handPos,
		VAdd(handPos, VScale(yAxis, axisLength)),
		GetColor(0, 255, 0)
	);

	// Z軸：青
	DrawLine3D(
		handPos,
		VAdd(handPos, VScale(zAxis, axisLength)),
		GetColor(0, 0, 255)
	);
}