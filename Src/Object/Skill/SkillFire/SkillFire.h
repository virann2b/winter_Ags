#pragma once

#include"../SkillBase.h"

class SkillFire : public SkillBase
{
public:

	SkillFire(const Transform& userTrans, SKILL_USER user);
	~SkillFire()override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

private:

#pragma region 定数

	//火の玉の半径
	static constexpr float FIRE_RADIUS = 40.0f;
	//火の玉の速さ
	static constexpr float FIRE_SPEED = 25.0f;
	//火の玉の生存時間
	static constexpr float FIRE_ALIVE_TIME = 60.0f;
	//消費MP
	static constexpr int FIRE_MP_COST = 20;
	//ダメージ量
	static constexpr int FIRE_DAMAGE = 10;

#pragma endregion

	//ファイアの開始処理
	void SkillStart(void) override;
	//ファイアの発動中処理
	void SkillUpdate(void) override;

};
