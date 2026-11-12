#pragma once
#include "../UIBase.h"

class Player;

class PlayerOperationUI : public UIBase
{

public:

	PlayerOperationUI(const Player& player);
	~PlayerOperationUI(void) override = default;

	// 描画
	void Draw(void) override;

private:

#pragma region 定数


	// 操作図の表示位置（左上）と表示サイズ
	static constexpr int DRAW_POS_X = 1100;
	static constexpr int DRAW_POS_Y = 500;
	static constexpr int DRAW_SIZE = 300;

	// 各スロットの丸の中心
	// 並びはスキルスロットの番号順・操作のキーに合わせて並べる
	// Y, X, B, A
	static constexpr int SLOT_CENTER_X[4] = { 150,  55, 245, 150 };
	static constexpr int SLOT_CENTER_Y[4] = { 55, 150, 150, 245 };
	// 丸の中に描くアイコンの大きさ
	static constexpr int ICON_SIZE = 68;

	// 使えないときの暗さ（0～255）
	static constexpr int DISABLE_BRIGHT = 90;

#pragma endregion

	//スロット取得用プレイヤー
	const Player& player;

	// スキルメニューの描画
	void DrawSkillMenu(void) const;
};

