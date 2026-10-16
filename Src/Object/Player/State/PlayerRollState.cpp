#include "PlayerRollState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"



PlayerRollState::PlayerRollState(
	std::function<void(void)> ChangeStateIdle,
	std::function<void(void)> playAnimeRoll,
	std::function<bool(void)> isAnimeEnd, 
	const Transform& playerTrans
):
	ChangeStateIdle(ChangeStateIdle),
	playAnimeRoll(playAnimeRoll),
	isAnimeEnd(isAnimeEnd),
	playerTrans(playerTrans)
{
}

void PlayerRollState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerRoll).down) { OwnChangeState(); }
}

void PlayerRollState::Enter(void)
{

	playAnimeRoll();

	int frnNo = MV1SearchFrame(playerTrans.model, "smartrig:Hips");
	if (frnNo < 0) { playAnimeRoll(); return; }   // フレームが見つからない場合の保険

	MV1ResetFrameUserLocalMatrix(playerTrans.model, frnNo);
	MATRIX mat = MV1GetFrameLocalMatrix(playerTrans.model, frnNo);

	// 元の行列を保ったまま、移動成分(m[3])のXZだけ0にしてYは元の値を維持
	// (Yを固定したい場合は 79.0f を入れる)
	mat.m[3][0] = 0.0f;
	mat.m[3][2] = 0.0f;

	MV1SetFrameUserLocalMatrix(playerTrans.model, frnNo, mat);
}

void PlayerRollState::Exit(void)
{
	int frnNo = MV1SearchFrame(playerTrans.model, "smartrig:Hips");
	if (frnNo >= 0) { MV1ResetFrameUserLocalMatrix(playerTrans.model, frnNo); }
}

void PlayerRollState::Update(void)
{
	if (isAnimeEnd())
	{
		ChangeStateIdle();
	}
}
