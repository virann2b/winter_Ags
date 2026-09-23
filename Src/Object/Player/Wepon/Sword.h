#pragma once
#include "../../Common/ActorBase/ActorBase.h"

class Sword
	:public ActorBase
{
public:

	// 状態
	enum class STATE
	{
		None = -1,

		//生成中状態
		Create,

		//待機状態
		Wait,

		//落ちる状態
		Fall,

		Max
	};

	Sword(const Vector3*& playerPos,
		const Transform& folliowTransform);

	~Sword() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	// 攻撃開始
	void Start(void);

private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(0.0f, 400.0f, 0.0f);

	Vector3 handLocalPos = { -65.0f,200.0f,0.0f };

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;

#pragma endregion

#pragma region 受け取る参照


	// 所有者のTransform（プレイヤーなど）
	const Transform& followTransform;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

#pragma endregion


	Vector3 localPos;
	Vector3 localRot;

	//現状状態
	STATE state;

	// Wait状態のカウンター
	unsigned short waitCounter;



	void SubUpdate(void) override;
};




