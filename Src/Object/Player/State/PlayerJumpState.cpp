#include "PlayerJumpState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerJumpState::PlayerJumpState(
	float JUMP_POWER,

	float& VelocityY,
	const bool& IsGround,
	bool& IsMaxJump,

	std::function<void(const Vector3&)> MoveAccel,

	std::function<void(void)> PlayAnimeJumpStart,
	std::function<void(void)> PlayAnimeJumpLoop,
	std::function<void(void)> PlayAnimeStamp,

	std::function<bool(void)> IsAnimeEnd,

	std::function<void(void)> ChangeStateIdle
) :
	JUMP_POWER(JUMP_POWER),

	VelocityY(VelocityY),
	IsGround(IsGround),
	IsMaxJump(IsMaxJump),

	MoveAccel(MoveAccel),

	PlayAnimeJumpStart(PlayAnimeJumpStart),
	PlayAnimeJumpLoop(PlayAnimeJumpLoop),
	PlayAnimeStamp(PlayAnimeStamp),

	IsAnimeEnd(IsAnimeEnd),

	ChangeStateIdle(ChangeStateIdle),

	step(STEP::Start),

	jumpCount(0),
	isAirborne(false)
{
}

void PlayerJumpState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down&&jumpCount < MAX_JUMP_COUNT) { OwnChangeState(); }
}

void PlayerJumpState::Enter(void)
{
	// フラグリセット
	step = STEP::Start;

	// ジャンプ開始アニメを再生
	PlayAnimeJumpStart();

	//ジャンプ回数を加算
	jumpCount++;
}

void PlayerJumpState::Update(void)
{
	switch (step) {

	case PlayerJumpState::STEP::Start: {
		// ジャンプ開始

		// アニメーション終了
		if (IsAnimeEnd()) {

			// 加速度加算
			VelocityY = JUMP_POWER;

			// 滞空アニメーションを再生
			PlayAnimeJumpLoop();

			// 次のステップへ
			step = STEP::Loop;
		}

		break;
	}

	case PlayerJumpState::STEP::Loop: {

		// コントローラーの入力を取得
		Vector3 vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

		// 入力がなければ次にキーボードの入力を取得
		if (vec == 0.0f) {
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
			if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
			vec.Normalize();
		}

		OwnStateConditionUpdate();

		if (jumpCount >= 2)
		{
			IsMaxJump = true;
		}

		// 最終的に入力があれば移動させる
		if (vec != 0.0f) {

			// 移動方向をカメラで回転させる
			vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

			// 移動
			MoveAccel(vec);
		}

		// 着地
		if (IsGround) {

			//ジャンプカウントリセット
			jumpCount = 0;
			isAirborne = false;
			// 着地のアニメーションを再生
			PlayAnimeStamp();
			// 次のステップへ
			step = STEP::Stamp;
		}

		break;
	}

	case PlayerJumpState::STEP::Stamp: {
		
		// 着地

		// この時点から入力によって再度ジャンプが可能にしておく
		OwnStateConditionUpdate();

		// アニメーション終了
		if (IsAnimeEnd()) {

			// 待機状態に戻り終了
			ChangeStateIdle();
		}

		break;
	}

	}
}

void PlayerJumpState::AlwaysUpdate(void)
{
	if (jumpCount == 0) return;

	// 空中に出たかどうかを記録
	if (!IsGround) { isAirborne = true; }

	// 一度空中に出てから着地したらリセット
	if (isAirborne && IsGround)
	{
		jumpCount = 0;
		isAirborne = false;
	}

}

