#include "SkillFire.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/Collider/SphereCollider.h"

SkillFire::SkillFire
(
	const Transform& userTrans,
	SKILL_USER user
):
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

	//当たり判定の追加
	AddSkillCollider(FIRE_RADIUS);

	//生存時間
	skillAliveTime = FIRE_ALIVE_TIME;
}

void SkillFire::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	//当てる相手に当たったら消える（ダメージは相手側のOnCollisionで処理）
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
	trans.pos += moveDir * FIRE_SPEED * TimeScale::Get();
}

void SkillFire::SkillEnd(void)
{
}

void SkillFire::SubDraw(void)
{
	//モデルが無いので仮で球を描画
	DrawSphere3D(trans.pos.ToVECTOR(), FIRE_RADIUS, 12, 0xff4000, 0xffa000, true);
}
