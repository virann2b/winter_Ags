#include "RangeAttack.h"

#include "../../../../Utility/Utility.h"

#include "../../../Common/Collider/SphereCollider.h"


RangeAttack::RangeAttack
(const Transform& playerTrans)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans),
	scale(SCALE::None),
	scaleSize(0.0f),
	scaleSizeMin(0.0f),
	scaleSizeNormal(0.0f),
	scaleSizeBig(0.0f),
	attackCollRef(nullptr)
{
}

void RangeAttack::Load(void)
{

#pragma region オブジェクト設定

	//移動判定ON
	SetDynamicFlg(true);
	//重力OFF
	SetGravityFlg(false);
	//コライダー同士の押し出しOFF
	SetPushFlg(false);
	//当たり判定OFF
	SetJudgeFlg(false);

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


	//初期設定サイズ
	scaleSize = scaleSizeBig;

	// 生成、ポインタ保持
	attackCollRef = new SphereCollider(COLLIDER_TAG::PlayerLargeAttack, scaleSize, trans.pos);


#pragma endregion

#pragma endregion

	//属性の登録
	attackCollRef->SetElement(ELEMENT::Physics);

	//怯み値の設定
	attackCollRef->SetStagger(RANGE_STAGGER);

	//HIT時のSE
	attackCollRef->SetHitSE("SwordHit_S");

	// コライダーの生成
	AddCollider(attackCollRef);

	//パラメータ追加
	ApplyAttackParam("RangeAttack", COLLIDER_TAG::PlayerLargeAttack);

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

	//半径変更
	attackCollRef->SetRadius(scaleSize);

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
	if (!GetJudgeFlg()) return;
	
	trans.pos.x = playerTrans.pos.x;
	trans.pos.z = playerTrans.pos.z ;
	trans.pos.y = 40;

}

void RangeAttack::SubDraw(void)
{
}
