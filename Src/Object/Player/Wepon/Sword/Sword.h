#pragma once
#include "../../../Common/ActorBase/ActorBase.h"


class Sword
	:public ActorBase
{
public:

	enum class SWORD_POSITION
	{
		None = -1,

		Hand,	//手

		Back,	//背中

		//手から背中の間
		HandtoBack,
		//背中から手の間
		BacktoHand,
		
		MAX
	};

	Sword(const Transform& playerTrans, bool isActive);

	~Sword() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	void SetActive(bool active) { isActive = active; }

private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(0.0f, 400.0f, 0.0f);

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;
	//落下時の最大Y座標
	static constexpr float MAX_FALL_POS_Y = -200.0f;
	//拡大量
	static constexpr float SCALE_POW = 0.05f;

	//右手のフレーム番号
	static constexpr int RIGHT_HAND_FLAME_INDEX = 17;

#pragma endregion

	//剣の待機位置
	SWORD_POSITION swordPosition;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	//調整座標パラメータ
	Vector3 justPos;

	bool isActive;

	void SubUpdate(void) override;
	void SubDraw(void)override;
};

