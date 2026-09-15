#include "Cactus.h"
#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/CactusWait.h"
#include "State/CactusAttack.h"

void Cactus::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Cactus/Cactus");

	//アニメーション追加
	CreateAnimationController();
	//アニメーション登録
	AddInFbxAnimation((int)ANIM::MAX, animationSpeedTabel_);

	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, 80.0f));
	// モデルの角度のズレを設定
	trans.localAngle.y = Deg2Rad(180.0f);
	//サイズ設定
	trans.scale = 1.3f;
	//アニメーションずれ修正
	trans.centerDiff = Vector3(0.0f,- 60.0f,0.0f) * trans.scale;

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	//待機状態追加
	AddState(
		STATE::Wait,
		new CactusWait(
			trans.pos,
			playerPos_,
			[&]() {ChangeState(STATE::Attack); },
			[&]() 
			{
			
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -60.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Wait, true); 
			}
		));

	AddState(
		STATE::Attack,
		new CactusAttack(
			trans.pos,
			playerPos_,
			[&]() {ChangeState(STATE::Wait); },
			[&]() 
			{
				//アニメーションずれ修正
				trans.centerDiff = Vector3(0.0f, -80.0f, 0.0f) * trans.scale;
				//再生アニメーション
				AnimePlay((int)ANIM::Attack, false); 
			},
			trans.angle.y
		));

#pragma endregion
}

void Cactus::CharacterInit(void)
{
	//初期
	ChangeState(STATE::Wait);
}

void Cactus::CharacterUpdate(void)
{
}

void Cactus::CharacterDraw(void)
{
}

void Cactus::CharacterAlphaDraw(void)
{
}

void Cactus::CharacterUiDraw(void)
{
}

void Cactus::CharacterRelease(void)
{
}
