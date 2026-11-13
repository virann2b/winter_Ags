#pragma once
#include"../SkillBase.h"

class SkillIce : public SkillBase
{
public:

	SkillIce(const Transform& userTrans, SKILL_USER user);
	~SkillIce()override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

private:

#pragma region 定数

	//氷の半径（攻撃範囲）
	static constexpr float ICE_RADIUS = 40.0f;
	//氷の速さ（1フレームの移動量）
	static constexpr float ICE_SPEED = 25.0f;
	//氷の生存時間（フレーム数）
	static constexpr float ICE_ALIVE_TIME = 60.0f;
	//消費MP
	static constexpr int ICE_MP_COST = 20;
	//ダメージ量
	static constexpr int ICE_DAMAGE = 10;

#pragma endregion

	//アイスの開始処理
	void SkillStart(void) override;
	//アイスの発動中処理
	void SkillUpdate(void) override;
	//アイス終了処理
	void SkillEnd(void) override;

	//仮の描画
	void SubDraw(void) override;
};
