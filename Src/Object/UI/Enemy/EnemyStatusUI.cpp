#include "EnemyStatusUI.h"

#include <cstring>
#include <algorithm>

#include "../../Enemy/EnemyBase.h"

#include "../../../Manager/Image/ImageManager.h"


EnemyStatusUI::EnemyStatusUI(const std::vector<EnemyBase*>& enemies)
	:
	UIBase(),
	enemies(enemies)
{
}

void EnemyStatusUI::Draw(void)
{
	for (const EnemyBase* enemy : enemies)
	{
		//敵がいない、死亡している場合は何もしない
		if (enemy == nullptr || enemy->IsDead()) { continue; }

		DrawEnemy(*enemy);
	}
}

Img::IMAGE_TAG EnemyStatusUI::GetElementTag(ELEMENT element)
{
	switch (element)
	{
	case ELEMENT::Physics:	return Img::IMAGE_TAG::Element_Physics;
	case ELEMENT::Fire:		return Img::IMAGE_TAG::Element_Fire;
	case ELEMENT::Ice:		return Img::IMAGE_TAG::Element_Ice;
	case ELEMENT::Wind:		return Img::IMAGE_TAG::Element_Wind;
	case ELEMENT::Thunder:	return Img::IMAGE_TAG::Element_Thunder;
	default:				return Img::IMAGE_TAG::Max;	//属性無し
	}
}

void EnemyStatusUI::DrawEnemy(const EnemyBase& enemy) const
{
	//頭の上を画面座標に変換
	const VECTOR screen = ConvWorldPosToScreenPos(enemy.GetUIPos().ToVECTOR());

	//カメラの後ろ側なら描かない
	if (screen.z <= 0.0f || screen.z >= 1.0f) { return; }

	//バーの左上（中心をUI位置に合わせる）
	const int left = (int)screen.x - BAR_WIDTH / 2;
	const int top = (int)screen.y - BAR_HEIGHT / 2;

	//背景
	DrawBox(left, top, left + BAR_WIDTH, top + BAR_HEIGHT, BACK_COLOR, true);

	//HPの割合分
	const CharacterBase::CharacterStatus& status = enemy.GetStatus();
	const float rate = GetRate((float)status.hp, (float)status.maxHp);
	DrawBox(left, top, left + (int)(BAR_WIDTH * rate), top + BAR_HEIGHT, HP_COLOR, true);

	
	//弱点硬直中は「弱点！」
	if (enemy.IsWeakStun())
	{
		//弱点画像
		const Img::ImageData& weak = Img::GetIns().GetImage(Img::IMAGE_TAG::Weak);
		const Img::ImageData& weakFrame = Img::GetIns().GetImage(Img::IMAGE_TAG::WeakFrame);
		const Img::ImageData& weakFill = Img::GetIns().GetImage(Img::IMAGE_TAG::WeakFill);


		//残り時間バー（HPバーの下・中身は枠の中央に置かれる）
		const int timeTop = top + BAR_HEIGHT + WEAK_TIME_BAR_SPACE;
		const int timeH = DrawFrameGauge(weakFrame, weakFill,
			(int)screen.x, timeTop, WEAK_TIME_DRAW_WIDTH, enemy.GetWeakStunRate());

		//弱点属性（残り時間バーの左隣・縦の中心をそろえる）
		const Img::IMAGE_TAG elementTag = GetElementTag(enemy.GetWeakStunElement());
		if (elementTag != Img::IMAGE_TAG::Max)
		{
			const int timeLeft = (int)screen.x - WEAK_TIME_DRAW_WIDTH / 2;
			const int iconX = timeLeft - ELEMENT_SPACE - ELEMENT_FRAME_SIZE / 2;
			const int iconY = timeTop + timeH / 2;
			const int iconSize = (int)(ELEMENT_FRAME_SIZE * ELEMENT_ICON_RATE);

			//枠
			DrawImageCenter(iconX, iconY, ELEMENT_FRAME_SIZE, ELEMENT_FRAME_SIZE,
				Img::GetIns().GetImage(Img::IMAGE_TAG::WeakElementFrame));
			//アイコン
			DrawImageCenter(iconX, iconY, iconSize, iconSize,
				Img::GetIns().GetImage(elementTag));
		}

		//表示サイズ
		const float scale = (float)WEAK_DRAW_HEIGHT / (float)weak.sizeY;
		//表示位置
		const int drawH = (int)(weak.sizeY * scale);

		DrawRotaGraph((int)screen.x + WEAK_OFFSET_X, top - WEAK_TEXT_SPACE - drawH / 2, scale, 0.0f, weak.handle, true);
	}

}
