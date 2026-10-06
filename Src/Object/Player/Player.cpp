#include "Player.h"

#include "../../Utility/Utility.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

#include "Wepon/Sword/Sword.h"
#include "Wepon/Gun/Gun.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerRollState.h"

Player::Player() : CharacterBase()
{
}

void Player::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(50);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Player");

	// モデルのスケール設定
	trans.scale = PLAYER_SCALE;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	// シェーダー登録
	//CreateShader(new DefaultShader());

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
			COLLIDER_TAG::Player,
			Vector3::Yonly(60.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			60.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	Sword* sword = new Sword(trans);
	AddChildActor(sword);

	Gun* gun = new Gun(trans);
	AddChildActor(gun);

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new PlayerIdleState([&]() { AnimePlay(ANIME_TYPE::Idle); })
	);

	// 移動状態
	AddState(
		STATE::Move,
		new PlayerMoveState(
			10.0f, 1.5f, 300,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			[&]() { AnimePlay(ANIME_TYPE::Run); },
			[&]() { AnimePlay(ANIME_TYPE::Run); }
		)
	);

	// ジャンプ状態
	AddState(
		STATE::Jump,
		new PlayerJumpState(
			24.0f, velocity.y, isGround,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]() { AnimePlay(ANIME_TYPE::JumpStart); },
			[&]() { AnimePlay(ANIME_TYPE::JumpIdle); },
			[&]() { AnimePlay(ANIME_TYPE::JumpEnd); },
			std::bind(&Player::IsAnimeEnd, this),
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	AddState(
		STATE::Roll,
		new PlayerRollState(
			[&]() {ChangeState(STATE::Idle); },
			[&]() {AnimePlay(ANIME_TYPE::Roll); },
			std::bind(&Player::IsAnimeEnd, this)
		)
	);
	

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);

	// 「待機状態」->「ジャンプ状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Jump);
	// 「移動状態」->「ジャンプ状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Jump);

	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Roll);
	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Roll);


#pragma endregion
}

void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Player::SubUpdate(void)
{
	/*static char type = 0;
	static bool prev = false, now = false;

	prev = now;
	now = CheckHitKey(KEY_INPUT_SPACE) == 1;

	if (!prev && now) {

		if (++type > 1) { type = 0; }

		switch (type){
		case 0: { CreateShader(new DefaultShader()); break; }
		case 1: { CreateShader(new RimLightShader()); break; }
		}
	}*/
}