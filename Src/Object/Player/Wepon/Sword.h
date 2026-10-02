#pragma once
#include "../../Common/ActorBase/ActorBase.h"


class Sword
	:public ActorBase
{
public:

	Sword(const Transform& playerTrans);

	~Sword() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;


private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(0.0f, 400.0f, 0.0f);

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;
	//落下時の最大Y座標
	constexpr static float MAX_FALL_POS_Y = -200.0f;
	//拡大量
	constexpr static float SCALE_POW = 0.05f;

#pragma endregion


	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;


	void SubUpdate(void) override;
};

