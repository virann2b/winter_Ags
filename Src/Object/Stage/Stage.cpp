#include "Stage.h"
#include "../Common/Collider/MeshCollider.h"

Stage::Stage()
{
}

void Stage::Load(void)
{
	//モデル読み込みw
	trans.LoadModel("Stage/Stage2");
	trans.pos = Vector3( 0,-100,0 );
	trans.scale = 3.0f;

	//描画ON
	SetIsDraw(true);

	//重力・移動OFF
	SetGravityFlg(false);
	SetDynamicFlg(false);

	//押し出しON
	SetPushFlg(true);

	//描画限定判定
	SetIgnoreDrawFocus(true);
	//デバッグ表示OFF
	SetColliderDebugDraw(false);

	//コライダー追加
	AddCollider(new MeshCollider(COLLIDER_TAG::Stage, trans.model));
}
