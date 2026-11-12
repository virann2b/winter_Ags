#pragma once

#include "../UIBase.h"

#include "../../Skill/ElementDefine.h"

#include <vector>


class EnemyBase;

class EnemyStatusUI : public UIBase
{
public:

	EnemyStatusUI(const std::vector<EnemyBase*>& enemies);

	~EnemyStatusUI() override = default;

	void Draw(void) override;

private:

#pragma region 定数

	// HPバーの大きさ（画面のピクセル）
	static constexpr int BAR_WIDTH = 70;
	static constexpr int BAR_HEIGHT = 8;

	static constexpr int WEAK_DRAW_HEIGHT = 60;
	static constexpr int WEAK_OFFSET_X = -50;

	//弱点属性の枠の大きさ（画面のピクセル）
	static constexpr int ELEMENT_FRAME_SIZE = 50;
	// 枠に対するアイコンの大きさ
	static constexpr float ELEMENT_ICON_RATE = 0.8f;
	// 残り時間バーとの隙間
	static constexpr int ELEMENT_SPACE = 4;

	static constexpr unsigned int HP_COLOR = 0xe03030;		// 赤
	static constexpr unsigned int BACK_COLOR = 0x303030;	// 背景

	// 「弱点！」をバーの上端からどれだけ上に出すか
	static constexpr int WEAK_TEXT_SPACE = 4;

	// 弱点硬直の残り時間バーを画面に表示する横幅
	static constexpr int WEAK_TIME_DRAW_WIDTH = 110;

	// HPバーと残り時間バーの隙間
	static constexpr int WEAK_TIME_BAR_SPACE = 5;


#pragma endregion


	// 敵の一覧（EnemyManagerのものを参照するだけ）
	const std::vector<EnemyBase*>& enemies;

	// 敵1体分の描画
	void DrawEnemy(const EnemyBase& enemy) const;

	//属性に対応する画像取得
	static Img::IMAGE_TAG GetElementTag(ELEMENT element);
};

