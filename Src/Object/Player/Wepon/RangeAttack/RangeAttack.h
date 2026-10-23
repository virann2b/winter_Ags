#pragma once
#include "../../../Common/ActorBase/ActorBase.h"

class RangeAttack : public ActorBase
{
public:

	//範囲攻撃の順序
	enum class STEP
	{
		None = -1,

		Max
	};

	enum class SCALE
	{
		None = -1,

		Max
	};

	RangeAttack();

	~RangeAttack() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;


private:

#pragma region 定数



#pragma endregion

	

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	void SubUpdate(void) override;
	void SubDraw(void)override;


};

