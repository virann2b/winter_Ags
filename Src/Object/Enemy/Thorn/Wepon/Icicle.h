#pragma once
#include "../../../Common/ActorBase/ActorBase.h"


class Icicle
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

	Icicle(const Vector3*& playerPos);

	~Icicle() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	// 攻撃開始
	void Start(void);

	//状態渡し
	STATE Getstate(void) const { return state; }

private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(0.0f, 400.0f, 0.0f);

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;

#pragma endregion

	//現状状態
	STATE state;

	// Wait状態のカウンター
	unsigned short waitCounter;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	void SubUpdate(void) override;
};

