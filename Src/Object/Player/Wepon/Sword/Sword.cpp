#include "Sword.h"

#include "../../../../Utility/Utility.h"

#include "../../../../Manager/AttackParam/AttackParamManager.h"

#include "../../../Common/Collider/CapsuleCollider.h"


Sword::Sword(const Transform& playerTrans, bool isActive)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans),
	isActive(isActive),
	swordPosition(SWORD_POSITION::None),
	isJudge(false),
	handIndex(-1),
	active(ACTIVE::None)
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

	// モデルをロード
	trans.LoadModel("Sword/Sword");

	//サイズ設定
	trans.scale = 0.7f;
	//ずれ補正
	trans.centerDiff = Vector3::Xonly(-50.0f);
	//角度の調整
	trans.SetLocalRotation(Quaternion::FromRotationZ(Deg2Rad(-180.0f)));

	//剣の初期位置
	swordPosition = SWORD_POSITION::Hand;

#pragma endregion

	//コライダーの追加
	 swordColl =new CapsuleCollider
	 (COLLIDER_TAG::PlayerSword,
		Vector3::Xonly(15.0f) * trans.scale,
		Vector3::Xonly(160.0f) * trans.scale,
		60.0f * trans.scale.MaxElementF());
	//属性の登録
	swordColl->SetElement(ELEMENT::Physics);

	//当たった時のSE
	swordColl->SetHitSE("SwordHit_S");
	//コライダー登録
	AddCollider(swordColl);

	//パラメータ追加
	ApplyAttackParam("SwordNormal", COLLIDER_TAG::PlayerSword);

}

void Sword::SetStagger(int stagger)
{
	//怯み値を剣を経由で登録
	if (swordColl != nullptr) { swordColl->SetStagger(stagger); }
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

		//剣のずれの大きさ
		static float workCounter = 0.0f;
		workCounter += 0.5f;

		//最終座標位置
		Vector3 goalPos = playerTrans.pos + playerTrans.rotation.Rotate(Vector3(-10, 60, -30));

		//剣の移動・揺れ
		trans.pos = trans.pos * 0.5f + goalPos * 0.5f;
		trans.pos.y += sinf(workCounter / 8.0f) * 1.0f;

		trans.rotation = Quaternion::FromRotationZ(Deg2Rad(-90.0f));
		trans.AddAngleXDeg(5.0f);
		trans.AddAngleYDeg(workCounter);

		break;
	}
	case Sword::SWORD_POSITION::Guard:
	{
		AttachToHand(GUARD_ROT_OFFSET_DEG, GUARD_POS_OFFSET);

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

void Sword::SetAttackParam(const std::string& id)
{
	//空の場合は何もしない
	if (swordColl == nullptr) { return; }

	//剣にパラメータを追加
	AttackParamManager::GetIns().Apply(*swordColl, id);
	
}

void Sword::AttachToHand(const Vector3& rotOffsetDeg, const Vector3& posOffset)
{
	const int handFrame = MV1SearchFrame(playerTrans.model, "Bone_036");
	if (handFrame < 0) { return; }

	const MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);
	const Quaternion handRot = Quaternion::FromMatrix(hand);

	//手のローカル軸基準での補正回転
	const Quaternion offsetRot = Quaternion::FromEulerXZY(Vector3(
		Deg2Rad(rotOffsetDeg.x),
		Deg2Rad(rotOffsetDeg.y),
		Deg2Rad(rotOffsetDeg.z)));

	//位置：手の位置＋手の向きに合わせたずらし
	trans.pos = Vector3(hand.m[3][0], hand.m[3][1], hand.m[3][2])
		+ handRot.Rotate(posOffset);

	//回転：補正 → 手の回転 の順に合成
	trans.rotation = Quaternion::FromMatrix(MMult(offsetRot.ToMatrix(), handRot.ToMatrix()));
	trans.AddAngleYDeg(90.0f);
}
