#include "Stage.h"
#include "../Common/Collider/MeshCollider.h"

Stage::Stage()
{
}

void Stage::Load(void)
{
	//モデル読み込み
	trans.LoadModel("Stage/Stage");
	trans.pos = Vector3( 0,-100,0 );
	trans.scale = 2.0f;

	//描画ON
	SetIsDraw(true);

	//重力・移動OFF
	SetGravityFlg(false);
	SetDynamicFlg(false);

	//押し出しON
	SetPushFlg(true);
	
	AddCollider(new MeshCollider(COLLIDER_TAG::Stage, trans.model));
}
