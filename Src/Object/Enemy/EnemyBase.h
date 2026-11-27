#pragma once
#include "../Common/CharacterBase/CharacterBase.h"
#include "../Skill/ElementDefine.h"

#include <vector>

class SkillBase;
class WeakGlowShader;

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
	bool IsStunned(void) const { return isStunned_; }
	//　弱点硬直判定
	bool IsWeakStun(void) const { return isWeakStun_; }

	// 攻撃対象の座標を設定
	void SetTargetPos(const Vector3* targetPos) { this->targetPos = targetPos; }

	// 死亡アニメーションまで終わったか（画面から消えたか）
	bool IsDieEnd(void) const { return isDieEnd_; }

	// 敵の番号（EnemyManagerが生成時に振る。同じ番号は二度と使われない）
	int GetId(void) const { return id; }

	//頭上にUIを出す位置
	Vector3 GetUIPos(void) const { return trans.pos + Vector3::Yonly(uiOffsetY); }

	//硬直時間の残り割合
	float GetWeakStunRate(void) const;

	//弱点硬直を起こした
	ELEMENT GetWeakStunElement(void) const { return weakStunElement; }

	// 今の怯み値（確認用）
	float GetStagger(void) const { return stagger; }
;
	//必殺技を発動できるか
	bool IsBreakChance(void) const { return breakChanceCounter > 0.0f; }
	//受付終了
	void EndBreakChance(void) { breakChanceCounter = 0.0f; }

	//必殺技の開始（行動・アニメーションを止めてその場で固める）
	void StartBreakFinished(void) { isBreakFinished_ = true; isFinishHold_ = true; velocity = Vector3(); OnBreakFinished(); }
	//斬られた瞬間：アニメーションだけ動かす（のけぞり・死亡を見せる。行動はまだ止めたまま）
	void ReleaseFinishFreeze(void) { isBreakFinished_ = false; }
	//必殺技の終了（演出が終わったら行動再開）
	void EndBreakFinished(void) { isBreakFinished_ = false; isFinishHold_ = false; }
	//必殺技の演出用に位置を移す（ステージ中心への移動・元の場所への復帰）
	void WarpTo(const Vector3& pos) { trans.pos = pos; }

	//必殺技のダメージを受ける
	void TakeFinishDamage(int damage);



protected:

	//派生先で専用の状態へ
	virtual void OnBreakFinished(void) {}

	//必殺技の演出が終わるまでは行動停止（ステートが止まる）
	bool IsActionStop(void) const override { return isFinishHold_; }
	//斬られるまではアニメーションも停止（その場で固まる）
	bool IsAnimeStop(void) const override { return isBreakFinished_; }

#pragma region 定数

	// 無敵時間
	static constexpr float DEFAULT_INVINCIBLE_FRAME = 60.0f;

	// 攻撃クールタイム
	static constexpr float DEFAULT_ATTACK_COOLTIME = 120.0f;

	// 弱点硬直の時間
	static constexpr float WEAK_STUN_FRAME = 360.0f;

	// 弱点硬直中の被ダメージ倍率
	static constexpr float WEAK_DAMAGE_RATE = 1.5f;

	// 距離の区分
	static constexpr float NEAR_RANGE = 200.0f;
	static constexpr float MIDDLE_RANGE = 500.0f;
	static constexpr float LONG_RANGE = 900.0f;

	// 停止距離
	static constexpr float DEFAULT_STOP_RANGE = 200.0f;

	// 弱点の反動（左右の揺れ）
	static constexpr float SHAKE_FRAME = 15.0f;		// 揺れる時間（フレーム）
	static constexpr float SHAKE_WIDTH = 20.0f;		// 最初の揺れ幅
	static constexpr float SHAKE_COUNT = 3.0f;		// 揺れる間の往復回数

	// 怯み値
	static constexpr float DEFAULT_STAGGER_MAX = 100.0f;			// 最大値

	// 体勢崩れ中は怯み値を削らない時間
	static constexpr float BREAK_GUARD_FRAME = 60.0f;

	// 体勢崩れのスロー（仮）
	static constexpr float BREAK_SLOW_SCALE = 0.1f;			// 速さ
	static constexpr unsigned short BREAK_SLOW_TIME = 60;	// 長さ

	// 怯み値が残っているときの反動の大きさ
	static constexpr float SMALL_SHAKE_RATE = 0.4f;

	//必殺技の入力受付時間（スロー時間と同じ）
	static constexpr float BREAK_CHANCE_FRAME = BREAK_SLOW_TIME;

	//当てた瞬間の強い光が消えるまでのフレーム数
	static constexpr float WEAK_GLOW_FLASH_FRAME = 20.0f;
	//弱点硬直中の光の強さ（基本）
	static constexpr float WEAK_GLOW_STUN_BASE = 0.35f;
	//弱点硬直中の明滅の幅
	static constexpr float WEAK_GLOW_STUN_PULSE = 0.15f;
	//弱点硬直中の明滅の速さ（1フレームに進む角度・ラジアン）
	static constexpr float WEAK_GLOW_PULSE_SPEED = 0.15f;

#pragma endregion

#pragma region 変数

	// UIを出す高さ
	float uiOffsetY = 200.0f;

	// カウント用攻撃クールタイム
	float attackCoolTime;

	//受付残り時間
	float breakChanceCounter = 0.0f;	
	//必殺技を受けている最中か（斬られるまで。アニメーションも止める）
	bool isBreakFinished_ = false;		
	//必殺技の演出が終わるまで行動を止めるか
	bool isFinishHold_ = false;

	//最後に受けた攻撃属性
	ELEMENT lastHitElement = ELEMENT::None;

	// 弱点硬直を起こした属性
	ELEMENT weakStunElement = ELEMENT::None;

	//発光シェーダー
	WeakGlowShader* weakGlowShader = nullptr;
	//光らせる属性
	ELEMENT weakGlowElement = ELEMENT::None;
	//当てた瞬間の強い光（1→0）
	float weakGlowFlash = 0.0f;
	//明滅用の時間
	float weakGlowTime = 0.0f;


#pragma endregion

#pragma region 弱点を突かれた時の発光

	//発光シェーダーの生成
	void CreateWeakGlowShader(void);
	//弱点を突かれた時に光らせ始める
	void StartWeakGlow(ELEMENT element);
	//光の強さの更新
	void UpdateWeakGlow(void);
	//属性ごとの光の色
	static Vector3 GetElementGlowColor(ELEMENT element);

#pragma endregion

	// オブジェクト初期設定
	void SetupEnemyObject(int pushWeight);

	// 初期化
	virtual void EnemyInit(void) {}
	// 更新
	virtual void EnemyUpdate(void) {}
	// 描画
	virtual void EnemyDraw(void) {}

	// 被弾時
	virtual void OnHit(void) = 0;

	// 死亡時
	virtual void OnDie(void) = 0;

	// 硬直開始時
	virtual void OnStun(void) {}

	// 硬直させる時間
	float GetStunFrame(void) const { return stunFrame; }

	// 残り硬直時間
	float GetStunRemain(void) const { return stunRemain; }
	// 硬直時間減少
	void CountStun(float frame);
;
	//硬直終了
	void EndStun(void) { isStunned_ = false; isWeakStun_ = false; }


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

	//ヒットエフェクト位置設定
	void SetHitEffectPos(Vector3 setPos){ hitEffectPos = setPos; }


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
	void LoadResistTable(const std::string& enemyName);

	// 怯み値をCSVから読み込む（派生先のLoadで呼ぶ）
	// 並び：最大値, 回復が始まるまで（フレーム）, 1フレームの回復量
	void LoadStagger(const std::string& enemyName);


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

	// HITエフェクト調整位置
	Vector3 hitEffectPos;
	
	// 攻撃対象の座標
	const Vector3* targetPos = nullptr;

	// 死亡アニメーションフラグ
	bool isDieEnd_ = false;

	// 硬直中フラグ
	bool isStunned_ = false;

	//弱点硬直フラグ
	bool isWeakStun_ = false;

	// 硬直させる時間
	float stunFrame = 0.0f;

	// 硬直の残り時間
	float stunRemain = 0.0f;

	// 反動の残り時間
	float shakeCounter = 0.0f;


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

	// 反動を開始する（rate：弱点のときを1とした大きさ）
	void StartShake(float rate = 1.0f) { shakeCounter = SHAKE_FRAME; shakeRate = rate; }

	// 今の反動の大きさ
	float shakeRate = 1.0f;

	// 怯み値
	float stagger = DEFAULT_STAGGER_MAX;
	float staggerMax = DEFAULT_STAGGER_MAX;

	// 回復が始まるまでの残り時間
	float staggerWaitCounter = 0.0f;
	// 体勢崩れ中の残り時間（この間は怯み値を削らない）
	float breakCounter = 0.0f;

	// 怯み値を削る
	bool ReduceStagger(int damage);
	// 怯み値の回復
	void UpdateStagger(void);

	// 体勢崩れ
	void BreakStance(void);
	//崩れた時の共有処理
	void OnStaggerBreak(void);

	// 反動の更新（見た目のズレを計算する）
	void UpdateShake(void);

};

