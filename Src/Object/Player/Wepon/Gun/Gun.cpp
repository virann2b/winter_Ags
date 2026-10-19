#include "Gun.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"

#include "../../Player.h"

#include <random>


Gun::Gun(const Transform& playerTrans,
	bool isAlive, int& bullet)
	:
	ActorBase(),
	playerTrans(playerTrans),
	isActive(false),
	bullet(bullet)
{
}

void Gun::Load(void)
{
	// モデルをロード
	trans.LoadModel("Gun/Gun");


	AddCollider(
		new CapsuleCollider(COLLIDER_TAG::Sword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 0.3f;

	isActive = false;
	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	SetJudgeFlg(false);

	//所持弾数初期化
	bullet = MAX_POSSESSION_BULLET;
	//弾クールタイム初期化
	recoverTimer = MAX_RECOVER_TIME;
}

void Gun::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Gun::Shot(int useBullet)
{
	//すでに弾が0の場合
	if (bullet <= 0) return;

	bullet = bullet - useBullet;

	if (bullet <= 0) return;

	bullet = (std::max)(bullet - useBullet, 0);

	BulletObj b;
	b.dir = GetShotDirection();
	b.moved = 0.0f;

	// 発射位置をランダムにずらす
	b.pos = trans.pos + GetRandomOffset();

	bullets.push_back(b);


	//途中で弾が0になった場合
	if (bullet <=  0) return;
}

void Gun::SubUpdate(void)
{
	//描画有効
	SetIsDraw(isActive);

	// ボーンは名前で検索(名前はモデルに合わせて変更)
	const int handFrame = MV1SearchFrame(playerTrans.model, "Bone_036");
	if (handFrame < 0) { return; }

	MATRIX hand = MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	trans.pos = Vector3(hand.m[3][0], hand.m[3][1], hand.m[3][2]);
	trans.rotation = Quaternion::FromMatrix(hand);

	//調整角度
	//trans.AddAngleYDeg(50.0f);
	trans.AddAngleXDeg(50.0f);
	trans.AddAngleZDeg(50.0f);

}

void Gun::BulletUpdate(void)
{
	//弾が最大数持っていた場合のみ
	if (bullet >= MAX_POSSESSION_BULLET)
	{
		recoverTimer = 0.0f;
		return;
	}

	recoverTimer++;
	if (recoverTimer >= MAX_RECOVER_TIME)
	{
		//弾残数追加
		bullet++;
		//弾クールタイムリセット
		recoverTimer = 0.0f;

		if (bullet >= MAX_POSSESSION_BULLET)
		{
			bullet = MAX_POSSESSION_BULLET;
		}
	}
}

void Gun::BulletMoveUpdate(void)
{

	for (auto& b : bullets)
	{
		b.pos = b.pos + b.dir * BULLET_SPEED;
		b.moved += BULLET_SPEED;
	}

	// 射程を超えた弾を削除
	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(),
			[](const BulletObj& b) { return b.moved >= BULLET_RANGE; }),
		bullets.end());
}

void Gun::BulletDraw(void)
{
	for (const auto& b : bullets)
	{
		DrawSphere3D(VGet(b.pos.x, b.pos.y, b.pos.z),
			BULLET_RADIUS, 8, 0xffff00, 0xffffff, TRUE);
	}
}


Vector3 Gun::GetShotDirection(void) const
{
	// プレイヤーの向き(クォータニオン)で、基準の正面ベクトルを回転させる
	Vector3 dir = playerTrans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f));
	return dir.Normalized();   // Vector3 に正規化関数があれば
}

Vector3 Gun::GetRandomOffset(void) const
{
	// static にして毎回シードし直さない
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(-SHOT_OFFSET_RANGE, SHOT_OFFSET_RANGE);

	return Vector3(dist(rng), dist(rng), dist(rng));
}