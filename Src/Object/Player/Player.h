#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

class Sword;
class Gun;
class RangeAttack;
class SkillBase;

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

	//ロックオン対象の設定（GameSceneから毎フレーム設定）
	void SetLockOnTarget(bool isLock, const Vector3& pos) { isLockOn = isLock; lockOnPos = pos; }

private:


	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,

		Move,

		Jump,

		Roll,

		Hit,

		AttackNormal,
		AttackAir,
		AttackCharge,

		Gun,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Run,

		Roll,

		Hit,

		JumpStart,
		JumpIdle,
		JumpEnd,

		AttackNormal_1,
		AttackNormal_2,
		AttackNormal_3,

		AttackAir_Start,
		AttackAir_End,

		AttackCharge_Start,
		AttackCharge_End,

		Gun,

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run

		1.5f,   // Roll

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

		1.5f,	// Gun

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Run

		false,  // Roll

		false,  // Hit

		false,  //JumpStart
		true,	//JumpIdle
		false,  //JumpEnd

		false,	// AttackNormal_1
		false,	// AttackNormal_2
		false,	// AttackNormal_3

		false,	// AttackCharge_Start
		false,	// AttackCharge_End

		false,	// Gun

	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Player/Idle.mv1",	//Idle

		"Data/Model/Player/Run.mv1",	//Run

		"Data/Model/Player/Roll.mv1",	//Roll

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

		"Data/Model/Player/Gun.mv1",	//Gun

	};

#pragma endregion

#pragma region 定数

	//プレイヤーサイズ
	static constexpr float PLAYER_SCALE = 1.3f;

	//スキル枠の数
	static constexpr int SKILL_SLOT_MAX = 4;

#pragma endregion

#pragma region 変数

	Sword* sword;
	Gun* gun;
	RangeAttack* rangeAttack;
	//スキル枠（Shift+Q/E/R/F ・ LB+X/A/B/Y）
	SkillBase* skillSlot[SKILL_SLOT_MAX];

	//ロックオン中かどうか
	bool isLockOn;
	//ロックオン中の敵の位置
	Vector3 lockOnPos;

	STATE state;

	//武器切り替え
	bool gunIsActive;
	bool swordIsActive;

	//ジャンプ回数最大フラグ
	bool isMaxJump;
	//銃状態
	bool isGunState;

	float targetTime;
	//連続発射数
	int bulletShotCount;
	//現在所持段数
	int bullet;
	
	

#pragma endregion

	// 初期化処理
	void SubInit(void)override 
	{
		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

	//指定した枠のスキルを使う（ロックオン中は敵の方向を向く）
	void UseSkill(int slot);

	void SubDraw(void)override;

};