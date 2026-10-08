#pragma once

#include "../Common/CharacterBase/CharacterBase.h"


class Player : public CharacterBase
{
public:
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

		NormalAttack,

		Roll,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Run,

		JumpStart,
		JumpIdle,
		JumpEnd,

		NormalAttack_1,
		NormalAttack_2,
		NormalAttack_3,

		Roll,

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run

		1.0f,	// JumpStart
		1.0f,	// JumpIdle
		1.0f,	// JumpEnd

		1.0f,	// NormalAttack1
		1.0f,	// NormalAttack2
		1.0f,	// NormalAttack3

		1.0f,   // Roll
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Run

		false,  //JumpStart
		true,	//JumpIdle
		false,  //JumpEnd

		false,	// NormalAttack1
		false,	// NormalAttack2
		false,	// NormalAttack3

		false,  // Roll
	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Player/Idle.mv1",//Idle

		"Data/Model/Player/Run.mv1",//Run

		"Data/Model/Player/Jump_Start.mv1",//JumpStart
		"Data/Model/Player/Jump_Idle.mv1",//JumpIdle
		"Data/Model/Player/Jump_End.mv1",//JumpEnd

		"Data/Model/Player/NormalAttack_1.mv1",//NomalAttack_1
		"Data/Model/Player/NormalAttack_2.mv1",//NomalAttack_2
		"Data/Model/Player/NormalAttack_3.mv1",//NomalAttack_3

		"Data/Model/Player/Roll.mv1",//Roll
	};
#pragma endregion

#pragma region 定数

	//プレイヤーサイズ
	static constexpr float PLAYER_SCALE = 1.3f;

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

	//アニメーション移動値無効
	void AnimeMoveControl(void);
};