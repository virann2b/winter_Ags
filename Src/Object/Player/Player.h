#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

#include "Wepon/Sword/Sword.h"
#include "Wepon/Gun/Gun.h"


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

private:


	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,
		Jump,
		Roll,

		NormalAttack,
		AirAttack,

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

		JumpStart,
		JumpIdle,
		JumpEnd,

		NormalAttack_1,
		NormalAttack_2,
		NormalAttack_3,

		AirAttack,

		Gun,

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run

		1.0f,   // Roll

		1.0f,	// JumpStart
		1.0f,	// JumpIdle
		1.0f,	// JumpEnd

		0.7f,	// NormalAttack1
		0.7f,	// NormalAttack2
		0.7f,	// NormalAttack3

		1.0f,	// AirAttack

		1.5f,	// Gun

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Run

		false,  // Roll

		false,  //JumpStart
		true,	//JumpIdle
		false,  //JumpEnd

		false,	// NormalAttack1
		false,	// NormalAttack2
		false,	// NormalAttack3

		false,	// AirAttack

		false,	// Gun

	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Player/Idle.mv1",//Idle

		"Data/Model/Player/Run.mv1",//Run

		"Data/Model/Player/Roll.mv1",

		"Data/Model/Player/Jump_Start.mv1",//JumpStart
		"Data/Model/Player/Jump_Idle.mv1",//JumpIdle
		"Data/Model/Player/Jump_End.mv1",//JumpEnd

		"Data/Model/Player/NormalAttack_1.mv1",//NomalAttack_1
		"Data/Model/Player/NormalAttack_2.mv1",//NomalAttack_2
		"Data/Model/Player/NormalAttack_3.mv1",//NomalAttack_3

		"Data/Model/Player/NormalAttack_1.mv1",//AirAttack

		"Data/Model/Player/Gun.mv1",//Gun

	};
#pragma endregion

#pragma region 定数

	//プレイヤーサイズ
	static constexpr float PLAYER_SCALE = 1.3f;

#pragma endregion

#pragma region 変数

	Sword* sword;
	Gun* gun;

	//武器切り替え
	bool gunIsActive;
	bool swordIsActive;

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

	void SubDraw(void)override;

	//アニメーション移動値無効
	void AnimeMoveControl(void);
};