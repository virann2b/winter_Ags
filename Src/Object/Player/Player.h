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

		Sword,	//剣
		Gun,	//銃

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

	//番号から敵を探す関数を設定
	void SetEnemyFinder(std::function< EnemyBase*(int)> finder) { findEnemy = finder; }

	//すべての敵を止める・再開する関数を設定
	void SetEnemyFreeze(std::function<void(bool)> freezer) { freezeAllEnemies = freezer; }

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

		BreakFinish,	//必殺技（一閃）

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

		Attack_Special_1,	//一閃用納刀

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

		0.9f,	// AttackNormal_1
		0.9f,	// AttackNormal_2
		0.9f,	// AttackNormal_3

		1.0f,	// AttackAir_Start
		1.0f,	// AttackAir_End

		1.0f,	// AttackCharge_Start
		1.0f,	// AttackCharge_End

		1.3f,	//AttackCharge_1
		1.3f,	//AttackCharge_2
		1.3f,	//AttackCharge_3


		0.7f,	//Attack_Special_1

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

		false,	//Attack_Special_1

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

		"Data/Model/Player/SheathingSword.mv1",	//Attack_Special_1

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

	
#pragma region 必殺技（一閃)

	//一閃を始める位置（敵からの距離）
	static constexpr float FINISH_START_DIST = 700.0f;			//一閃を始める位置（敵からの距離）	
	static constexpr float FINISH_PASS_DIST = 400.0f;			//敵の背後に抜ける距離
	static constexpr int   FINISH_DAMAGE = 30;					//ダメージ
	static constexpr float FINISH_HITSTOP_SCALE = 0.1f;			//到着時のヒットストップの速さ
	static constexpr unsigned short FINISH_HITSTOP_TIME = 20;	//到着時のヒットストップの長さ

	//一閃：構え中のカメラ（プレイヤーを敵側の斜め前から映す）
	static constexpr float FINISH_READY_CAM_FRONT = 270.0f;		//プレイヤーから前方への距離
	static constexpr float FINISH_READY_CAM_SIDE = 80.0f;		//横へのずらし
	static constexpr float FINISH_READY_CAM_HEIGHT = 210.0f;	//高さ
	static constexpr float FINISH_READY_LOOK_HEIGHT = 110.0f;	//プレイヤーのどの高さを見るか
	static constexpr float FINISH_READY_CAM_FOV = 50.0f;		//視野角（度）

	//一閃：カメラが元の位置に戻ったとみなす条件
	static constexpr float FINISH_RETURN_SETTLE_DIST = 1.0f;	//1フレームの移動量がこれ以下なら戻った
	static constexpr float FINISH_RETURN_MIN_FRAME = 10.0f;		//最低でも待つフレーム数
	static constexpr float FINISH_RETURN_MAX_FRAME = 90.0f;		//最大で待つフレーム数（保険）

	//一閃：移動中のカメラ
	//（プレイヤーの斜め後ろから敵とプレイヤーを映す）
	static constexpr float FINISH_PASS_CAM_BACK = 350.0f;		//プレイヤーから後方への距離
	static constexpr float FINISH_PASS_CAM_SIDE = 200.0f;		//横へのずらし（プレイヤーが画面右に来る向き）
	static constexpr float FINISH_PASS_CAM_HEIGHT = 150.0f;		//高さ
	static constexpr float FINISH_PASS_LOOK_RATE = 0.6f;		//プレイヤー→敵のどのあたりを見るか（0：プレイヤー、1：敵）
	static constexpr float FINISH_PASS_LOOK_HEIGHT = 100.0f;	//見る高さ
	static constexpr float FINISH_PASS_CAM_FOV = 60.0f;			//視野角（度）

	//一閃：演出を行う場所（ステージの中心）
	static constexpr float FINISH_ARENA_X = 0.0f;
	static constexpr float FINISH_ARENA_Z = 0.0f;
	//元の場所に戻すとき、ステージの外に出ないようにする範囲（±）
	static constexpr float FINISH_STAGE_LIMIT = 9700.0f;

#pragma endregion

#pragma region 空中攻撃

	//プレイヤーからの高さ
	static constexpr float AIR_CAM_HEIGHT = 600;
	//後ろへのずらし
	static constexpr float AIR_CAM_BACK = 150;
	//プレイヤーのどの高さを見るか
	static constexpr float AIR_CAM_LOOK_HEIGHT = 50;
	//視野角
	static constexpr float AIR_CAM_FOV = 60;
	//見下ろし位置へ移るフレーム数
	static constexpr float AIR_CAM_BLEND_FRAME = 10;

#pragma endregion



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
	//すべての敵を止める・再開する関数
	std::function<void(bool)> freezeAllEnemies;

	//武器切り替え
	bool swordIsActive;

	//ジャンプ回数最大フラグ
	bool isMaxJump;

	//必殺技の対象の敵
	int finishEnemyId;
	//一閃の開始点
	Vector3 finishStartPos;
	//一閃の到着点
	Vector3 finishTargetPos;
	//カメラが戻るのを待つ間の前フレームのカメラ座標
	Vector3 finishCamPrevPos;
	//カメラが戻るのを待った時間
	float finishReturnCounter;
	//一閃を受ける敵の元の位置（ステージ中心へ移す前）
	Vector3 finishEnemyOrigin;
	//一閃のためにステージ中心へ移しているか
	bool isFinishMoved_;

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

	//必殺技の入力チェック（敵が怯みチャンス中なら発動）
	void UpdateBreakFinishInput(void);
	//一閃の後、ステージ中心から元の場所へ戻す
	void RestoreFinishPosition(void);

	// ステート切り替え時の処理
	void OnChangeState(int prevState, int nextState) override;

	// ステートに合わせて武器の基本の状態を決める
	void ApplyWeaponState(STATE state);
	// 剣の位置・剣の判定・銃の表示をまとめて設定
	void SetWeapon(Sword::SWORD_POSITION pos, Sword::ACTIVE active, bool isGunActive);
	// 剣の判定ON/OFF
	void SwordJudge(bool isOn);

};