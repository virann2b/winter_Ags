#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

#include <functional>


#include "Wepon/Sword/Sword.h"

class Gun;
class RangeAttack;
class SkillBase;
class EnemyBase;

class Player : public CharacterBase
{
public:

	//武器切り替え用
	enum class  BATTLE_MODE
	{
		None = -1,

		Sword,
		Gun,

		Max
	};

	Player();
	~Player()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;	
	// ロックオンしている敵の方向に向く
	void TargetDirection(const Vector3& targetPos);

	//ロックオン対象の番号を設定（GameSceneから毎フレーム設定。ロックオンしていなければ EnemyBase::INVALID_ID）
	void SetLockOnTarget(int enemyId) { lockOnEnemyId = enemyId; }

	//番号から敵を探す関数を設定（GameSceneから初期化時に設定）
	void SetEnemyFinder(std::function< EnemyBase*(int)> finder) { findEnemy = finder; }

	//銃を構えているか
	bool IsAiming(void) const;

	// スキルスロットの取得（読むだけ・空き枠はnullptr）
	const SkillBase* GetSkill(int slot) const;
	// スキルスロットの数
	static constexpr int GetSkillSlotMax(void) { return SKILL_SLOT_MAX; }
	// スキルメニューを開いているか（スキルキーを押している間）
	bool IsSkillMenuOpen(void) const;

	//銃の情報取得
	const Gun& GetGun(void) const { return *gun; }

private:


	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,			//待機

		Move,			//移動

		Jump,			//ジャンプ

		Roll,			//回避

		Guard,			//ガード

		Hit,			//被ダメージ

		AttackNormal,	//通常攻撃
		AttackAir,		//空中攻撃
		AttackCharge,	//強攻撃

		Gun,			//銃

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,				//待機

		Run,				//移動

		Roll,				//回避

		Guard,				//ガード　

		Hit,				//被ダメージ

		JumpStart,			//ジャンプ開始
		JumpIdle,			//ジャンプ中
		JumpEnd,			//ジャンプ終了

		AttackNormal_1,		//通常攻撃1
		AttackNormal_2,		//通常攻撃2
		AttackNormal_3,		//通常攻撃3

		AttackAir_Start,	//空中攻撃開始
		AttackAir_End,		//空中攻撃終了

		AttackCharge_Start,	//強攻撃タメ開始
		AttackCharge_End,	//強攻撃タメ終了

		AttackCharge_1,
		AttackCharge_2,
		AttackCharge_3,

		Gun,				//銃構え

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run

		1.5f,   // Roll

		1.0f,	// Guard

		0.5f,	// Hit

		1.0f,	// JumpStart
		1.0f,	// JumpIdle
		1.0f,	// JumpEnd

		0.7f,	// AttackNormal_1
		0.7f,	// AttackNormal_2
		0.7f,	// AttackNormal_3

		1.0f,	// AttackAir_Start
		1.0f,	// AttackAir_End

		1.0f,	// AttackCharge_Start
		1.0f,	// AttackCharge_End

		1.0f,	//AttackCharge_1
		1.0f,	//AttackCharge_2
		1.0f,	//AttackCharge_3

		1.5f,	// Gun

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Run

		false,  // Roll

		false,	// Guard

		false,  // Hit

		false,  //JumpStart
		true,	//JumpIdle
		false,  //JumpEnd

		false,	// AttackNormal_1
		false,	// AttackNormal_2
		false,	// AttackNormal_3

		false,	// AttackCharge_Start
		false,	// AttackCharge_End

		false,	//AttackCharge_1
		false,	//AttackCharge_2
		false,	//AttackCharge_3

		false,	// Gun

	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Player/Idle.mv1",	//Idle

		"Data/Model/Player/Run.mv1",	//Run

		"Data/Model/Player/Roll.mv1",	//Roll

		"Data/Model/Player/Guard.mv1",	//Guard

		"Data/Model/Player/Hit.mv1",	//Hit

		"Data/Model/Player/Jump_Start.mv1",		//JumpStart
		"Data/Model/Player/Jump_Idle.mv1",		//JumpIdle
		"Data/Model/Player/Jump_End.mv1",		//JumpEnd

		"Data/Model/Player/AttackNormal_1.mv1",	// AttackNormal_1
		"Data/Model/Player/AttackNormal_2.mv1",	// AttackNormal_2
		"Data/Model/Player/AttackNormal_3.mv1",	// AttackNormal_3

		"Data/Model/Player/AttackJump_1.mv1",	// AttackAir_Start
		"Data/Model/Player/AttackJump_2.mv1",	// AttackAir_End

		"Data/Model/Player/AttackCharge_1.mv1",	// AttackCharge_Start
		"Data/Model/Player/AttackCharge_2.mv1",	// AttackCharge_End

		"Data/Model/Player/ChargeAttack_Combo_1.mv1",	//AttackCharge_1
		"Data/Model/Player/ChargeAttack_Combo_2.mv1",	//AttackCharge_2
		"Data/Model/Player/ChargeAttack_Combo_3.mv1",	//AttackCharge_3

		"Data/Model/Player/Gun.mv1",	//Gun

	};

#pragma endregion

#pragma region 定数

	//プレイヤーサイズ
	static constexpr float PLAYER_SCALE = 1.3f;

	//スキル枠の数
	static constexpr int SKILL_SLOT_MAX = 4;

	//踏み込み時の最大移動速度
	static constexpr float ATTACK_STEP_ACCEL_MAX = 3.0f;

	//踏み込み判定距離
	static constexpr float ATTACK_STEP_STOP_DIST = 200.0f;

	//剣の怯みダメージ
	static constexpr int STAGGER_NORMAL = 5;		//通常攻撃
	static constexpr int STAGGER_CHARGE_COMBO = 25;	//強攻撃
	static constexpr int STAGGER_CHARGE = 50;		//タメ攻撃

	static constexpr int   GUARD_DAMAGE_MIN = 1;

	static constexpr int   GUARD_MP_COST = 3;

	//ガード時のダメージ倍率
	static constexpr float GUARD_DAMAGE_RATE = 0.5f;	

#pragma endregion

#pragma region 変数

	//剣
	Sword* sword;
	//銃
	Gun* gun;
	//範囲攻撃
	RangeAttack* rangeAttack;
	//スキル枠
	SkillBase* skillSlot[SKILL_SLOT_MAX];

	//ロックオン中の敵の番号
	int lockOnEnemyId;
	//番号から敵を探す関数
	std::function<EnemyBase*(int)> findEnemy;

	//武器切り替え
	bool swordIsActive;

	//ジャンプ回数最大フラグ
	bool isMaxJump;

#pragma endregion

	// 初期化処理
	void SubInit(void)override 
	{
		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}
	//更新
	void SubUpdate(void)override;
	//描画
	void SubDraw(void)override;

	//指定した枠のスキルを使う（ロックオン中は敵の方向を向く）
	void UseSkill(int slot);

	//ロックオン中の生存している敵を探す
	const EnemyBase* FindLockOnEnemy(void) const;

	//ロックオンしている敵に向く
	void FaceLockOnEnemy(void);

	//攻撃の踏み込み開始
	void AttackStepStart(void);
	//攻撃の踏み込み中の移動
	void AttackStepMove(void);

	// ステート切り替え時の処理
	void OnChangeState(int prevState, int nextState) override;

	// ステートに合わせて武器の基本の状態を決める
	void ApplyWeaponState(STATE state);
	// 剣の位置・剣の判定・銃の表示をまとめて設定
	void SetWeapon(Sword::SWORD_POSITION pos, Sword::ACTIVE active, bool isGunActive);
	// 剣の判定ON/OFF
	void SwordJudge(bool isOn);

};