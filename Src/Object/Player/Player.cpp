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

#include "../Skill/SkillFire/SkillFire.h"

#include "../Enemy/EnemyBase.h"

#include "../../Manager/Input/InputManager.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerRollState.h"
#include "State/PlayerHitState.h"

#include "State/PlayerAttackNormalState.h"
#include "State/PlayerAttackAirState.h"
#include "State/PlayerAttackChargeState.h"

#include "State/PlayerGunState.h"	

Player::Player()
	:
	CharacterBase("Data/Parameter/Player/"),
	swordIsActive(true),
	gun(nullptr),
	sword(nullptr),
	rangeAttack(nullptr),
	skillSlot(),
	lockOnEnemyId(EnemyBase::INVALID_ID),
	findEnemy(),
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

	//フレーム取得
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
	gun = new Gun(trans); 

	gun->SetEnemyFinder([&](int id) { return findEnemy ? findEnemy(id) : nullptr; });
	gun->SetAddMp([this](int recovery) { AddMp(recovery); });

	AddChildActor(gun);

	//範囲攻撃の生成
	rangeAttack = new RangeAttack(trans);
	AddChildActor(rangeAttack);

	//スキル（ファイア）の生成
	SkillFire* skillFire = new SkillFire(trans, SkillBase::SKILL_USER::Player);
	AddChildActor(skillFire);
	//スキル1に設定（スキル2～4は空き）
	skillSlot[0] = skillFire;

	sword->ChangeActive(Sword::ACTIVE::DrawOnly);
	sword->ChangePosition(Sword::SWORD_POSITION::Back);

#pragma endregion


#pragma region 状態設定


#pragma region 状態設定：基本操作

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
			15.0f, 1.5f, 300,
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
			20.0f, velocity.y, isGround, isMaxJump,
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
			[&]() 
			{
				AnimePlay(ANIME_TYPE::Roll);
				//回避中は判定なし
				SetGravityFlg(false);
				SetJudgeFlg(false);
			},
			std::bind(&Player::IsAnimeEnd, this),
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			trans,
			[&]()
			{ 
				//当たり判定を戻す
				SetGravityFlg(true);
				SetJudgeFlg(true);
				ChangeState(STATE::Idle); 
			}
		)
	);
#pragma endregion


#pragma region 状態設定：攻撃関連

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
			std::bind(&Player::GetAnimeRatio, this)
		)
	);

	//空中攻撃状態
	AddState(
		STATE::AttackAir,
		new PlayerAttackAirState(
			isGround, isMaxJump,
			[&]()
			{
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);
				AnimePlay(ANIME_TYPE::AttackAir_Start);
			},
			[&]()
			{
				sword->ChangeActive(Sword::ACTIVE::DrawOnly);
				AnimePlay(ANIME_TYPE::AttackAir_End);
			},
			[&]()
			{
				//範囲攻撃発生
				rangeAttack->Start(RangeAttack::SCALE::Big);
			},
			[&]()
			{
				//範囲攻撃削除
				rangeAttack->End();
			},
			std::bind(&Player::GetAnimeRatio, this),
			std::bind(&Player::IsAnimeEnd, this),
			[&]()
			{
				rangeAttack->End();
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
				AnimePlay(ANIME_TYPE::Gun);
			},
			[&]()
			{
				//武器の切り替え
				gun->SetActive(false);
				sword->ChangeActive(Sword::ACTIVE::Active);
				ChangeState(STATE::Idle);
			},
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]()
			{
				gun->Shot();
			},
			10.0f,
			ACCEL_MAX,
			//ロックオンしているか（銃に入れる条件・構えを続ける条件）
			[&]() { return FindLockOnEnemy() != nullptr; }
		)
	);

	//HIT状態
	AddState(
		STATE::Hit,
		new PlayerHitState(
			[&]() {AnimePlay(ANIME_TYPE::Hit); },
			std::bind(&Player::IsAnimeEnd, this),
			std::bind(&Player::IsDead, this),
			[&]() {ChangeState(STATE::Idle); }
		)
	);

#pragma endregion


#pragma region 状態遷移制御登録

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

	// 「待機状態」->「HIT状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Hit);
	// 「移動状態」->「HIT状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Hit);
	// 「ジャンプ状態」->「HIT状態」の自動遷移登録
	RegisterStateTransition(STATE::Jump, STATE::Hit);
	// 「通常攻撃状態」->「HIT状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackNormal, STATE::Hit);
	// 「銃状態」->「HIT状態」の自動遷移登録
	RegisterStateTransition(STATE::Gun, STATE::Hit);

#pragma endregion

#pragma endregion


#pragma region ステータス設定・読み込み

	//読み込みファイル
	const std::string fileName = "PlayerStatus";

	// maxHpパラメーター名
	std::string parameterName = "MaxHp";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ MAX_HP =  GetParameterToInt(fileName, parameterName);}
	
	// maxHpパラメーター名
	parameterName = "MaxMp";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ MAX_MP = GetParameterToInt(fileName, parameterName);}

	//ステータス初期化
	InitStatus(MAX_HP, MAX_MP);


#pragma endregion


	
}

void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{

	case COLLIDER_TAG::Enemy_ChilbiPunch:
	{
		TakeDamage(1);
		ChangeState(IsDead() ? STATE::Hit : STATE::Hit);

		break;
	}

	//敵のスキル
	case COLLIDER_TAG::EnemySkill:
	{
		//スキル側のコライダーに設定されたダメージ
		TakeDamage(other.GetDamage());
		ChangeState(STATE::Hit);

		break;
	}

	}
}

void Player::TargetDirection(const Vector3& target)
{
	trans.SetRotation(Quaternion::FromRotationY(atan2f(target.x, target.z)));
}

void Player::SubUpdate(void)
{

	if (isGround)
	{
		isMaxJump = false;
	}

#pragma region 銃関連

	
	if(GetState() == (int)STATE::Gun)
	{
		//照準の更新処理
		gun->UpdateAim(FindLockOnEnemy() ? lockOnEnemyId : EnemyBase::INVALID_ID);
	}
	else
	{
		//構えていない時に照準リセット・弾の残弾数の回復
		gun->ResetAim();
		gun->UpdateRecover();

	}

	gun->UpdateHit();
	gun->FollowHand();


#pragma endregion



	//スキル発動（スキルメニューを押している間）
	const KEY_TYPE SKILL_KEYS[SKILL_SLOT_MAX] = {
		KEY_TYPE::PlayerSkill1,
		KEY_TYPE::PlayerSkill2,
		KEY_TYPE::PlayerSkill3,
		KEY_TYPE::PlayerSkill4,
	};
	for (int i = 0; i < SKILL_SLOT_MAX; ++i)
	{
		if (Input::GetIns().GetInfo(SKILL_KEYS[i]).down) { UseSkill(i); }
	}
}

void Player::UseSkill(int slot)
{
	//範囲外・空き枠・発動中なら何もしない
	if (slot < 0 || slot >= SKILL_SLOT_MAX) { return; }
	SkillBase* skill = skillSlot[slot];
	if (skill == nullptr || skill->IsActive()) { return; }

	//MPを支払う（足りなければ撃てない）
	if (!UseMp(skill->GetMpCost())) { return; }

	//ロックオン中なら敵の方向を向いてから撃つ
	if (const EnemyBase* enemy = FindLockOnEnemy())
	{
		Vector3 dir = enemy->GetTrans().pos - trans.pos;
		dir.y = 0.0f;  
		if (dir.LengthSq() > 0.000001f) { TargetDirection(dir); }
	}

	skill->Activate();
}

const EnemyBase* Player::FindLockOnEnemy(void) const
{
	if (!findEnemy) { return nullptr; }

	const EnemyBase* enemy = findEnemy(lockOnEnemyId);

	//既に死んでいる敵はロックオンしない
	if (enemy == nullptr || enemy->IsDead()) { return nullptr; }

	return enemy;
}

bool Player::IsAiming(void) const
{
	return GetState() == (int)STATE::Gun;
}

const SkillBase* Player::GetSkill(int slot) const
{
	//スキルがセットされていない場合は何もしない
	if (slot < 0 || slot >= SKILL_SLOT_MAX) { return nullptr; }

	return skillSlot[slot];
}

bool Player::IsSkillMenuOpen(void) const
{
	//スキルメニューを押している間
	return Input::GetIns().GetInfo(KEY_TYPE::PlayerSkillMenu).now;
}

void Player::SubDraw(void)
{

	if(GetState() == (int)STATE::Gun)
	{
	
		gun->DrawBulletUI();
		gun->DrawAimUI();
	}

	//DrawFormatString(0, 120, 0xffffff, "Pos : %.2f,%.2f,%.2f", trans.pos.x, trans.pos.y, trans.pos.z);

	//DrawFormatString(0, 240, 0xffffff, "HP : %d / %d  MP : %d / %d",
	//	status.hp, status.maxHp, status.mp, status.maxMp);


}





