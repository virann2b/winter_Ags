#pragma once
#include "../../pch.h"

class UIBase
{
public:

	UIBase(void) = default;
	virtual ~UIBase(void) = default;

	// 初期化（UIManagerのInitで呼ばれる）
	virtual void Init(void) {}
	// 更新（演出の時間を進めるなど）
	virtual void Update(void) {}
	// 描画（必ず派生先で書く）
	virtual void Draw(void) = 0;
	// 解放（画像などを持ったら派生先で消す）
	virtual void Release(void) {}

	// 表示するかの設定・取得
	void SetIsDraw(bool flg) { isDraw = flg; }
	bool GetIsDraw(void) const { return isDraw; }

protected:

	// 表示フラグ
	bool isDraw = true;

};