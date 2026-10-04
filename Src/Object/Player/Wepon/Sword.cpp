#include "Sword.h"

#include "../../../Utility/Utility.h"
#include "../../Common/Collider/CapsuleCollider.h"


Sword::Sword(
	const Transform& playerTrans)
	:ActorBase(),
	playerTrans(playerTrans)
{
}

void Sword::Load(void)
{
	// モデルをロード
	trans.LoadModel("Sword/Sword");


	AddCollider(
		new CapsuleCollider(COLLIDER_TAG::Sword,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 0.7f;

	//描画ON
	SetIsDraw(true);
	//判定オフ
	SetJudgeFlg(false);
}

void Sword::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
}

void Sword::SubUpdate(void)
{
	// フレーム22のワールドマトリクスを取得
	MATRIX mat = MV1GetFrameLocalWorldMatrix(playerTrans.model, RIGHT_HAND_FLAME_INDEX);

	// 位置補正（プレイヤーの向きに合わせて微調整）
	MATRIX offset = MMult(MGetTranslate(VGet(0.0f, 0.0f, -3.0f)), mat);

	// 武器自身の位置を適用
	trans.pos = VGet(offset.m[3][0], offset.m[3][1], offset.m[3][2]);
}
