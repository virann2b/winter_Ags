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

	// 使った弾の暗さ（0～255）
	static constexpr int EMPTY_BRIGHT = 70;

	// 照準を置いた弾（次に撃つ分）の色
	static constexpr int AIM_R = 255;
	static constexpr int AIM_G = 110;
	static constexpr int AIM_B = 110;


	/*static constexpr float CENTER_X = (float)Application::SCREEN_SIZE_X_HALF;

	static constexpr float CENTER_Y = (float)Application::SCREEN_SIZE_Y_HALF;*/

	/*static constexpr float  PI = DX_PI_F;*/


#pragma endregion

	//銃の読み込み用プレイヤー
	const Player& player;

};

