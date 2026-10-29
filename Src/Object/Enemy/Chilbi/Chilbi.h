#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"

#include "Wepon/Punch.h"

class Chilbi: public CharacterBase
{

public:

	Chilbi(Vector3 initPos);
	~Chilbi()override = default;


	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	enum class STATE
	{
		None = -1,

		Idle,

		Run,

		Attack,

		Hit,

		Die,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Run,

		Attack_1,
		Attack_2,

		Hit,

		Die,

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run

		0.7f,	// Attack_1
		0.7f,	// Attack_2

		0.6f,	// Hit
		
		0.8f	// Die
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	//Idle

		true,	//Run

		false,	//Attack_1
		false,	//Attack_2

		false,	//Hit

		false,	//Die


	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Enemy/Chilbi/Idle.mv1",	//Idle

		"Data/Model/Enemy/Chilbi/Run.mv1",	//Run

		"Data/Model/Enemy/Chilbi/Attack_1.mv1",	//Attack_1
		"Data/Model/Enemy/Chilbi/Attack_2.mv1",	//Attack_1

		"Data/Model/Enemy/Chilbi/Hit.mv1",	//Hit

		"Data/Model/Enemy/Chilbi/Die.mv1",	//Die

	};

#pragma endregion

	//初期読み込み
	void SubInit(void)override;
	//更新
	void SubUpdate(void)override;

	void SubDraw(void)override;

#pragma region 参照する変数

	//初期座標
	const Vector3& initPos;

#pragma endregion
	
	//パンチ
	Punch* punch;
	//状態
	STATE state;

	bool isClearRequested = false;

	// 無敵カウンター
	float invincibleCounter;

};

