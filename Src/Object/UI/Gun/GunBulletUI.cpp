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

	const Gun& gun = player.GetGun();
	const int maxBullet = gun.GetMaxBullet();

	if (maxBullet <= 0) { return; }

	const int remaim = gun.GetBullet();

	const int aimStart = remaim - gun.GetAim();

	const Img::ImageData& bullet = Img::GetIns().GetImage(Img::IMAGE_TAG::GunBullet);

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	/*for (int i = 0; i < maxBullet; ++i)
	{
		const float rad = -PI * 0.5f + (PI * 2.0f) * i / maxBullet;
		const int x = (int)(CENTER_X + consf)
	}*/

}
