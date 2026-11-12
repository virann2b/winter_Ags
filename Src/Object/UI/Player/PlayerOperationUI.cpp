#include "PlayerOperationUI.h"

#include "../../Player/Player.h"

#include "../../Skill/SkillBase.h"

#include "../../../Manager/Image/ImageManager.h"


PlayerOperationUI::PlayerOperationUI(const Player& player)
	:
	UIBase(),
	player(player)
{
}

void PlayerOperationUI::Draw(void)
{
	//スキルメニュー中はスキルの操作図
	if (player.IsSkillMenuOpen())
	{
		DrawSkillMenu();
		return;
	}

	//デフォルトの操作方法
	const Img::ImageData& operation_1 = Img::GetIns().GetImage(Img::IMAGE_TAG::Operation_1);
	DrawGraph(1100, 500, operation_1.handle, true);

}

void PlayerOperationUI::DrawSkillMenu(void) const
{
	const Img::ImageData& operation_2 = Img::GetIns().GetImage(Img::IMAGE_TAG::Operation_2);

	//土台（空の丸4つ）
	DrawGraph(1100, 500, operation_2.handle, true);

	//画像の大きさを表示サイズに合わせる倍率
	const float scale = (float)DRAW_SIZE / (operation_2.sizeX);

	const int mp = player.GetStatus().mp;

	for (int i = 0; i < Player::GetSkillSlotMax(); ++i)
	{
		const SkillBase* skill = player.GetSkill(i);
		if (skill == nullptr || skill->GetIconTag() == Img::IMAGE_TAG::Max) { continue; }

		//丸の中心（画像の左上からの位置をそのまま足す）
		const int centerX = DRAW_POS_X + SLOT_CENTER_X[i];
		const int centerY = DRAW_POS_Y + SLOT_CENTER_Y[i];

		const bool isUsable_ = (mp >= skill->GetMpCost()) && !skill->IsActive();
		if (!isUsable_) { SetDrawBright(DISABLE_BRIGHT, DISABLE_BRIGHT, DISABLE_BRIGHT); }

		DrawImageCenter(centerX, centerY, ICON_SIZE, ICON_SIZE,
			Img::GetIns().GetImage(skill->GetIconTag()));

		SetDrawBright(255, 255, 255);
	}
}
