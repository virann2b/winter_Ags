#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
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

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Run,

		Max
	};

	// アニメーション再生速度テーブル
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		0.8f,	// Run
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Run

	};

	// アニメーションパステーブル
	const char* ANIME_PATH_TABLE[(int)ANIME_TYPE::Max] =
	{
		"Data/Model/Enemy/Chilbi/Idle.mv1",//Idle

		"Data/Model/Enemy/Chilbi/Run.mv1",//Run

	};
#pragma endregion

	void SubInit(void)override;
	void SubUpdate(void)override;



#pragma region 参照する変数

	//初期座標
	const Vector3& initPos;

#pragma endregion

};

