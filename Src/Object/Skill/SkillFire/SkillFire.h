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
	//火の玉の速さ（1フレームの移動量）
	static constexpr float FIRE_SPEED = 25.0f;
	//火の玉の生存時間（フレーム数）
	static constexpr float FIRE_ALIVE_TIME = 60.0f;

#pragma endregion

	//ファイアの開始処理
	void SkillStart(void) override;
	//ファイアの発動中処理
	void SkillUpdate(void) override;
	//ファイア終了処理
	void SkillEnd(void) override;

	//仮の描画
	void SubDraw(void) override;
};
