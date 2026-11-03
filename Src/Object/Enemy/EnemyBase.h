#pragma once
#include "../Common/CharacterBase/CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:

	enum class ENEMY_TYPE
	{
		None = -1,

		Chilbi,	//チルビー

		Max
	};

	EnemyBase(const Vector3& initPos);

	~EnemyBase()override = default;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

protected:


#pragma region 定数・変数

	// 無敵時間(フレーム)
	static constexpr float DEFAULT_INVINCIBLE_FRAME = 60.0f;

	// プレイヤーの攻撃で受けるダメージ
	static constexpr int DEFAULT_DAMAGE = 1;


#pragma endregion



	// オブジェクト初期設定
	void SetupEnemyObject(int pushWeight);

	// 初期化
	virtual void EnemyInit(void) {}

	// 更新
	virtual void EnemyUpdate(void) {}

	// 描画
	virtual void EnemyDraw(void) {}

	// プレイヤーの攻撃を受けたときのダメージ量(必要なら上書き)
	virtual int GetDamage(COLLIDER_TAG otherTag) const;

	// 被弾時(死亡していない場合)
	virtual void OnHit(void) = 0;
	// 死亡時
	virtual void OnDie(void) = 0;


	//無敵中か
	bool IsInvincible(void) const { return invincibleCounter > 0.0f; }

	// 無敵開始
	void StartInvincible(float frame = DEFAULT_INVINCIBLE_FRAME) { invincibleCounter = frame; }

	// 初期座標
	const Vector3& GetInitPos(void) const { return initPos; }


private:

	// 初期読み込み
	void SubInit(void)override final;

	// 更新
	void SubUpdate(void)override final;

	// 描画
	void SubDraw(void)override final;

	// 初期座標
	Vector3 initPos;

};

