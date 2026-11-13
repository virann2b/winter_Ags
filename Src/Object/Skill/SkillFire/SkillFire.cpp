#include "SkillFire.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/Collider/SphereCollider.h"

SkillFire::SkillFire(const Transform& userTrans,SKILL_USER user)
	:
	SkillBase(userTrans, user)
{
}

void SkillFire::Load(void)
{
	// モデルをロード
	trans.LoadModel("Skill/Fire/Fire");

	trans.scale = 0.5f;

	//オブジェクト初期設定
	SetUpSkillObject();

#pragma region パラメータ登録

	//属性登録
	element = ELEMENT::Fire;
	//スキルアイコン登録
	iconTag = Img::IMAGE_TAG::Element_Fire;

	//ダメージ量
	damage = FIRE_DAMAGE;
	//攻撃サイズ
	collRadius = FIRE_RADIUS;
	//攻撃スピード
	moveSpeed = FIRE_SPEED;
	//スキル生存時間
	skillAliveTime = FIRE_ALIVE_TIME;
	//スキル消費コスト
	mpCost = FIRE_MP_COST;

#pragma endregion

	//当たり判定の追加
	AddSkillCollider();
}

void SkillFire::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	//当てる相手に当たったら消える
	if (IsTarget(other.GetTag())) { Finish(); }
}

void SkillFire::SkillStart(void)
{
	//正面の方向に位置を調整
	const Vector3 offset = userTrans.rotation.Rotate(Vector3(0.0f, SKILL_OFFSET_POS_Y, SKILL_OFFSET_POS_Z));
	//位置を更新
	trans.pos = userTrans.pos + offset;

	//スキル使用者の向きを取得（発動時の向きでまっすぐ飛ぶ）
	moveDir = userTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
}

void SkillFire::SkillUpdate(void)
{
	//まっすぐ進む
	trans.pos += moveDir * moveSpeed * TimeScale::Get();
}

