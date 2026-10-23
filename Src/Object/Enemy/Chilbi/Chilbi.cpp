#include "Chilbi.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/TimeScale/TimeScale.h"
#include "../../../Scene/SceneManager.h"

#include "../../Common/Collider/CapsuleCollider.h"

#include "State/ChilbiIdleState.h"
#include "State/ChilbiHitState.h"
#include "State/ChilbiDieState.h"



Chilbi::Chilbi(Vector3 initPos)
	:
	initPos(initPos)
{
}

void Chilbi::Load(void)
{


#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);
	// 押し出しだしによる重みを設定
	SetPushWeight(60);

	//当たり判定有効にする
	SetJudgeFlg(true);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Enemy/Chilbi/Chilbi");

	// モデルのスケール設定
	trans.scale = 1.5;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	//初期座標登録
	trans.pos = initPos;

	hp = 10;

#pragma endregion

#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	//個別のアニメーション読み込み
	for (int i = 0; i < (int)ANIME_TYPE::Max; ++i)
	{
		AddAnimation(i, ANIME_SPEED_TABLE[i], ANIME_LOOP_TABLE[i], ANIME_PATH_TABLE[i]);
	}

#pragma endregion

#pragma region コライダーの生成

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Enemy,
			Vector3::Yonly(60.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			40.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion

#pragma region 状態設定

	//待機状態
	AddState(
		STATE::Idle,
		new ChilbiIdleState(
			[&]() { AnimePlay((int)ANIME_TYPE::Idle); }
		)
	);

	//HIT状態
	AddState(
		STATE::Hit,
		new ChilbiHitState(
			[&]() { AnimePlay((int)ANIME_TYPE::Hit); },
			[&]() { ChangeState(STATE::Idle); },
			std::bind(&Chilbi::IsAnimeEnd, this)
		)
	);

	//死亡状態
	AddState(
		STATE::Die,
		new ChilbiDieState(
			[&]() { AnimePlay((int)ANIME_TYPE::Die); },
			std::bind(&Chilbi::GetAlive, this),
			std::bind(&Chilbi::IsAnimeEnd, this)

		)
	);



#pragma endregion

}

void Chilbi::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (invincibleCounter > 0) { return; }

	switch (other.GetTag())
	{

	case COLLIDER_TAG::Sword:
	{
		TakeDamage(1);
		ChangeState(GetAlive() ? STATE::Die : STATE::Hit);

		invincibleCounter = 60;
	}
		break;
	default:
		break;
	}
}

void Chilbi::SubInit(void)
{
	//初期状態
	ChangeState(STATE::Idle);

	invincibleCounter = 0.0f;

	isClearRequested = false;
}

void Chilbi::SubUpdate(void)
{
	if (invincibleCounter > 0.0f) { invincibleCounter -= TimeScale::Get(); }
	else { invincibleCounter = 0.0f; }

	// 死亡アニメが終わったらゲームクリア
	if (GetAlive() && GetState() == (int)STATE::Die && IsAnimeEnd() && !isClearRequested)
	{
		isClearRequested = true;
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
	}
}

void Chilbi::SubDraw(void)
{
	DrawFormatString(0, 80, 0xffffff, "EnemyHp : %d", hp);

}
