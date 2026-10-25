#pragma once
#include "../../../Common/ActorBase/ActorBase.h"

class RangeAttack : public ActorBase
{
public:

	//範囲攻撃サイズ
	enum class SCALE
	{
		None = -1,

		//小
		Min,

		//中
		Normal,

		//大
		Big,

		Max
	};

	RangeAttack();

	~RangeAttack() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	//開始
	void Start(SCALE setScale);
	//終了
	void End(void);

private:

#pragma region 定数

	//指定サイズごとの範囲攻撃の最大範囲
	float scaleSizeMin;
	float scaleSizeNormal;
	float scaleSizeBig;

#pragma endregion


	SCALE scale;

	float scaleSize;

	// プレイヤーのモデル制御情報の参照
	//const Transform& playerTrans;

	void SubUpdate(void) override;
	void SubDraw(void)override;


};

