#include "WorldSceneBase.h"

#include "../../../Object/Common/ActorBase/ActorBase.h"

WorldSceneBase::WorldSceneBase(void) :
	SceneBase(),
	gameSpace()
{
}

void WorldSceneBase::SubPreUpdate(void)
{
	// オブジェクトが入力を移動方向へ変換する前に、空間切り替え状態を進める
	gameSpace.Update();

	SubWorldPreUpdate();
}

void WorldSceneBase::SubPostUpdate(void)
{
	// 当たり判定の押し出し後に補正することで、2D中にZ方向へずれる問題を防ぐ
	for (ActorBase* object : objects) {
		if (object != nullptr) { gameSpace.ApplyConstraint(*object); }
	}

	SubWorldPostUpdate();
}

void WorldSceneBase::SubPreObjectAdd(ActorBase& object)
{
	// WorldSceneBaseが所有する全オブジェクトへ空間管理クラスを自動登録する
	object.SetGameSpaceController(&gameSpace);

	SubWorldPreObjectAdd(object);
}

void WorldSceneBase::SubAlphaDraw(void)
{
	gameSpace.DrawDebug();
	SubWorldDebugDraw();

	SubWorldAlphaDraw();
}

