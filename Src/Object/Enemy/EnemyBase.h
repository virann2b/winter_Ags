#pragma once
#include "../Common/CharacterBase/CharacterBase.h"

#include <vector>

class SkillBase;

class EnemyBase : public CharacterBase
{
public:

	//敵の種類
	enum class ENEMY_TYPE
	{
		None = -1,

		Chilbi,	//チルビー

		Max
	};

	//距離の種類
	enum class DISTANCE
	{
		None = -1,

		Near,		//近距離
		Middle,		//中距離
		Long,		//遠距離

		Out,		//範囲外	

		Max
	};


	EnemyBase(const Vector3& initPos);

	~EnemyBase()override = default;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 攻撃対象の座標を設定
	void SetTargetPos(const Vector3* targetPos) { this->targetPos = targetPos; }

	// 死亡アニメーションまで終わったか（画面から消えたか）
	bool IsDieEnd(void) const { return isDieEnd; }
;

protected:


#pragma region 定数・変数

	// 無敵時間(フレーム)
	static constexpr float DEFAULT_INVINCIBLE_FRAME = 60.0f;

	// 攻撃クールタイム（フレーム）
	static constexpr float DEFAULT_ATTACK_COOLTIME = 120.0f;

	// プレイヤーの攻撃で受けるダメージ
	static constexpr int DEFAULT_DAMAGE = 1;

	// 距離の区分
	static constexpr float NEAR_RANGE = 200.0f;
	static constexpr float MIDDLE_RANGE = 500.0f;
	static constexpr float LONG_RANGE = 900.0f;

	// 停止距離
	static constexpr float DEFAULT_STOP_RANGE = 200.0f;

	// カウント用攻撃クールタイム
	float attackCoolTime;

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

	// 死亡アニメーション終了時に呼ぶ（描画・当たり判定を消す）
	void DieEnd(void);

	//攻撃範囲外（距離)
	bool IsFar(float stopRange = DEFAULT_STOP_RANGE) const;

	//攻撃範囲（距離）
	bool IsNear(float stopRange = DEFAULT_STOP_RANGE) const;

	// プレイヤーへの単位ベクトル
	Vector3 GetDirectionToTarget(void) const;

#pragma region スキル関連

	// 攻撃対象との距離の区分を取得
	DISTANCE GetTargetDistance(void) const;

	/// <summary>
	/// スキルを追加する（子アクターとして生成される）
	/// </summary>
	/// <param name="skill">スキル（SKILL_USER::Enemyで生成したもの）</param>
	/// <param name="minDistance">使える距離の下限</param>
	/// <param name="maxDistance">使える距離の上限</param>
	/// <param name="coolTime">使った後のクールタイム（フレーム）</param>
	void AddSkill(SkillBase* skill, DISTANCE minDistance, DISTANCE maxDistance, float coolTime);

	// 今の距離で使えるスキルからランダムに1つ選んで使う（使えたらtrue）
	bool TryUseSkill(void);

	// どれかのスキルが発動中か
	bool IsSkillActive(void) const;

#pragma endregion

private:

	// 初期読み込み
	void SubInit(void)override final;

	// 更新
	void SubUpdate(void)override final;

	// 描画
	void SubDraw(void)override final;

	// 初期座標
	Vector3 initPos;
	
	// 攻撃対象の座標
	const Vector3* targetPos = nullptr;

	// 死亡アニメーションまで終わったか
	bool isDieEnd = false;

	// スキル枠
	struct SkillSlot
	{
		SkillBase* skill;		// スキル本体
		DISTANCE minDistance;	// 使える距離の下限
		DISTANCE maxDistance;	// 使える距離の上限
		float coolTime;			// クールタイム（フレーム）
		float coolCount;		// 残りクールタイム（フレーム）
	};
	std::vector<SkillSlot> skillSlots;

	// スキルのクールタイム更新
	void SkillCoolTimeUpdate(void);

};

