#include "PlayerGunState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"




PlayerGunState::PlayerGunState(
	std::function<void(void)> playAnimeGun, 
	std::function<void(void)> ChangeStateIdle,
	float& targetTime,
	int& bulletCount,
	std::function<void(void)> playShot)
	:
	playAnimeGun(playAnimeGun),
	ChangeStateIdle(ChangeStateIdle),
	targetTime(targetTime),
	bulletCount(bulletCount),
	playShot(playShot),
	state(STATE::None)
{
}

void PlayerGunState::OwnStateConditionUpdate(void)
{
	//入力
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).down) { OwnChangeState(); }
}

void PlayerGunState::Enter(void)
{
	playAnimeGun();

	//初期状態
	state = STATE::Target;

	//ターゲットタイムリセット
	targetTime = 0.0f;
}

void PlayerGunState::Update(void)
{
	//待機状態に遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerGunMode).up) { ChangeStateIdle(); }

	switch (state)
	{
		case PlayerGunState::STATE::Target:
		{

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
			playShot();
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
	
}

void PlayerGunState::ChangeState(STATE state_)
{
	state = state_;
}
