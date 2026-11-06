#include "Chilbi.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/TimeScale/TimeScale.h"
#include "../../../Scene/SceneManager.h"

#include "../../Common/Collider/CapsuleCollider.h"

#include "State/ChilbiIdleState.h"
#include "State/ChilbiMoveState.h"
#include "State/ChilbiHitState.h"
#include "State/ChilbiDieState.h"
#include "State/ChilbiAttackState.h"

#include "../../Skill/SkillFire/SkillFire.h"



Chilbi::Chilbi(Vector3 initPos)
	:
	EnemyBase(initPos),
	punch(nullptr),
	state(STATE::None)
{
}

void Chilbi::Load(void)
{

#pragma region モデル設定

	//オブジェクト初期設定
	SetupEnemyObject(CHILIBI_WEIGHT);


	// モデルの読み込み
	trans.LoadModel("Enemy/Chilbi/Chilbi");

	// モデルのスケール設定
	trans.scale = 2.0f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -102.81f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	//初期座標登録
	trans.pos = GetInitPos();

	//加速度を調整
	ACCEL_MAX = 5.5f;

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
			Vector3::Yonly(50.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			35.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion

#pragma region 状態設定

	//待機状態
	AddState(
		STATE::Idle,
		new ChilbiIdleState(
			[&]() { AnimePlay((int)ANIME_TYPE::Idle);},
			[&]() { ChangeState(STATE::Move); },
			[&]() { ChangeState(STATE::Attack); },
			[&]() { return IsNear(); },
			attackCoolTime
		)
	);

	//移動状態
	AddState(
		STATE::Move,
		new ChilbiMoveState(
			[&]() { AnimePlay((int)ANIME_TYPE::Move); },
			[&]() { ChangeState(STATE::Attack); },
			std::bind(&Chilbi::MoveAccel, this, std::placeholders::_1),
			[&]() { return GetDirectionToTarget(); },
			[&]() { return IsNear(); }
		)
	);

	//HIT状態
	AddState(
		STATE::Hit,
		new ChilbiHitState(
			[&]() 
			{
				//攻撃状態から遷移した場合の判定削除
				punch->JudgeEnd();

				AnimePlay((int)ANIME_TYPE::Hit);
			},
			std::bind(&Chilbi::IsDead, this),
			[&]() { ChangeState(STATE::Idle); },
			std::bind(&Chilbi::IsAnimeEnd, this)
		)
	);

	//攻撃状態
	AddState(
		STATE::Attack,
		new ChilbiAttackState(
			[&]() 
			{ 
				AnimePlay((int)ANIME_TYPE::Attack_1); 
				punch->JudgeStart();
			},
			[&]()
			{ 
				AnimePlay((int)ANIME_TYPE::Attack_2); 
				punch->JudgeStart();

			},
			[&]() 
			{ 
				punch->JudgeEnd();
				attackCoolTime = DEFAULT_ATTACK_COOLTIME;
				ChangeState(STATE::Idle); 

			},
			std::bind(&Chilbi::IsAnimeEnd, this)
		)
	);

	//死亡状態
	AddState(
		STATE::Die,
		new ChilbiDieState(
			[&]() 
			{ 
				//攻撃状態から遷移した場合の判定削除
				punch->JudgeEnd();
				//アニメーション再生
				AnimePlay((int)ANIME_TYPE::Die); 
			},
			std::bind(&Chilbi::IsDead, this),
			std::bind(&Chilbi::IsAnimeEnd, this),
			//死亡アニメーション終了後に描画・判定を消す
			[&]() { DieEnd(); }
		)
	);



#pragma endregion

#pragma region 子クラス生成

	//パンチの生成
	punch = new Punch(trans);
	AddChildActor(punch);

	//スキル（ファイア）：中距離～遠距離で使う
	AddSkill(
		new SkillFire(trans, SkillBase::SKILL_USER::Enemy),
		DISTANCE::Middle, DISTANCE::Long,
		FIRE_COOLTIME
	);

#pragma endregion

}


void Chilbi::EnemyInit(void)
{
	//初期状態
	ChangeState(STATE::Idle);

}

void Chilbi::EnemyUpdate(void)
{
	//攻撃クールタイムの更新
	if (attackCoolTime > 0.0f)
	{
		attackCoolTime -= TimeScale::Get();
	}
	else
	{
		attackCoolTime = 0.0f;
	}

	//プレイヤーに近づいている途中（攻撃範囲外）ならスキルを使う
	if (GetState() == (int)STATE::Move) { TryUseSkill(); }
}

void Chilbi::EnemyDraw(void)
{
	DrawFormatString(0, 80, 0xffffff, "EnemyHp : %d", hp);
}

void Chilbi::OnHit(void)
{
	ChangeState(STATE::Hit);
}

void Chilbi::OnDie(void)
{
	ChangeState(STATE::Die);

}
