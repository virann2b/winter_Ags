#include "Sword.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"


Sword::Sword(const Transform& playerTrans, bool isActive)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans),
	isActive(isActive),
	swordPosition(SWORD_POSITION::None)
{
}

void Sword::Load(void)
{
#pragma region オブジェクト設定

	SetDynamicFlg(true);
	//衝突設定
	SetGravityFlg(false);
	//当たり判定による押し出し
	SetPushFlg(false);

	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	SetJudgeFlg(isJudge);

#pragma endregion

	// モデルをロード
	trans.LoadModel("Sword/Sword");

	//サイズ設定
	trans.scale = 0.7f;

	trans.centerDiff = Vector3::Xonly(-50.0f);

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::PlayerSword,
			Vector3::Xonly(15.0f) * trans.scale,
			Vector3::Xonly(160.0f) * trans.scale,
			60.0f * trans.scale.MaxElementF()
		)
	);

	//角度の調整
	trans.SetLocalRotation(Quaternion::FromRotationZ(Deg2Rad(-180.0f)));

	swordPosition = SWORD_POSITION::Hand;
}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Enemy:
	{
		TakeDamage(1);
	}
	}
}

void Sword::ChangePosition(SWORD_POSITION nextPostion)
{
	swordPosition = nextPostion;
}

void Sword::ChangeActive(ACTIVE active_)
{
	active = active_;
}

void Sword::SubUpdate(void)
{	
	
	ActiveUpdate();

	SwordUpdate();
	
}

void Sword::SubDraw(void)
{
	// 右手のボーンを取得
	handIndex = MV1SearchFrame(
		playerTrans.model,
		"mixamorig:RightHand"
	);

	if (handIndex == -1)
	{
		return;
	}

	// 右手のワールド行列
	MATRIX handMatrix =
		MV1GetFrameLocalWorldMatrix(
			playerTrans.model,
			handIndex
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

}

void Sword::SwordUpdate(void)
{
	//剣の位置変更
	switch (swordPosition)
	{

	case Sword::SWORD_POSITION::Hand: {

		// ボーンは名前で検索(名前はモデルに合わせて変更)
		const int handFrame = MV1SearchFrame(playerTrans.model, "Bone_036");
		if (handFrame < 0) { return; }

		MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

		trans.pos = Vector3(hand.m[3][0], hand.m[3][1], hand.m[3][2]);
		trans.rotation = Quaternion::FromMatrix(hand);
		trans.AddAngleYDeg(90.0f);

		break;
	}

	case Sword::SWORD_POSITION::Back: {


		static float workCounter = 0.0f;
		workCounter += 0.5f;

		Vector3 goalPos = playerTrans.pos + playerTrans.rotation.Rotate(Vector3(-10, 60, -30));

		trans.pos = trans.pos * 0.5f + goalPos * 0.5f;
		trans.pos.y += sinf(workCounter / 8.0f) * 1.0f;

		trans.rotation = Quaternion::FromRotationZ(Deg2Rad(-90.0f));
		trans.AddAngleXDeg(5.0f);
		trans.AddAngleYDeg(workCounter);

		break;
	}

	case Sword::SWORD_POSITION::HandtoBack: {

		break;
	}

	case Sword::SWORD_POSITION::BacktoHand: {
		break;
	}

	default:
		break;
	}
}

void Sword::ActiveUpdate(void)
{
	switch (active)
	{
	case Sword::ACTIVE::Active:
	{
		//描画ON
		SetIsDraw(true);
		//判定オフ
		SetJudgeFlg(true);

		break;
	}
	case Sword::ACTIVE::DrawOnly:
	{
		//描画ON
		SetIsDraw(true);
		//判定オフ
		SetJudgeFlg(false);
		break;
	}
	case Sword::ACTIVE::NotActive:
	{
		//描画ON
		SetIsDraw(false);
		//判定オフ
		SetJudgeFlg(false);
		break;
	}
	}
}
