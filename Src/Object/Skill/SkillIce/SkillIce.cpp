#include "SkillIce.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/Collider/SphereCollider.h"

SkillIce::SkillIce(const Transform& userTrans, SKILL_USER user)
	:
	SkillBase(userTrans, user)
{
}

void SkillIce::Load(void)
{
	// モデルをロード
	trans.LoadModel("Skill/Ice/Ice");

	trans.scale = 0.5f;

	//オブジェクト初期設定
	SetUpSkillObject();

#pragma region パラメータ登録

	//属性登録
	element = ELEMENT::Ice;
	//スキルアイコン登録
	iconTag = Img::IMAGE_TAG::Element_Ice;


	//ダメージ量
	damage = ICE_DAMAGE;
	//攻撃サイズ
	collRadius = ICE_RADIUS;
	//攻撃スピード
	moveSpeed = ICE_SPEED;
	//スキル生存時間
	skillAliveTime = ICE_ALIVE_TIME;
	//スキル消費コスト
	mpCost = ICE_MP_COST;

#pragma endregion

	//当たり判定の追加（collRadius・element・damageを使う）
	AddSkillCollider();
}

void SkillIce::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	//当てる相手に当たったら消える（ダメージは相手側のOnCollisionで処理）
	if (IsTarget(other.GetTag())) { Finish(); }
}

void SkillIce::SkillStart(void)
{
	//正面の方向に位置を調整
	const Vector3 offset = userTrans.rotation.Rotate(Vector3(0.0f, SKILL_OFFSET_POS_Y, SKILL_OFFSET_POS_Z));
	//位置を更新
	trans.pos = userTrans.pos + offset;

	//スキル使用者の向きを取得（発動時の向きでまっすぐ飛ぶ）
	moveDir = userTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
}

void SkillIce::SkillUpdate(void)
{
	//まっすぐ進む
	trans.pos += moveDir * moveSpeed * TimeScale::Get();
}

void SkillIce::SkillEnd(void)
{
}

void SkillIce::SubDraw(void)
{
}
