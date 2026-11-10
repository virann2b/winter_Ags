#pragma once
#include "../Common/CharacterBase/CharacterBase.h"
#include "../Skill/ElementDefine.h"

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


	// 敵がいない場合
	static constexpr int INVALID_ID = -1;

	EnemyBase(const Vector3& initPos);

	~EnemyBase()override = default;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;
	//銃専陽ダメージ処理
	void TakeGunDamege(int damage);

	// 硬直
	void Stun(float frame);
	// 弱点硬直
	void WeakStun(void);

	// 硬直中判定
	bool IsStunned(void) const { return isStunned; }
	//　弱点硬直判定
	bool IsWeakStun(void) const { return isWeakStun; }

	// 攻撃対象の座標を設定
	void SetTargetPos(const Vector3* targetPos) { this->targetPos = targetPos; }

	// 死亡アニメーションまで終わったか（画面から消えたか）
	bool IsDieEnd(void) const { return isDieEnd; }

	// 敵の番号（EnemyManagerが生成時に振る。同じ番号は二度と使われない）
	int GetId(void) const { return id; }
;

protected:


#pragma region 定数・変数

	// 無敵時間
	static constexpr float DEFAULT_INVINCIBLE_FRAME = 60.0f;

	// 攻撃クールタイム
	static constexpr float DEFAULT_ATTACK_COOLTIME = 120.0f;

	// 弱点硬直の時間
	static constexpr float WEAK_STUN_FRAME = 360.0f;

	// 弱点硬直中の被ダメージ倍率
	static constexpr float WEAK_DAMAGE_RATE = 1.5f;

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

	//最後に受けた攻撃属性
	ELEMENT lastHitElement = ELEMENT::None;

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

	// 被弾時
	virtual void OnHit(void) = 0;

	// 死亡時
	virtual void OnDie(void) = 0;

	// 硬直開始時
	virtual void OnStun(void) {}

	// 硬直させる時間
	float GetStunFrame(void) const { return stunFrame; }
;
	//硬直終了
	void EndStun(void) { isStunned = false; isWeakStun = false; }


	//無敵中か
	bool IsInvincible(void) const { return invincibleCounter > 0.0f; }

	// 無敵開始
	void StartInvincible(float frame = DEFAULT_INVINCIBLE_FRAME) { invincibleCounter = frame; }

	// 初期座標
	const Vector3& GetInitPos(void) const { return initPos; }

	// 死亡アニメーション終了時に呼ぶ（描画・当たり判定を消す）
	void DieEnd(void);

	//攻撃範囲外
	bool IsFar(float stopRange = DEFAULT_STOP_RANGE) const;

	//攻撃範囲
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

	/// <summary>
	/// 属性耐性テーブル読み込み
	/// </summary>
	/// <param name="enemyName">取得する敵の名前</param>
	void LoadResist(const std::string& enemyName);


	/// <summary>
	/// 属性の弱点判定
	/// </summary>
	/// <param name="weakelememt">う</param>
	/// <returns></returns>
	bool IsWeak(ELEMENT element) const;


#pragma endregion

private:

	// 番号を振れるのはEnemyManagerだけ
	friend class EnemyManager;

	//属性耐性テーブル
	ElementResistTable resistTable;

	// 敵の番号
	int id = INVALID_ID;

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

	// 死亡アニメーションフラグ
	bool isDieEnd = false;

	// 硬直中フラグ
	bool isStunned = false;

	//弱点硬直フラグ
	bool isWeakStun = false;

	// 硬直させる時間（フレーム）
	float stunFrame = 0.0f;

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

	// 硬直開始の共通処理
	void StartStun(float frame, bool isWeak);

	//弱点時のダメージ
	int WeakDamage(int damege) const;

};

