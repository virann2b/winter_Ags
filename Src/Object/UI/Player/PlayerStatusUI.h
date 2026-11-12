#pragma once

#include "../UIBase.h"

#include "../../Common/CharacterBase/CharacterBase.h"

#include "../../../Manager/Image/ImageManager.h"

class PlayerStatusUI : public UIBase
{

public:

	PlayerStatusUI(const CharacterBase::CharacterStatus& status);
	~PlayerStatusUI(void) override = default;

	void Init(void) override;

	// 描画
	void Draw(void) override;

private:

#pragma region 定数

	// バーの横幅
	static constexpr int BAR_WIDTH = 480;
	// HPバーの高さ
	static constexpr int HP_BAR_HEIGHT = 14;

	// HP・MP枠を画面に表示する横幅
	static constexpr int FRAME_DRAW_WIDTH = 480;

	// HPFrame.pngの中で枠として使う部分（画像のピクセル）
	static constexpr int HP_FRAME_SRC_X = 214;
	static constexpr int HP_FRAME_SRC_Y = 411;
	static constexpr int HP_FRAME_SRC_W = 1250;
	static constexpr int HP_FRAME_SRC_H = 91;

	// HPFrame.pngの中でバーを置く位置（画像のピクセル）
	static constexpr int HP_FILL_X = 363;
	static constexpr int HP_FILL_Y = 432;


	// MPFrame.pngの中で枠として使う部分（画像のピクセル）
	static constexpr int MP_FRAME_SRC_X = 147;
	static constexpr int MP_FRAME_SRC_Y = 297;
	static constexpr int MP_FRAME_SRC_W = 1878;
	static constexpr int MP_FRAME_SRC_H = 133;

	// MPFrame.pngの中でバーを置く範囲（画像のピクセル）
	static constexpr int MP_FILL_X = 361;
	static constexpr int MP_FILL_Y = 326;
	static constexpr int MP_FILL_W = 1638;
	static constexpr int MP_FILL_H = 71;

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

	void DrawValue(int right, int top, int value)const;

	// 画像のバーを割合分描く
	void DrawGaugeImage(int left, int top, int width, int height,
		int value, int maxValue, const Img::ImageData& image) const;

};