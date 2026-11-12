#include "PlayerStatusUI.h"

#include "../../../Application/Application.h"


#include <string>
#include <algorithm>

PlayerStatusUI::PlayerStatusUI(
	const CharacterBase::CharacterStatus& status
):
	UIBase(),
	status(status)
{
}

void PlayerStatusUI::Init(void)
{
	hpDelay = Img::GetIns().GetImage(Img::IMAGE_TAG::HpDelay);

	//赤いバーは今のHPから始める
	delayHp = (float)status.hp;
	prevHp = status.hp;
	delayWaitCount = 0.0f;
}

void PlayerStatusUI::Update(void)
{
	if (status.hp < prevHp) { delayWaitCount = DELAY_WAIT_FRAME; }
	//前フレームHP更新
	prevHp = status.hp;

	if (delayHp <= (float)status.hp)
	{
		delayHp = (float)status.hp;
		return;
	}

	if (delayWaitCount > 0.0f)
	{
		delayWaitCount -= 1.0f;
		return;
	}
	//HPの減少
	delayHp -= status.hp * DELAY_DECREASE_RATE;
	if (delayHp < (float)status.hp) { delayHp = (float)status.hp; }
}

void PlayerStatusUI::Draw(void)
{
#pragma region 画像読み込み

	const Img::ImageData& hpFrame = Img::GetIns().GetImage(Img::IMAGE_TAG::HpFrame);
	const Img::ImageData& hpFill = Img::GetIns().GetImage(Img::IMAGE_TAG::HpFill);

	const Img::ImageData& mpFrame = Img::GetIns().GetImage(Img::IMAGE_TAG::MpFrame);
	const Img::ImageData& mpFill = Img::GetIns().GetImage(Img::IMAGE_TAG::MpFill);

#pragma endregion


	//バーの左端（画面の中央にそろえる）
	const int left = Application::SCREEN_SIZE_X_HALF - BAR_WIDTH / 2;
	const int right = left + BAR_WIDTH;

	//MPバーの上端（画面の下から）
	const int top = Application::SCREEN_SIZE_Y - BOTTOM_MARGIN - MP_BAR_HEIGHT;

	//HPバーの上端（MPバーの上）
	const int hpTop = top - BAR_SPACE - HP_BAR_HEIGHT;
	//MPバーの上端（MPバーの上）
	const int mpTop = top - BAR_SPACE - MP_BAR_HEIGHT;

#pragma region HP

	

	const float hpScale = (float)FRAME_DRAW_WIDTH / (float)HP_FRAME_SRC_W;

	const int hpFrameW = FRAME_DRAW_WIDTH;
	const int hpFrameH = (int)(HP_FRAME_SRC_H * hpScale);
	const int hpFrameLeft = Application::SCREEN_SIZE_X_HALF - hpFrameW / 2;
	const int hpFrameTop = hpTop - BAR_SPACE - hpFrameH;

	//枠（画像の枠の部分だけを切り取って描く）
	DrawRectExtendGraph(
		hpFrameLeft, hpFrameTop, hpFrameLeft + hpFrameW, hpFrameTop + hpFrameH,
		HP_FRAME_SRC_X, HP_FRAME_SRC_Y, HP_FRAME_SRC_W, HP_FRAME_SRC_H,
		hpFrame.handle, true);

	//バーの表示位置
	const int hpFillLeft = hpFrameLeft + (int)((HP_FILL_X - HP_FRAME_SRC_X) * hpScale);
	const int hpFillTop = hpFrameTop + (int)((HP_FILL_Y - HP_FRAME_SRC_Y) * hpScale);
	const int hpFillW = (int)(hpFill.sizeX * hpScale);
	const int hpFillH = (int)(hpFill.sizeY * hpScale);

	//バー
	DrawFillRate(hpFillLeft, hpFillTop, hpFillW, hpFillH, GetRate(delayHp, (float)status.maxHp), hpDelay);

	DrawFillRate(hpFillLeft, hpFillTop, hpFillW, hpFillH, GetRate((float)status.hp, (float)status.maxHp), hpFill);

	//数値（バーの右端・枠の上）
	DrawValue(hpFillLeft + hpFillW, hpFrameTop, status.hp);

#pragma endregion


#pragma region MP

	const float mpScale = (float)FRAME_DRAW_WIDTH / (float)MP_FRAME_SRC_W;

	//枠の表示位置（画面中央・画面の下から）
	const int mpFrameW = FRAME_DRAW_WIDTH;
	const int mpFrameH = (int)(MP_FRAME_SRC_H * mpScale);
	const int mpFrameLeft = Application::SCREEN_SIZE_X_HALF - mpFrameW / 2;
	const int mpFrameTop = Application::SCREEN_SIZE_Y - BOTTOM_MARGIN - mpFrameH;

	DrawRectExtendGraph(
		mpFrameLeft, mpFrameTop, mpFrameLeft + mpFrameW, mpFrameTop + mpFrameH,
		MP_FRAME_SRC_X, MP_FRAME_SRC_Y, MP_FRAME_SRC_W, MP_FRAME_SRC_H,
		mpFrame.handle, true);

	//バーの表示位置と大きさ（画像の大きさではなく、決めた範囲に合わせる）
	const int mpFillLeft = mpFrameLeft + (int)((MP_FILL_X - MP_FRAME_SRC_X) * mpScale);
	const int mpFillTop = mpFrameTop + (int)((MP_FILL_Y - MP_FRAME_SRC_Y) * mpScale);
	const int mpFillW = (int)(MP_FILL_W * mpScale);
	const int mpFillH = (int)(MP_FILL_H * mpScale);

	DrawFillRate(mpFillLeft, mpFillTop, mpFillW, mpFillH, GetRate((float)status.mp, (float)status.maxMp), mpFill);
	DrawValue(mpFillLeft + mpFillW, mpFrameTop, status.mp);

#pragma endregion

}


void PlayerStatusUI::DrawValue(int right, int top, int value) const
{
	//数値を文字にする
	const std::string text = std::to_string(value);

	//文字の幅を測って右端をそろえる
	const int width = GetDrawStringWidth(text.c_str(), (int)text.size());

	//バーの上端から文字の高さ分上に描く
	DrawString(right - width, top - TEXT_SPACE - GetFontSize(), text.c_str(), TEXT_COLOR);
}
