#pragma once

#include"../SceneBase.h"

class EnemyManager;
class EnemyBase;
class GameCamera;

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// UI描画
	void SubUiDraw(void)override;

#pragma endregion

#pragma region 定数

	// ゲームオーバーまでの制限時間(秒)
	static constexpr float TIME_LIMIT_SEC = 120.0f;

	// 最後の敵を倒したときのスロー演出（死亡アニメーションが終わるまで続く）
	static constexpr float CLEAR_SLOW_SCALE = 0.2f;					// スロー中の時間倍率
	static constexpr unsigned short CLEAR_SLOW_CHANGE_TIME = 5;		// スローに入るのにかける時間（フレーム）

#pragma endregion

	GameCamera* gameCamera;
	//ロックオン中の敵
	const EnemyBase* lockedEnemy;   


	// 経過時間(秒)
	float elapsedSec;

	// 遷移済みフラグ(毎フレーム遷移を呼ばないため)
	bool isChanging;

	// クリア時のスロー演出を開始したか（1回だけ行うため）
	bool isClearSlow;

	void CreateCamera(void)override;


};