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
	bullet(bullet),
	recoverTimer(0.0f)
{
}

void Gun::Load(void)
{
	// モデルをロード
	trans.LoadModel("Gun/Gun");


	AddCollider(
		new CapsuleCollider(COLLIDER_TAG::PlayerSword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 0.2f;

	trans.localRotation = Quaternion::FromRotationY(Deg2Rad(-90.0f));

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
	if (bullet <= 0) return;

	bullet = (std::max)(bullet - useBullet, 0);

	BulletObj b;
	b.dir = GetShotDirection();
	b.moved = 0.0f;
	b.pos = trans.pos + GetRandomOffset();
	bullets.push_back(b);
}

void Gun::SubUpdate(void)
{
	SetIsDraw(isActive);

	////銃の設定位置フレーム
	//const int handFrame =
	//	MV1SearchFrame(playerTrans.model, "smartrig:RightHand");

	//if (handFrame < 0) { return; }

	//MATRIX hand =
	//	MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	//// 手のワールド座標
	//Vector3 handPos(
	//	hand.m[3][0],
	//	hand.m[3][1],
	//	hand.m[3][2]
	//);
	//// 回転行列からスケールを除去
	//MATRIX rotationMat = hand;
	//for (int i = 0; i < 3; i++)
	//{
	//	float length = sqrtf(
	//		rotationMat.m[i][0] * rotationMat.m[i][0] +
	//		rotationMat.m[i][1] * rotationMat.m[i][1] +
	//		rotationMat.m[i][2] * rotationMat.m[i][2]
	//	);
	//	if (length > 0.00001f)
	//	{
	//		for (int j = 0; j < 3; j++)
	//		{
	//			rotationMat.m[i][j] /= length;
	//		}
	//	}
	//}
	//// 手の回転を取得
	//Quaternion handRotation =
	//	Quaternion::FromMatrix(rotationMat);

	//// 手の回転に銃の持ち方の補正を加える
	//trans.rotation = handRotation * gripRotation;
	//// 手のローカル座標系で位置を補正
	//trans.pos = handPos + handRotation.Rotate(gripOffset);
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

void Gun::FollowHand(void)
{
	const int handFrame =
		MV1SearchFrame(playerTrans.model, "smartrig:RightHand");
	if (handFrame < 0) { return; }

	if (handFrame < 0) { return; }

	MATRIX hand =
		MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	// 手のワールド座標
	Vector3 handPos(
		hand.m[3][0],
		hand.m[3][1],
		hand.m[3][2]
	);
	// 回転行列からスケールを除去
	MATRIX rotationMat = hand;
	for (int i = 0; i < 3; i++)
	{
		float length = sqrtf(
			rotationMat.m[i][0] * rotationMat.m[i][0] +
			rotationMat.m[i][1] * rotationMat.m[i][1] +
			rotationMat.m[i][2] * rotationMat.m[i][2]
		);
		if (length > 0.00001f)
		{
			for (int j = 0; j < 3; j++)
			{
				rotationMat.m[i][j] /= length;
			}
		}
	}
	// 手の回転を取得
	Quaternion handRotation =
		Quaternion::FromMatrix(rotationMat);

	// 手の回転に銃の持ち方の補正を加える
	trans.rotation = handRotation * gripRotation;

	// 手のローカル座標系で位置を補正
	trans.pos = handPos + handRotation.Rotate(gripOffset);
	Vector3 playerDelta = playerTrans.pos - prevPlayerPos;
	trans.pos += playerDelta;
	prevPlayerPos = playerTrans.pos;


}

void Gun::DrawBulletUI(void) const
{
	// 画面中央を円の中心にする(仮)
	int sw = 0, sh = 0;
	GetScreenState(&sw, &sh, nullptr);
	const float cx = sw * 0.5f;
	const float cy = sh * 0.5f;

	const float PI = 3.14159265f;
	const float RING_RADIUS = 150.0f;	// リングの半径
	const float BULLET_UI_R = 12.0f;	// 弾アイコンの半径

	for (int i = 0; i < MAX_POSSESSION_BULLET; ++i)
	{
		// 真上から時計回りに等間隔で配置
		float rad = -PI * 0.5f + (PI * 2.0f) * i / MAX_POSSESSION_BULLET;
		float x = cx + cosf(rad) * RING_RADIUS;
		float y = cy + sinf(rad) * RING_RADIUS;

		if (i < bullet)
		{
			// 残っている弾:金色で塗りつぶし+縁取り
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0xd4af37, TRUE);
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0x6b5a14, FALSE, 2.0f);
		}
		else
		{
			// 空の弾:暗い輪郭のみ
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0x555555, FALSE, 2.0f);
		}
	}

	// リング中央に数字も表示(確認用)
	DrawFormatString((int)cx - 20, (int)cy - 8, 0xffffff,
		"%d / %d", bullet, MAX_POSSESSION_BULLET);
}

Vector3 Gun::GetRandomOffset(void) const
{
	// static にして毎回シードし直さない
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> dist(-SHOT_OFFSET_RANGE, SHOT_OFFSET_RANGE);

	return Vector3(dist(rng), dist(rng), dist(rng));
}