#include "PlayerRollState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"



PlayerRollState::PlayerRollState(
	std::function<void(void)> PlayAnimeRoll,
	std::function<bool(void)> IsAnimeEnd,
	std::function<void(const Vector3&)> MoveAccel,
	const Transform& playerTrans,
	std::function<void(void)> ChangeStateIdle

):
	PlayAnimeRoll(PlayAnimeRoll),
	IsAnimeEnd(IsAnimeEnd),
	MoveAccel(MoveAccel),
	playerTrans(playerTrans),
	ChangeStateIdle(ChangeStateIdle)
{
}

void PlayerRollState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerRoll).down) { OwnChangeState(); }
}

void PlayerRollState::Enter(void)
{
	//入力方向を取得
	Vector3 inputVec = InputVec();

	if (inputVec != 0.0f) 
	{
		// 入力があればカメラ基準に回転
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));
		rollDir = inputVec;
	}
	else
	{
		// 入力がなければプレイヤーの正面
		rollDir = playerTrans.VTrans(Vector3(0.0f, 0.0f, 1.0f));
	}

	rollDir.y = 0.0f;
	rollDir.Normalize();
	
	//方向 × 距離 = 移動ベクトル
	rollMove = rollDir * ROLL_DISTANCE;

	//回避の目標座標
	rollTargetPos = playerTrans.pos + rollMove;

	//アニメーションの再生
	PlayAnimeRoll();
}

void PlayerRollState::Update(void)
{
	//巡回目標座標までのベクトルを取得
	Vector3 vec = rollTargetPos - playerTrans.pos;
	//Y軸方向は移動しない
	vec.y = 0.0f;

	//巡回目標座標までの距離が近い場合
	if (vec.Length() <= ARRIVE_RANGE || IsAnimeEnd())
	{
		ChangeStateIdle();
		return;
	}

	//巡回目標座標までのベクトルを正規化して加速移動する
	vec.Normalize();
	MoveAccel(vec);
}


Vector3 PlayerRollState::InputVec(void) const
{
	// 返却用一時変数
	Vector3 vec = Vector3();

	// コントローラーの入力を取得
	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	return vec;
}
