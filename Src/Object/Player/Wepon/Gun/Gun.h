#pragma once
#include "../../../Common/ActorBase/ActorBase.h"

#include "../../../../Utility/Utility.h"

#include <vector>

class Gun :public ActorBase
{
public:

	// 仮の弾
	struct BulletObj
	{
		Vector3 pos{ 0,0,0 };        // 現在位置
		Vector3 dir{ 0,0,0 };        // 進行方向(正規化済み)
		float  moved = 0.0f;      // 移動した距離
	};

	Gun(const Transform& playerTrans,
		bool isAlive, int& bullet);

	~Gun() override = default;

	void Load(void) override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) override;

	//武器切り替え
	void SetActive(bool active) {isActive = active;	}

	//弾発射処理
	void Shot(int useBullet);

	//弾数取得
	int GetBullet(void) const { return bullet; }

	//弾情報の更新
	void BulletUpdate(void);

	void BulletMoveUpdate(void);   // 弾の移動・消去
	void BulletDraw(void);         // 弾の描画

	Vector3 GetShotDirection(void) const;

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

	//最大弾所持数
	static constexpr int  MAX_POSSESSION_BULLET = 48;

	//弾回復時間
	static constexpr float MAX_RECOVER_TIME = 30.0f;

	static constexpr float BULLET_SPEED = 30.0f;     // 1フレームの移動量
	static constexpr float BULLET_RANGE = 1500.0f;   // この距離で消える
	static constexpr float BULLET_RADIUS = 8.0f;

	std::vector<BulletObj> bullets;

#pragma endregion

	// 手のボーンからの位置補正（手のローカル座標系）
	Vector3 gripOffset = Vector3(-5.0f, 20.0f, -5.0f);
	// 手のボーンからの回転補正
	Quaternion gripRotation =
		Quaternion::FromEulerXZY(Vector3(Deg2Rad(90.0f), Deg2Rad(180.0f), Deg2Rad(-10.0f)));

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	//武器切り替えフラグ
	bool isActive;
	//弾
	int& bullet;
	//弾クールタイム
	float recoverTimer;

	//派生クラス更新
	void SubUpdate(void) override;

	Vector3 GetRandomOffset(void) const;
	static constexpr float SHOT_OFFSET_RANGE = 30.0f;   // ずらす最大幅(±)
};