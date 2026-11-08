#include "PlayerGunState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerGunState::PlayerGunState(
	std::function<void(void)> PlayAnimeGun,
	std::function<void(void)> ChangeStateIdle,
	std::function<void(const Vector3&)> MoveAccel,
	std::function<void(void)> PlayShot,
	float ACCEL_MAX,
	float& accelMax,
	std::function<bool(void)> IsLockOn
):
	PlayAnimeGun(PlayAnimeGun),
	ChangeStateIdle(ChangeStateIdle),
	MoveAccel(MoveAccel),
	PlayShot(PlayShot),
	ACCEL_MAX(ACCEL_MAX),
	accelMax(accelMax),
	IsLockOn(IsLockOn)
{
}

void PlayerGunState::OwnStateConditionUpdate(void)
{
	//ロックオンをしている時のみ銃に遷移可能
	if (IsLockOn())
	{
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).down
			|| Input::GetIns().GetTriggerInfo(TRIGGER_TYPE::Left).down){ OwnChangeState(); }
	}
}

void PlayerGunState::Enter(void)
{
	//銃アニメーション再生
	PlayAnimeGun();
}

void PlayerGunState::Update(void)
{
	//ロックオンが外れた（Eで解除・敵がいなくなった）ら構えを解く
	if (!IsLockOn())
	{
		ChangeStateIdle();
		return;
	}

	//待機状態に遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).up || Input::GetIns().GetTriggerInfo(TRIGGER_TYPE::Left).up)
	{ 
		ChangeStateIdle();
		return;
	}

	//右クリックを押している時のみ
	//コントローラの場合はRTが入力された時のみ
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunShot).down || Input::GetIns().GetTriggerInfo(TRIGGER_TYPE::Right).down)
	{
		PlayShot();
	}


	Vector3 inputVec = InputVec();

	// スティックの倒し具合で速度を変える(キーボードは長さ1なので最大速度)
	accelMax = ACCEL_MAX * inputVec.Length();

	if (inputVec != 0.0f) 
	{
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));
		MoveAccel(inputVec);
	}
	
}

void PlayerGunState::Exit(void)
{
	accelMax = ACCEL_MAX;

	//カメラを元のモードに戻すのも、GameSceneから伝わった「構えていない」をもとにカメラが行う
}

Vector3 PlayerGunState::InputVec(void) const
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


