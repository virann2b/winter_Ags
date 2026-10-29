#pragma once
#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class Punch : public ActorBase
{
public:

	Punch(const Transform& chilbiTrans);

	~Punch() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	//攻撃当たり判定開始
	void JudgeStart(void);
	//攻撃当たり判定終了
	void JudgeEnd(void);

private:

	// プレイヤーのモデル制御情報の参照
	const Transform& chilbiTrans;

	void SubUpdate(void) override;
	//void SubDraw(void)override;


};

