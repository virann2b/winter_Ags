#include "RangeAttack.h"

#include "../../../../Utility/Utility.h"

#include "../../../Common/Collider/SphereCollider.h"


RangeAttack::RangeAttack
(const Transform& playerTrans)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans)
{
}

void RangeAttack::Load(void)
{
	//移動判定ON
	SetDynamicFlg(true);
	//重力OFF
	SetGravityFlg(false);
	//当たり判定ON
	//SetJudgeFlg(true);

#pragma region 外部読み込み

	// その回のパラメーター名
	std::string parameterName = "SizeMin";
	// そのパラメータが存在するかどうか
	if (!IsParameterExist("AttackLange", parameterName)) { return; }
	scaleSizeMin = (float)GetParameterToInt("AttackLange", parameterName);

	// その回のパラメーター名
	parameterName = "SizeNormal";
	// そのパラメータが存在するかどうか
	if (!IsParameterExist("AttackLange", parameterName)) { return; }
	scaleSizeNormal = (float)GetParameterToInt("AttackLange", parameterName);

	// その回のパラメーター名
	parameterName = "SizeBig";
	// そのパラメータが存在するかどうか
	if (!IsParameterExist("AttackLange", parameterName)) { return; }
	scaleSizeBig = (float)GetParameterToInt("AttackLange", parameterName);

#pragma endregion

	//初期セットサイズ
	Start(SCALE::Big);

	AddCollider(new SphereCollider
	(COLLIDER_TAG::PlayerLargeAttack, scaleSize, trans.pos));

}

void RangeAttack::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void RangeAttack::Start(SCALE setScale)
{
	switch (setScale)
	{
	case RangeAttack::SCALE::Min:
	{
		scaleSize = scaleSizeMin;

		break;
	}
	case RangeAttack::SCALE::Normal:
	{
		scaleSize = scaleSizeNormal;

		break;
	}
	case RangeAttack::SCALE::Big:
	{
		scaleSize = scaleSizeBig;

		break;
	}
	}

	//当たり判定ON
	SetJudgeFlg(true);
}

void RangeAttack::End(void)
{
	//当たり判定OFF
	SetJudgeFlg(false);
}


void RangeAttack::SubUpdate(void)
{

	//// ボーンは名前で検索(名前はモデルに合わせて変更)
	//const int Frame = MV1SearchFrame(playerTrans.model, "smartrig:LeftToeBase"); //smartrig:LeftToeBase
	//if (Frame < 0) { return; }

	//MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, Frame);

	//trans.pos = Vector3(hand.m[3][0], hand.m[3][1], hand.m[3][2]);
	//trans.rotation = Quaternion::FromMatrix(hand);
	//trans.AddAngleXDeg(-15.0f);

	trans.pos.x = playerTrans.pos.x;
	trans.pos.z = playerTrans.pos.z;
	trans.pos.y = 0;


	//Start(SCALE::Big);
}

void RangeAttack::SubDraw(void)
{
}
