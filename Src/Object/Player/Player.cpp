#include "Player.h"

#include "../../Utility/Utility.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "../Common/AnimationController/AnimationController.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

#include "Wepon/Sword/Sword.h"
#include "Wepon/Gun/Gun.h"
#include "Wepon/RangeAttack/RangeAttack.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerRollState.h"

#include "State/PlayerAttackNormalState.h"
#include "State/PlayerAttackAirState.h"
#include "State/PlayerAttackChargeState.h"

#include "State/PlayerGunState.h"	

Player::Player()
	:
	CharacterBase(),
	gunIsActive(false),
	swordIsActive(true),
	gun(nullptr),
	sword(nullptr),
	rangeAttack(nullptr),
	targetTime(0.0f),
	bulletShotCount(0),
	bullet(0),
	isGunState(false),
	state(STATE::None)
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
	trans.centerDiff = Vector3(0.0f, -82.775f, 0.0f) * trans.scale;

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
	

	anime->SetPositionCorrectionFrame("smartrig:Hips");
	anime->SetPositionCorrection(true);
#pragma endregion


#pragma region コライダーの生成

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			Vector3::Yonly(55.0f) * trans.scale,
			Vector3::Yonly(-55.0f) * trans.scale,
			20.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	//剣の生成
	sword = new Sword(trans,swordIsActive);
	AddChildActor(sword);

	//銃の生成
	gun = new Gun(trans,gunIsActive,bullet);
	AddChildActor(gun);

	//範囲攻撃の生成
	rangeAttack = new RangeAttack(trans);
	AddChildActor(rangeAttack);

	sword->ChangeActive(Sword::ACTIVE::DrawOnly);
	sword->ChangePosition(Sword::SWORD_POSITION::Back);

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new PlayerIdleState(
			[&]() { AnimePlay(ANIME_TYPE::Idle); },
			[&]() { sword->ChangeActive(Sword::ACTIVE::DrawOnly); }
		)
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
			24.0f, velocity.y, isGround,isMaxJump,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]() { AnimePlay(ANIME_TYPE::JumpStart); },
			[&]() { AnimePlay(ANIME_TYPE::JumpIdle); },
			[&]() { AnimePlay(ANIME_TYPE::JumpEnd); },
			std::bind(&Player::IsAnimeEnd, this),
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	//回避状態
	AddState(
		STATE::Roll,
		new PlayerRollState(
			[&]() {AnimePlay(ANIME_TYPE::Roll); },
			std::bind(&Player::IsAnimeEnd, this),
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			trans,
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	//攻撃状態
	AddState(
		STATE::AttackNormal,
		new PlayerAttackNormalState(
			[&]() 
			{
				ChangeState(STATE::Idle);
				sword->ChangePosition(Sword::SWORD_POSITION::Back);

			},
			[&]() {sword->ChangeActive(Sword::ACTIVE::Active); },
			[&]()
			{
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);
				AnimePlay(ANIME_TYPE::AttackNormal_1);
			},
			[&]() {AnimePlay(ANIME_TYPE::AttackNormal_2); },
			[&]() {AnimePlay(ANIME_TYPE::AttackNormal_3); },
			std::bind(&Player::IsAnimeEnd, this),
			std::bind(&Player::GetAnimeRatio,this)
		)
	);

	//空中攻撃状態
	AddState(
		STATE::AttackAir,
		new PlayerAttackAirState(
			isGround,isMaxJump,
			[&]()
			{
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);
				AnimePlay(ANIME_TYPE::AttackAir_Start); 
			},
			[&]() 
			{
				sword->ChangeActive(Sword::ACTIVE::Active);
				AnimePlay(ANIME_TYPE::AttackAir_End); 
			},
			std::bind(&Player::IsAnimeEnd, this),
			[&]() 
			{
				ChangeState(STATE::Idle); 
				sword->ChangePosition(Sword::SWORD_POSITION::Back);

			}

		)
	);
	
	AddState(
		STATE::AttackCharge,
		new PlayerAttackChargeState(
			[&]() 
			{
				sword->ChangeActive(Sword::ACTIVE::DrawOnly);
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);

				AnimePlay(ANIME_TYPE::AttackCharge_Start);
			},
			[&]() 
			{
				AnimePlay(ANIME_TYPE::AttackCharge_End); 
				sword->ChangeActive(Sword::ACTIVE::Active);

			},
			std::bind(&Player::IsAnimeEnd, this),
			[&]()
			{
				ChangeState(STATE::Idle);
				sword->ChangePosition(Sword::SWORD_POSITION::Back);
			}
		)
	);

	// GUN状態
	AddState(
		STATE::Gun,
		new PlayerGunState(
			[&]()
			{ 
				//武器の切り替え
				sword->ChangeActive(Sword::ACTIVE::DrawOnly);
				gun->SetActive(true);
				isGunState = true;

				AnimePlay(ANIME_TYPE::Gun);
			},
			[&]()
			{ 
				//武器の切り替え
				gun->SetActive(false);
				sword->ChangeActive(Sword::ACTIVE::Active);
				isGunState = false;


				ChangeState(STATE::Idle); 
			},
			targetTime,
			bulletShotCount,
			[&]()
			{
				gun->Shot(1);
			}
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

	// 「待機状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Roll);
	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Roll);

	// 「待機状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackNormal);
	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::AttackNormal);

	// 「待機状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackCharge);
	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::AttackCharge);

	// 「待機状態」->「銃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Gun);
	// 「待機状態」->「銃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Gun);

	// 「ジャンプ状態」->「空中攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Jump, STATE::AttackAir);

#pragma endregion
}

void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	
}

void Player::SubUpdate(void)
{
	if (isGround)
	{
		isMaxJump = false;
	}

	if (state == STATE::Gun)
	{
		isGunState = true;
	}
	else
	{
		isGunState = false;
	}

	//弾の更新
	if (!isGunState)
	{
		gun->BulletUpdate();
	}

	gun->BulletMoveUpdate();
}

void Player::SubDraw(void)
{

	gun->BulletDraw();

	//DrawFormatString(0, 120, 0xffffff, "Pos : %.2f,%.2f,%.2f", trans.pos.x, trans.pos.y, trans.pos.z);
	//DrawFormatString(0, 150, 0xffffff, "time : %.2f", targetTime);
	/*DrawFormatString(0, 220, 0xffffff, "isMaxJump : %d", isMaxJump);
	DrawFormatString(0, 240, 0xffffff, "isGround : %d", isGround);*/

	DrawString(0, 120, "操作：WASD", 0xffffff);
	DrawString(0, 140, "ジャンプ：スペース【最大２回】", 0xffffff);
	DrawString(0, 160, "回避：Rキー", 0xffffff);
	DrawString(0, 180, "ロックオン：Eキー", 0xffffff);
	DrawString(0, 200, "通常攻撃：左クリック", 0xffffff);

	DrawString(0, 240, "【 銃モード 】構え：右クリック長押し", 0xffffff);
	DrawString(0, 260, "【 銃モード 】発射：左クリック (弾に当たり判定はないです）", 0xffffff);
	DrawFormatString(0, 280, 0xffffff, "【 銃モード 】残弾数(右クリック長押しで増加最大6発) : %d", bulletShotCount);
	DrawFormatString(0, 300, 0xffffff, "【 銃モード 】弾所持数 : %d", bullet);

	DrawString(0, 340, "タメ攻撃：Fキー長押し【タメ】離すと【攻撃】", 0xffffff);
	DrawString(0, 360, "空中攻撃：ジャンプ２回目中に通常攻撃　", 0xffffff);

}



