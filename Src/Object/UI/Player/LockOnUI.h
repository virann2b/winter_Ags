#pragma once

#include "../UIBase.h"

#include <functional>

class EnemyBase;

class LockOnUI : public UIBase
{
public:

	/// <summary>
	/// ロックオン中の敵にマーカーを表示するUI
	/// </summary>
	/// <param name="getTarget">ロックオン中の敵を返す関数</param>
	LockOnUI(std::function<const EnemyBase* (void)> getTarget);
	~LockOnUI(void) override = default;

	//描画
	void Draw(void) override;

private:

#pragma region 定数

	// マーカーを出す高さ
	static constexpr float LOCK_OFFSET_Y = -30.0f;

	// この距離のとき
	static constexpr float LOCK_BASE_DIST = 500.0f;
	// この大きさで表示する
	static constexpr float LOCK_BASE_SIZE = 96.0f;

	// 遠いときの最小サイズ
	static constexpr float LOCK_MIN_SIZE = 40.0f;
	// 近いときの最大サイズ
	static constexpr float LOCK_MAX_SIZE = 128.0f;

#pragma endregion

	// ロックオン中の敵を取得する関数
	std::function<const EnemyBase* (void)> getTarget;

};
