#include "LockOnUI.h"

#include <algorithm>

#include "../../Enemy/EnemyBase.h"

#include "../../../Manager/Image/ImageManager.h"


LockOnUI::LockOnUI(std::function<const EnemyBase* (void)> getTarget)
	:
	UIBase(),
	getTarget(getTarget)
{
}

void LockOnUI::Draw(void)
{
	if (!getTarget) { return; }

	//ロックオン中の敵
	const EnemyBase* target = getTarget();

	//敵がいない・死亡した場合は何もしない
	if (target == nullptr || target->IsDead()) { return; }

	//マーカーを出すワールド座標（敵の体の中心あたり）
	const VECTOR lockPos = VAdd(target->GetTrans().pos.ToVECTOR(), VGet(0.0f, LOCK_OFFSET_Y, 0.0f));

	//画面座標に変換
	const VECTOR screen = ConvWorldPosToScreenPos(lockPos);

	//カメラの後ろ側なら描かない
	if (screen.z <= 0.0f || screen.z >= 1.0f) { return; }

	//カメラからの距離
	const float dist = VSize(VSub(lockPos, GetCameraPosition()));
	if (dist <= 0.0f) { return; }

	//距離に反比例させて、最小～最大の間に収める
	const float size = std::clamp(LOCK_BASE_SIZE * (LOCK_BASE_DIST / dist), LOCK_MIN_SIZE, LOCK_MAX_SIZE);

	//ロックオン画像
	const Img::ImageData& lock = Img::GetIns().GetImage(Img::IMAGE_TAG::LockOn);
	if (lock.handle == -1) { return; }

	//縮小してもギザギザしないように
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//敵の位置を中心に描画
	DrawImageCenter((int)screen.x, (int)screen.y, (int)size, (int)size, lock);

	//描画モードを戻す
	SetDrawMode(DX_DRAWMODE_NEAREST);
}
