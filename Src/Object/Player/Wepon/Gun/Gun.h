#pragma once
#include "../../../Common/ActorBase/ActorBase.h"

#include "../../../../Utility/Utility.h"

#include <vector>

class EnemyBase;

class Gun :public ActorBase
{
public:

	Gun(const Transform& playerTrans,
		bool isAlive);

	~Gun() override = default;

	//読み込み
	void Load(void) override;

	//弾情報の更新
	void UpdateRecover(void);
	//標準の更新
	void UpdateAim(int targetId);
	//着弾更新
	void UpdateHit(void);


	//弾の残弾数描画
	void DrawBulletUI(void) const;
	//照準の描画
	void DrawAimUI(void) const;
	

	//武器切り替え
	void SetActive(bool active) {isActive = active;	}

	//弾発射処理
	void UseBullet(int useBullet);

	//銃の位置調整
	void FollowHand(void);
	//照準のリセット
	void ResetAim(void);

	//番号から敵を探す
	void SetEnemyFinder(std::function<EnemyBase* (int) > finder) { findEnemy = finder; }
	//射撃
	void Shot(void);

	//残弾数取得
	int GetBullet(void) const { return remainingBullet; }
	//照準しているの数取得
	int GetAim(void) const { return aimCount; }

private:

	//描画する標準位置
	enum class AIMING_POS
	{
		None = -1,
		
		Center,		//中心
		Up,			//上
		Left,		//左
		Right,		//右
		DownLeft,	//左下
		DownRight,	//右下

		Max
	};

	// 着弾予定
	struct Hit
	{
		int targetId;	// 敵の番号
		int index;		// 何発目か
		float timer;	// 着弾までの残りフレーム
	};

#pragma region 定数

	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;
	//右手のフレーム番号
	static constexpr int RIGHT_HAND_FLAME_INDEX = 17;
	// ずらす最大幅
	static constexpr float SHOT_OFFSET_RANGE = 30.0f;  

	static constexpr float AIM_DRAW_RADIUS = 16.0f;

	float HIT_INTERVAL = 3.0f;		// 着弾の時間差（フレーム）
	int damage = 2;

	//銃パラメータ
	struct GunParams
	{
		//数値を入れてるのは保険
		float MAX_AIMING_TIME = 10.0f;		//1発目の照準最大時間
		float RECOVER_TIME = 180.0f;		//弾回復時間
		int MAX_AMMO = 48;					//最大弾所持数
		int MAX_SHOT= 6;					//一度に打てる最大弾数
		float AIM_TIME_DECREASE = 1.0f;		//照準の減少時間
		float AIM_SPREAD = 60.0f;			//照準の広がり
	};


#pragma endregion

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 番号から敵を探す関数
	std::function< EnemyBase*(int)> findEnemy;

	std::vector<Hit> hitBullet;

	// 手のボーンからの位置補正
	Vector3 gripOffset = Vector3(-5.0f, 20.0f, -5.0f);

	// 手のボーンからの回転補正(銃の回転の場合この数値を）
	Quaternion gripRotation =
		Quaternion::FromEulerXZY(Vector3(Deg2Rad(90.0f), Deg2Rad(180.0f), Deg2Rad(-10.0f)));

	//銃のパラメータ
	GunParams params;

	//武器切り替えフラグ
	bool isActive;

	//照準を置いている敵の番号
	int aimTargetId;

	//残弾数
	int remainingBullet;
	//照準の数
	int aimCount;

	//弾クールタイム
	float recoverTimer;
	//照準時間
	float aimingTimer;

	int debugHitCount = 0;

	//1フレーム前のプレイヤー座標
	Vector3 prevPlayerPos;

	//派生クラス更新
	void SubUpdate(void) override;

	/// <summary>
	///照準を置くまでの時間
	/// </summary>
	/// <param name="aimCount">照準の数</param>
	/// <returns></returns>
	float GetAimTime(int aimCount) const;

	//照準のワールド座標
	Vector3 GetAimWorldPos(const EnemyBase& enemy, int index) const;

};