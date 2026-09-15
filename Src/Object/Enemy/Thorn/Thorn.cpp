#include "Thorn.h"

#include "Wepon/Icicle.h"

#include "../../../Utility//Utility.h"
#include "../../Common/Collider/SphereCollider.h"

#include "State/ThornAttack.h"
#include "State/ThornWait.h"

void Thorn::Load(void)
{
	// モデルをロード
	trans.LoadModel("Enemy/Thorn/Thorn");

	//アニメーション追加
	CreateAnimationController();
	//アニメーション登録
	AddInFbxAnimation((int)ANIM::MAX, animationSpeedTabel_);

	ColliderCreate(new SphereCollider(COLLIDER_TAG::Enemy, 60.0f));

	// モデルの角度のズレを設定
	trans.localAngle.y = Deg2Rad(180.0f);
	
	//サイズ設定
	trans.scale = 1.3f;

	//アニメーションずれ修正
	trans.centerDiff = Vector3(0.0f, -40.0f, 0.0f) * trans.scale;

	//プレイヤー座標参照を取得
	Icicle* icicle = new Icicle(playerPos);
	subObjects.emplace_back(icicle);

#pragma region 状態初期設定(ステートが追加されるたびに追加する)

	// 移動状態を追加
	AddState(
		STATE::Attack,
		new ThornAttack(
			trans.pos,
			playerPos,
			[&]()
			{
				ChangeState(STATE::Wait);
			},
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Attack, false);
			},
			trans.angle.y,
			*icicle
			));

	//待機状態を追加
	AddState(
		STATE::Wait,
		new ThornWait(
			trans.pos,
			playerPos,
			[&]()
			{
				ChangeState(STATE::Attack);
			},
			[&]()
			{
				//再生アニメーション
				AnimePlay((int)ANIM::Wait, true);
			}
		));
	

#pragma endregion

	for (ActorBase* subObject : subObjects) { subObject->Load(); }
}

void Thorn::CharacterInit(void)
{
	ChangeState(STATE::Wait);

	// 加減速度を設定
	ACCEL_RATE = DECEL_RATE = 0.5f;
	// 加速最大値を設定
	ACCEL_MAX = 4.0f;
	for (ActorBase* subObject : subObjects) { subObject->Init(); }
}

void Thorn::CharacterUpdate(void)
{
	// 抱える下位アクター全ての描画処理
	for (ActorBase* subObject : subObjects) { subObject->Update(); }

}

void Thorn::CharacterDraw(void)
{
	// 抱える下位アクター全ての描画処理
	for (ActorBase* subObject : subObjects) { subObject->Draw(); }
}

void Thorn::CharacterAlphaDraw(void)
{
	// 抱える下位アクター全ての描画処理
	for (ActorBase* subObject : subObjects) { subObject->AlphaDraw(); }
}

void Thorn::CharacterUiDraw(void)
{
	for (ActorBase* subObject : subObjects) { subObject->UiDraw(); }
}

void Thorn::CharacterRelease(void)
{
	// 抱える下位アクター全ての解放処理
	for (ActorBase*& subObject : subObjects) {
		subObject->Release();
		delete subObject;
		subObject = nullptr;
	}
	subObjects.clear();
}

