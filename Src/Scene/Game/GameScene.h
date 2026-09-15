#pragma once

#include "../Common/GameSpace/WorldSceneBase.h"

class GameScene : public WorldSceneBase
{
public:
	GameScene();
	~GameScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 更新
	void SubWorldPostUpdate(void)override;

	// UI描画
	void SubUiDraw(void)override;

#pragma endregion

	void CreateCamera(void)override;
};