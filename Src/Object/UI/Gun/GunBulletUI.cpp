#include "GunBulletUI.h"

#include <cmath>

#include "../../Player/Player.h"
#include "../../Player/Wepon/Gun/Gun.h"
#include "../../../Application/Application.h"

GunBulletUI::GunBulletUI(const Player& player)
	:
	UIBase(),
	player(player)
{
}

void GunBulletUI::Draw(void)
{
	//e‚ğ\‚¦‚Ä‚¢‚é‚Ì‚İ
	if (!player.IsAiming()) { return; }

	//e‚ğæ“¾
	const Gun& gun = player.GetGun();
	//Å‘å’e”
	const int maxBullet = gun.GetMaxBullet();


	if (maxBullet <= 0) { return; }

	//c’e”
	const int remain = gun.GetBullet();
	//g—p‚µ‚½’e”
	const int used = maxBullet - remain;
	//ËŒ‚‚ÌÅŒã‚Ì’e”Ô†
	const int aimEnd = used + gun.GetAim();

	//’eŠÛ‰æ‘œ
	const Img::ImageData& bullet = Img::GetIns().GetImage(Img::IMAGE_TAG::GunBullet);
	//‹ó’e‰æ‘œ
	const Img::ImageData& empty = Img::GetIns().GetImage(Img::IMAGE_TAG::GunBulletEmpty);

	//‰~‚Ì’†‰›À•W
	const float CENTER_X = (float)Application::SCREEN_SIZE_X_HALF;
	const float CENTER_Y = (float)Application::SCREEN_SIZE_Y_HALF;
	
	const float PI = DX_PI_F;

	//k¬‚µ‚½‚Æ‚«‚ÉƒMƒUƒMƒU‚µ‚È‚¢—p
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	for (int i = 0; i < maxBullet; ++i)
	{
		const float rad = -PI * 0.5f + (PI * 2.0f) * i / maxBullet;

		const int x = (int)(CENTER_X + cosf(rad) * RING_RADIUS);
		const int y = (int)(CENTER_Y + sinf(rad) * RING_RADIUS);

		if (i < used)
		{
			//g‚Á‚½’e‚ÍˆÃ‚­
			DrawImageCenter(x, y, BULLET_SIZE, BULLET_SIZE, empty);
			continue;
		}

		//Æ€‚ğ’u‚¢‚½’eiŸ‚ÉŒ‚‚Â•ªj‚ÍÔ‚Á‚Û‚­
		if (i < aimEnd){ SetDrawBright(AIM_R, AIM_G, AIM_B);}

		//’eŠÛ‚Ì•`‰æ
		DrawImageCenter(x, y, BULLET_SIZE, BULLET_SIZE, bullet);

		//–¾‚é‚³‚ğ–ß‚·
		SetDrawBright(255, 255, 255);
	}

	//•`‰æƒ‚[ƒh‚ğ–ß‚·
	SetDrawMode(DX_DRAWMODE_NEAREST);
}
