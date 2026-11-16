#include "Player.h"

#include "../../Utility/Utility.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "../Common/AnimationController/AnimationController.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

#include "Wepon/Gun/Gun.h"
#include "Wepon/RangeAttack/RangeAttack.h"

#include "../Skill/SkillFire/SkillFire.h"
#include "../Skill/SkillIce/SkillIce.h"


#include "../Enemy/EnemyBase.h"

#include "../../Manager/Input/InputManager.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerRollState.h"
#include "State/PlayerHitState.h"
#include "State/PlayerGuardState.h"

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
	findEnemy()
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
	SetPushWeight(0);

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
	SkillIce* skillIce = new SkillIce(trans, SkillBase::SKILL_USER::Player);

	AddChildActor(skillFire);
	AddChildActor(skillIce);


	//スキル1に設定（スキル2～4は空き）
	skillSlot[0] = skillFire;
	skillSlot[1] = skillIce;
	skillSlot[2] = skillFire;
	skillSlot[3] = skillIce;

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

	//ガード状態
	AddState(
		STATE::Guard,
		new PlayerGuardState(
			[&]() 
			{ 
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);
				AnimePlay(ANIME_TYPE::Guard);
			},
			std::bind(&Player::IsAnimeEnd, this),
			[&]() { ChangeState(STATE::Idle); }
		)
	);
#pragma endregion


#pragma region 状態設定：攻撃関連

	//攻撃状態
	AddState(
		STATE::AttackNormal,
		new PlayerAttackNormalState(
			[&]() { ChangeState(STATE::Idle);},
			[&]() { AnimePlay(ANIME_TYPE::AttackNormal_1);},
			[&]() { AnimePlay(ANIME_TYPE::AttackNormal_2); },
			[&]() { AnimePlay(ANIME_TYPE::AttackNormal_3); },
			std::bind(&Player::IsAnimeEnd, this),
			std::bind(&Player::GetAnimeRatio, this),
			std::bind(&Player::AttackStepStart,this),
			std::bind(&Player::AttackStepMove, this)
		)
	);

	//空中攻撃状態
	AddState(
		STATE::AttackAir,
		new PlayerAttackAirState(
			isGround, isMaxJump,
			[&]()
			{
				AnimePlay(ANIME_TYPE::AttackAir_Start);
			},
			[&]()
			{
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
			}

		)
	);

	AddState(
		STATE::AttackCharge,
		new PlayerAttackChargeState(
			[&](){ AnimePlay(ANIME_TYPE::AttackCharge_Start);},
			[&]()
			{ 
				AnimePlay(ANIME_TYPE::AttackCharge_End);
				FaceLockOnEnemy();
				sword->SetStagger(STAGGER_CHARGE);
			},
			[&](){ AnimePlay(ANIME_TYPE::AttackCharge_1);},
			[&](){ AnimePlay(ANIME_TYPE::AttackCharge_2); },
			[&](){ AnimePlay(ANIME_TYPE::AttackCharge_3); },
			std::bind(&Player::GetAnimeRatio, this),
			std::bind(&Player::SwordJudge, this, std::placeholders::_1),
			std::bind(&Player::IsAnimeEnd, this),
			[&](){ ChangeState(STATE::Idle);}
		)
	);

	// GUN状態
	AddState(
		STATE::Gun,
		new PlayerGunState(
			[&]() { AnimePlay(ANIME_TYPE::Gun);},
			[&]() { ChangeState(STATE::Idle);},
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]() { gun->Shot();},
			10.0f,
			ACCEL_MAX,
			//ロックオンしているか
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

	// 「待機状態」->「ガード状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Guard);
	// 「移動状態」->「ガード状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Guard);

	// 「待機状態」->「通常攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackNormal);
	// 「移動状態」->「通常攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::AttackNormal);

	// 「待機状態」->「強攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackCharge);
	// 「移動状態」->「強攻撃状態」の自動遷移登録
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
	FaceLockOnEnemy();

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

void Player::FaceLockOnEnemy(void)
{
	const EnemyBase* enemy = FindLockOnEnemy();
	if (enemy == nullptr) { return; }

	Vector3 dir = enemy->GetTrans().pos - trans.pos;
	dir.y = 0.0f;

	if (dir.LengthSq() > 0.000001f) { TargetDirection(dir); }
}

void Player::AttackStepStart(void)
{
	//踏み込みの速さ
	ACCEL_MAX = ATTACK_STEP_ACCEL_MAX;

	FaceLockOnEnemy();

}

void Player::AttackStepMove(void)
{
	//ロックオンしている場合
	if (const EnemyBase* enemy = FindLockOnEnemy())
	{
		//敵との距離を取得
		Vector3 toEnemy = enemy->GetTrans().pos - trans.pos;
		toEnemy.y = 0.0f;
		//距離が近い場合は何もしない
		if (toEnemy.Length() <= ATTACK_STEP_STOP_DIST) { return; }
	}

	//正面に進む
	Vector3 forward = trans.VTrans(Vector3(0.0f, 0.0f, 1.0f));
	forward.y = 0.0f;

	forward.Normalize();
	MoveAccel(forward);
}

void Player::OnChangeState(int prevState, int nextState)
{
	//次の状態に合わせて武器の状態を変更する
	ApplyWeaponState(static_cast<STATE>(nextState));
}

void Player::ApplyWeaponState(STATE state)
{
	//剣の位置
	using POS = Sword::SWORD_POSITION;
	//剣の当たり判定
	using ACT = Sword::ACTIVE;

	switch (state)
	{
	case Player::STATE::Guard:
	{
		SetWeapon(POS::Guard, ACT::DrawOnly, false);
		break;
	}
	case Player::STATE::AttackNormal:
	{
		SetWeapon(POS::Hand, ACT::Active, false);
		sword->SetStagger(STAGGER_NORMAL);
		break;
	}
	case Player::STATE::AttackAir:
	{
		SetWeapon(POS::Hand, ACT::DrawOnly, false);

		break;
	}
	case Player::STATE::AttackCharge:
	{
		//判定ステート内で切り替え
		SetWeapon(POS::Hand, ACT::DrawOnly, false);
		sword->SetStagger(STAGGER_CHARGE_COMBO);

		break;
	}
	case Player::STATE::Gun:
	{
		//判定ステート内で切り替え
		SetWeapon(POS::Back, ACT::DrawOnly, true);
		break;
	}
	default:
	{
		SetWeapon(POS::Back, ACT::DrawOnly, false);
		break;
	}
	}
}

void Player::SetWeapon(Sword::SWORD_POSITION pos, Sword::ACTIVE active, bool isGunActive)
{
	//剣の位置・描画変更
	sword->ChangePosition(pos);
	sword->ChangeActive(active);
	//銃の描画変更
	gun->SetActive(isGunActive);
}

void Player::SwordJudge(bool isOn)
{
	if (isOn)
	{
		sword->ChangeActive(Sword::ACTIVE::Active);
	}
	else
	{
		sword->ChangeActive(Sword::ACTIVE::DrawOnly);
	}
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
	//銃状態の時のみ描画を行う
	if(GetState() == (int)STATE::Gun)
	{
		gun->DrawAimUI();
	}

}





