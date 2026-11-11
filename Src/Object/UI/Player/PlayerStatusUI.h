#pragma once

#include "../UIBase.h"
#include "../../Common/CharacterBase/CharacterBase.h"

class PlayerStatusUI : public UIBase
{

public:

	PlayerStatusUI(const CharacterBase::CharacterStatus& status);
	~PlayerStatusUI(void) override = default;

	// 描画
	void Draw(void) override;

private:

#pragma region 定数

	// バーの横幅
	static constexpr int BAR_WIDTH = 480;
	// HPバーの高さ
	static constexpr int HP_BAR_HEIGHT = 14;
	// MPバーの高さ
	static constexpr int MP_BAR_HEIGHT = 10;

	// 画面下からMPバーの下端までの距離
	static constexpr int BOTTOM_MARGIN = 40;
	// HPバーの下端からMPバーの上端までの距離
	static constexpr int BAR_SPACE = 24;
	// バーと数値の隙間
	static constexpr int TEXT_SPACE = 2;

	// 色
	static constexpr unsigned int HP_COLOR = 0xffd800;		// 黄色
	static constexpr unsigned int MP_COLOR = 0x30d050;		// 緑
	static constexpr unsigned int BACK_COLOR = 0x303030;	// 背景（暗い灰色）
	static constexpr unsigned int TEXT_COLOR = 0xffffff;	// 数値

#pragma endregion

	// 表示するステータス（プレイヤーのものを参照するだけ）
	const CharacterBase::CharacterStatus& status;


	// バー1本の描画（背景 → 中身）
	void DrawBar(int left, int top, int height, int value, int maxValue, unsigned int color) const;

	void DrawValue(int right, int top, int value)const;

};