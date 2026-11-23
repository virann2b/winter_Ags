#include "Player.h"

#include <algorithm>

#include "../../Utility/Utility.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "../Common/AnimationController/AnimationController.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

#include "../../Manager/Sound/SoundManager.h"

#include "Wepon/Gun/Gun.h"
#include "Wepon/RangeAttack/RangeAttack.h"

#include "../Skill/SkillFire/SkillFire.h"
#include "../Skill/SkillIce/SkillIce.h"

#include "../Enemy/EnemyBase.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/TimeScale/TimeScale.h"
#include "../../Manager/Camera/CurrentCamera.h"
#include "../../Manager/Camera/CameraEvent/FixedCameraEvent/FixedCameraEvent.h"
#include "../../Manager/Camera/CameraEvent/FollowOverheadCameraEvent/FollowOverheadCameraEvent.h"

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
#include "State/PlayerBreakFinishState.h"

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
	finishEnemyId(EnemyBase::INVALID_ID),
	finishStartPos(),
	finishTargetPos(),
	finishCamPrevPos(),
	finishReturnCounter(0.0f),
	finishEnemyOrigin(),
	isFinishMoved_(false)
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

	//銃の生成
	gun = new Gun(trans);

	gun->SetEnemyFinder([&](int id) { return findEnemy ? findEnemy(id) : nullptr; });
	gun->SetAddMp([this](int recovery) { AddMp(recovery); });

	AddChildActor(gun);


	//剣の生成
	sword = new Sword(trans,swordIsActive);
	AddChildActor(sword);
	//パラメータ追加
	ApplyAttackParam("SwordNormal", COLLIDER_TAG::PlayerSword);


	//範囲攻撃の生成
	rangeAttack = new RangeAttack(trans);
	AddChildActor(rangeAttack);
	//パラメータ追加
	ApplyAttackParam("RangeAttack", COLLIDER_TAG::PlayerLargeAttack);

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

	//剣の描画切り替え
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
			[&]() { AnimePlay(ANIME_TYPE::JumpStart);},
			[&]() { AnimePlay(ANIME_TYPE::JumpIdle); },
			[&]() 
			{ 

				Snd::GetIns().Play("Jump_End");
				AnimePlay(ANIME_TYPE::JumpEnd);
			},
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
				//ファイアサウンド再生
				Snd::GetIns().Play("Roll");
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
			[&]()
			{ 
				AnimePlay(ANIME_TYPE::AttackNormal_1);
				Snd::GetIns().Play("SwordSwing");

			},
			[&]() 
			{ 
				AnimePlay(ANIME_TYPE::AttackNormal_2);
				Snd::GetIns().Play("VC_NormalCombo_1");
				Snd::GetIns().Play("SwordSwing");


			},
			[&]()
			{ 
				AnimePlay(ANIME_TYPE::AttackNormal_3);
				Snd::GetIns().Play("VC_NormalCombo_2");
				Snd::GetIns().Play("SwordSwing");


			},
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
				Snd::GetIns().Play("Sword_Air");
				Snd::GetIns().Play("Jump_End");

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
			[&]()
			{ 
				sword->ChangePosition(Sword::SWORD_POSITION::Hand);
				AnimePlay(ANIME_TYPE::AttackCharge_Start);
			},
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

	//必殺技（一閃）状態
	AddState(
		STATE::BreakFinish,
		new PlayerBreakFinishState(
			//① 構え
			[&]()
			{
				FaceLockOnEnemy();
				//TODO：専用の構えアニメーションに差し替え
				AnimePlay(ANIME_TYPE::Attack_Special_1);
				//敵の行動を止める
				if (EnemyBase* enemy = findEnemy ? findEnemy(finishEnemyId) : nullptr)
				{
					enemy->StartBreakFinished();

					//ほかの敵もすべて止める（演出中に動かないように）
					if (freezeAllEnemies) { freezeAllEnemies(true); }

					//演出中はプレイヤーと怯んだ敵だけを描画する
					ActorBase::SetDrawFocus({ this, enemy });

					//プレイヤー→敵の向き（元の場所で計算）
					Vector3 toEnemy = enemy->GetTrans().pos - trans.pos;
					toEnemy.y = 0.0f;
					if (toEnemy.LengthSq() <= 0.000001f)
					{
						//重なっている場合は今の正面方向
						toEnemy = trans.VTrans(Vector3(0.0f, 0.0f, 1.0f));
						toEnemy.y = 0.0f;
					}
					toEnemy.Normalize();

					//敵を元の場所からステージの中心へ移す（高さはそのまま）
					finishEnemyOrigin = enemy->GetTrans().pos;
					Vector3 enemyPos = finishEnemyOrigin;
					enemyPos.x = FINISH_ARENA_X;
					enemyPos.z = FINISH_ARENA_Z;
					enemy->WarpTo(enemyPos);
					isFinishMoved_ = true;

					//中心の敵から一定の距離だけ離れた位置で構え直す
					trans.pos.x = enemyPos.x - toEnemy.x * FINISH_START_DIST;
					trans.pos.z = enemyPos.z - toEnemy.z * FINISH_START_DIST;
					TargetDirection(toEnemy);

					//カメラ：敵側の斜め前からプレイヤーを映す
					const Vector3 right(toEnemy.z, 0.0f, -toEnemy.x);
					const Vector3 camPos = trans.pos
						+ toEnemy * FINISH_READY_CAM_FRONT
						+ right * FINISH_READY_CAM_SIDE
						+ Vector3::Yonly(FINISH_READY_CAM_HEIGHT);
					const Vector3 lookPos = trans.pos + Vector3::Yonly(FINISH_READY_LOOK_HEIGHT);

					CurrentCamera::Get().StartEvent(new FixedCameraEvent(camPos, lookPos, Deg2Rad(FINISH_READY_CAM_FOV)));
				}
			},
			//② 一閃開始：敵の背後を到着点にする
			[&]()
			{
				//アニメーション再生
				AnimePlay(ANIME_TYPE::AttackCharge_End);
				//敵をすり抜けるため押し出しOFF
				SetPushFlg(false);
				SetGravityFlg(false);

				finishStartPos = trans.pos;
				finishTargetPos = trans.pos;

				if (EnemyBase* enemy = findEnemy ? findEnemy(finishEnemyId) : nullptr)
				{
					Vector3 toEnemy = enemy->GetTrans().pos - trans.pos;
					toEnemy.y = 0.0f;
					if (toEnemy.LengthSq() > 0.000001f)
					{
						//敵までの距離
						const float enemyDist = toEnemy.Length();

						toEnemy.Normalize();
						finishTargetPos = enemy->GetTrans().pos + toEnemy * FINISH_PASS_DIST;
						//高さは変えない
						finishTargetPos.y = trans.pos.y;

						//カメラ：プレイヤーの斜め後ろから、敵とプレイヤーが同時に映る画角
						const Vector3 right(toEnemy.z, 0.0f, -toEnemy.x);
						const Vector3 camPos = trans.pos
							- toEnemy * FINISH_PASS_CAM_BACK
							- right * FINISH_PASS_CAM_SIDE
							+ Vector3::Yonly(FINISH_PASS_CAM_HEIGHT);
						const Vector3 lookPos = trans.pos
							+ toEnemy * (enemyDist * FINISH_PASS_LOOK_RATE)
							+ Vector3::Yonly(FINISH_PASS_LOOK_HEIGHT);

						CurrentCamera::Get().StartEvent(new FixedCameraEvent(camPos, lookPos, Deg2Rad(FINISH_PASS_CAM_FOV)));
					}
				}
			},
			//一閃中の移動（rate：0～1）
			[&](float rate)
			{
				trans.pos.x = finishStartPos.x + (finishTargetPos.x - finishStartPos.x) * rate;
				trans.pos.z = finishStartPos.z + (finishTargetPos.z - finishStartPos.z) * rate;
			},
			//③ 背後に到着：ダメージ・ヒットストップ
			[&]()
			{
				//すり抜け終了：押し出し・重力を戻す
				SetPushFlg(true);
				SetGravityFlg(true);

				if (EnemyBase* enemy = findEnemy ? findEnemy(finishEnemyId) : nullptr)
				{
					//斬られた瞬間にアニメーションだけ再開してからダメージ（のけぞり・死亡を見せる）
					//行動はカメラが戻るまで止めたまま
					enemy->ReleaseFinishFreeze();
					enemy->TakeFinishDamage(FINISH_DAMAGE);
				}
				TimeScale::GetIns().TemporaryScale(FINISH_HITSTOP_SCALE, FINISH_HITSTOP_TIME);
				//SoundTable.csvに登録後に有効化
				//Snd::GetIns().Play("FinishSlash");
				//TODO：専用の残心アニメーションを再生（今は斬り抜けアニメーションの続きで代用）
			},
			std::bind(&Player::IsAnimeEnd, this),
			[&]()
			{
				//カメラを通常に戻す
				CurrentCamera::Get().EndEvent();
				//ステージ中心から元の場所へ戻す
				RestoreFinishPosition();
				//描画を元に戻す
				ActorBase::ClearDrawFocus();

				finishCamPrevPos = CurrentCamera::Get().GetPos();
				finishReturnCounter = 0.0f;
			},
			
			[&]()
			{
				//カメラが元の位置に戻ったか

				finishReturnCounter += 1.0f;

				//1フレームでのカメラの移動量
				const Vector3 camPos = CurrentCamera::Get().GetPos();
				const float move = (camPos - finishCamPrevPos).Length();
				finishCamPrevPos = camPos;

				//一定時間たったら戻す
				if (finishReturnCounter >= FINISH_RETURN_MAX_FRAME) { return true; }
				return finishReturnCounter >= FINISH_RETURN_MIN_FRAME && move <= FINISH_RETURN_SETTLE_DIST;
			},
		
			[&]()
			{
				//後始末

				//押し出し・重力ON
				SetPushFlg(true);
				SetGravityFlg(true);

				//カメラを通常に戻す
				CurrentCamera::Get().EndEvent();

				//途中で中断された場合も、ステージ中心から元の場所へ戻す
				RestoreFinishPosition();

				if (EnemyBase* enemy = findEnemy ? findEnemy(finishEnemyId) : nullptr) { enemy->EndBreakFinished(); }
				finishEnemyId = EnemyBase::INVALID_ID;

				//止めていたすべての敵を再開
				if (freezeAllEnemies) { freezeAllEnemies(false); }

				//描画を元に戻す
				ActorBase::ClearDrawFocus();
			},
			[&]() { ChangeState(STATE::Idle); }
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
	//必殺技中は無敵
	if (static_cast<STATE>(GetState()) == STATE::BreakFinish) { return; }

	switch (other.GetTag())
	{
	case COLLIDER_TAG::Enemy_ChilbiPunch:
	case COLLIDER_TAG::EnemySkill:
	{

		//ガード中の判定
		const bool isGuard = (static_cast<STATE>(GetState()) == STATE::Guard);
		//MPが使用可能か
		const bool isGuardAvailable = isGuard && (GetMp() >= GUARD_MP_COST);
		//受け取るダメージ量
		const int defaultDamage = other.GetDamage();

		//受け渡すダメージ
		int damage;

		//ガード中かつMPを使用可能か
		if (isGuardAvailable)
		{
			damage = static_cast<int>(defaultDamage * GUARD_DAMAGE_RATE);
		}
		else
		{
			damage = static_cast<int>(defaultDamage);
		}


		if (isGuardAvailable) { UseMp(GUARD_MP_COST); }

		//スキル側のコライダーに設定されたダメージ
		TakeDamage(damage);

		//if (!isGuardAvailable) { ChangeState(STATE::Hit); }

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
	const KEY_TYPE SKILL_KEYS[SKILL_SLOT_MAX] = 
	{
		KEY_TYPE::PlayerSkill1,
		KEY_TYPE::PlayerSkill2,
		KEY_TYPE::PlayerSkill3,
		KEY_TYPE::PlayerSkill4,
	};
	for (int i = 0; i < SKILL_SLOT_MAX; ++i)
	{
		if (Input::GetIns().GetInfo(SKILL_KEYS[i]).down) { UseSkill(i); }
	}

	//必殺技の入力チェック
	UpdateBreakFinishInput();
}

void Player::RestoreFinishPosition(void)
{
	//ステージ中心へ移していなければ何もしない
	if (!isFinishMoved_) { return; }
	isFinishMoved_ = false;

	//ステージ中心から元の場所までのずれ（水平のみ）
	const float shiftX = finishEnemyOrigin.x - FINISH_ARENA_X;
	const float shiftZ = finishEnemyOrigin.z - FINISH_ARENA_Z;

	//敵を元の場所へ戻す（高さは今のまま）
	if (EnemyBase* enemy = findEnemy ? findEnemy(finishEnemyId) : nullptr)
	{
		Vector3 enemyPos = enemy->GetTrans().pos;
		enemyPos.x = finishEnemyOrigin.x;
		enemyPos.z = finishEnemyOrigin.z;
		enemy->WarpTo(enemyPos);
	}

	//プレイヤーは敵との位置関係（敵の背後）を保ったまま移す
	//ステージの外に出る場合は範囲内に収める
	const Vector3 before = trans.pos;
	trans.pos.x = std::clamp(trans.pos.x + shiftX, -FINISH_STAGE_LIMIT, FINISH_STAGE_LIMIT);
	trans.pos.z = std::clamp(trans.pos.z + shiftZ, -FINISH_STAGE_LIMIT, FINISH_STAGE_LIMIT);

	//カメラもプレイヤーと同じだけずらして、画面がステージを横切って流れないようにする
	const Vector3 moved = trans.pos - before;
	CurrentCamera::Get().SetPos(CurrentCamera::Get().GetPos() + moved);
}

void Player::UpdateBreakFinishInput(void)
{
	if (!Input::GetIns().GetInfo(KEY_TYPE::PlayerBreakFinish).down) { return; }

	//必殺技中・被ダメージ中は発動しない
	const STATE nowState = static_cast<STATE>(GetState());
	if (nowState == STATE::BreakFinish || nowState == STATE::Hit) { return; }

	//ロックオン中の敵が怯みチャンス中か
	EnemyBase* enemy = findEnemy ? findEnemy(lockOnEnemyId) : nullptr;
	if (enemy == nullptr || enemy->IsDead() || !enemy->IsBreakChance()) { return; }

	//チャンス消費（二重発動防止）
	enemy->EndBreakChance();

	finishEnemyId = lockOnEnemyId;
	ChangeState(STATE::BreakFinish);
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

	//空中攻撃が終わったら（着地・被弾などどの終わり方でも）見下ろしカメラを終了
	//（GameCameraが補間しながら元の位置に戻る）
	if (static_cast<STATE>(prevState) == STATE::AttackAir) { CurrentCamera::Get().EndEvent(); }

	//空中攻撃の間は、プレイヤーの上から見下ろすカメラにする
	if (static_cast<STATE>(nextState) == STATE::AttackAir)
	{
		CurrentCamera::Get().StartEvent(new FollowOverheadCameraEvent(
			trans.pos,
			AIR_CAM_HEIGHT, AIR_CAM_BACK, AIR_CAM_LOOK_HEIGHT,
			Deg2Rad(AIR_CAM_FOV), AIR_CAM_BLEND_FRAME));
	}
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
	case Player::STATE::BreakFinish:
	{
		//見た目だけ手に持つ（ダメージは直接与える）
		SetWeapon(POS::Hand, ACT::DrawOnly, false);
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
	return Input::GetIns().GetInfo(KEY_TYPE::PlayerSkillMenu).now ||
		Input::GetIns().GetTriggerInfo(TRIGGER_TYPE::Right).now;
}

void Player::SubDraw(void)
{
	//銃状態の時のみ描画を行う
	if(GetState() == (int)STATE::Gun)
	{
		gun->DrawAimUI();
	}

}





