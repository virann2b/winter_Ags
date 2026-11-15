#pragma once
#include "../UIBase.h"

class Player;

class GunBulletUI : public UIBase
{
public:

	GunBulletUI(const Player& player);
	~GunBulletUI() override = default;

	//描画
	void Draw(void) override;

private:

#pragma region 定数

	// 円の半径（画面のピクセル）
	static constexpr float RING_RADIUS = 150.0f;
	// 弾1発の表示サイズ（画面のピクセル）
	static constexpr int BULLET_SIZE = 16;


	// 照準を置いた弾（次に撃つ分）の色
	static constexpr int AIM_R = 255;
	static constexpr int AIM_G = 110;
	static constexpr int AIM_B = 110;

#pragma endregion

	//銃の読み込み用プレイヤー
	const Player& player;

};

