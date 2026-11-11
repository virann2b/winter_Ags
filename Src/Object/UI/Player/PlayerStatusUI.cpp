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

void PlayerStatusUI::Draw(void)
{
	//バーの左端（画面の中央にそろえる）
	const int left = Application::SCREEN_SIZE_X_HALF - BAR_WIDTH / 2;
	const int right = left + BAR_WIDTH;

	//MPバーの上端（画面の下から）
	const int mpTop = Application::SCREEN_SIZE_Y - BOTTOM_MARGIN - MP_BAR_HEIGHT;
	//HPバーの上端（MPバーの上）
	const int hpTop = mpTop - BAR_SPACE - HP_BAR_HEIGHT;

	//HP
	DrawBar(left, hpTop, HP_BAR_HEIGHT, status.hp, status.maxHp, HP_COLOR);
	DrawValue(right, hpTop, status.hp);

	//MP
	DrawBar(left, mpTop, MP_BAR_HEIGHT, status.mp, status.maxMp, MP_COLOR);
	DrawValue(right, mpTop, status.mp);
}

void PlayerStatusUI::DrawBar(int left, int top, int height, int value, int maxValue, unsigned int color) const
{
	//背景
	DrawBox(left, top, left + BAR_WIDTH, top + height, BACK_COLOR, true);

	//最大値が0なら中身は描かない（0除算防止）
	if (maxValue <= 0) { return; }

	//残りの割合（0～1におさめる）
	const float rate = std::clamp((float)value / (float)maxValue, 0.0f, 1.0f);

	//中身（左から割合分）
	const int fillWidth = (int)(BAR_WIDTH * rate);
	DrawBox(left, top, left + fillWidth, top + height, color, true);
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
