#include "PlayerGunState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"
#include "../../../Manager/Camera/Game/GameCamera.h"

PlayerGunState::PlayerGunState(
	std::function<void(void)> PlayAnimeGun,
	std::function<void(void)> ChangeStateIdle,
	std::function<void(const Vector3&)> MoveAccel,
	float ACCEL_MAX,
	float& accelMax,
	float& targetTime,
	int& bulletCount,
	std::function<void(void)> PlayShot
):
	PlayAnimeGun(PlayAnimeGun),
	ChangeStateIdle(ChangeStateIdle),
	MoveAccel(MoveAccel),
	ACCEL_MAX(ACCEL_MAX),
	accelMax(accelMax),
	targetTime(targetTime),
	bulletCount(bulletCount),
	PlayShot(PlayShot),
	state(STATE::None)
{
}

void PlayerGunState::OwnStateConditionUpdate(void)
{
	//入力
	auto camera = dynamic_cast<GameCamera*>(&CurrentCamera::Get())->GetMode();

	//ロックオンをしている時のみ銃に遷移可能
	if (camera == GameCamera::Mode::Auto)
	{
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).down) { OwnChangeState(); }
	}
}

void PlayerGunState::Enter(void)
{
	//銃アニメーション再生
	PlayAnimeGun();

	//初期状態
	state = STATE::Target;

	//ターゲットタイムリセット
	targetTime = 0.0f;

	// カメラを専用のモードに変える
	dynamic_cast<GameCamera*>(&CurrentCamera::Get())->SetMode(GameCamera::Mode::Gun);
}

void PlayerGunState::Update(void)
{
	//待機状態に遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).up) 
	{ 
		ChangeStateIdle();
		return;
	}

	switch (state)
	{
		case PlayerGunState::STATE::Target:
		{
			//右クリックを押している時のみ
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunShot).down)
			{
				ChangeState(STATE::Shot);
				return;
			}

			if (targetTime < BULLET_COUNT_MAX_TIME)
			{
				targetTime++;
				int count = (int)(targetTime / BULLET_COUNT_TIME);
				bulletCount = (count > MAX_BULLET) ? MAX_BULLET : count;
			}


			break;
		}
		case PlayerGunState::STATE::Shot:
		{
			if (bulletCount == 0)
			{
				ChangeState(STATE::Relod);
				return;
			}

			//弾の処理
			PlayShot();
			bulletCount--;


			break;
		}
		case PlayerGunState::STATE::Relod:
		{
			//ターゲット追従時間リセット
			targetTime = 0.0f;
			//発射数リセット
			bulletCount = 0;
			//ターゲット状態に遷移
			ChangeState(STATE::Target);

			break;
		}
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

	// カメラを専用のモードに変える
	dynamic_cast<GameCamera*>(&CurrentCamera::Get())->SetMode(GameCamera::Mode::Auto);
}

void PlayerGunState::ChangeState(STATE state_)
{
	state = state_;
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


