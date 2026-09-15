#include "Icicle.h"

#include "../../../../Utility/Utility.h"
#include "../../../Common/Collider/CapsuleCollider.h"


Icicle::Icicle(const Vector3*& playerPos)
	:ActorBase(),
	playerPos(playerPos)
{
}

void Icicle::Load(void)
{
	// モデルをロード
	trans.LoadModel("Icicle/Icicle");

	ColliderCreate(
		new CapsuleCollider(COLLIDER_TAG::Icicle,
			Vector3::Yonly(40.0f),
			Vector3::Yonly(-40.0f),
			40.0f));

	//サイズ設定
	trans.scale = 1.3f;

	//初期ステート
	state = STATE::None;

	//描画オフ
	SetIsDraw(false);
	//判定オフ
	SetJudge(false);
}


void Icicle::SubUpdate(void)
{


	//状態管理
	switch (state)
	{
	case Icicle::STATE::None: { return; }

	case Icicle::STATE::Create: {

		SetGravityFlg(false);

		trans.scale += 0.05f;
		if (trans.scale.MaxElementF() >= MODEL_SCALE) {
			state = STATE::Wait;
		}

		break;
	}

	case Icicle::STATE::Wait: {

		if(++waitCounter >= WAIT_TIME) 
		{

			SetGravityFlg(true);

			state = STATE::Fall;
		}

		break;
	}

	case Icicle::STATE::Fall: 
	{


		//画面外にでたら消す
		if (trans.pos.y <= -200.0f) {
			state = STATE::None;

			SetIsDraw(false);

			SetJudge(false);
		}

		break;
	}

	}
}

void Icicle::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	switch (other.GetTag())
	{
	case COLLIDER_TAG::Player:
		state = STATE::None;

		SetIsDraw(false);

		SetJudge(false);
		break;
	case COLLIDER_TAG::Enemy:
		break;
	case COLLIDER_TAG::Stage:
		SetIsDraw(false);

		SetJudge(false);
		break;
	case COLLIDER_TAG::Icicle:
		break;
	default:
		break;
	}
}

void Icicle::Start(void)
{
	// 描画判定を有効にする
	SetIsDraw(true);

	// 当たり判定を有効にする
	SetJudge(true);

	// スケールを0にする
	trans.scale = 0.0f;

	//加速度初期化
	velocity = 0.0f;

	// 重力を消す
	SetGravityFlg(false);

	// 座標を設定
	trans.pos = *playerPos + CREATE_LOCAL_POS;

	//カウンター初期化
	waitCounter = 0;

	// 状態を生成中にする
	state = STATE::Create;
}
