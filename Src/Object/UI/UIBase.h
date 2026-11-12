#pragma once
#include "../../pch.h"

#include "../../Manager/Image/ImageManager.h"

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

	/// <summary>
	/// 画像を左から割合分だけ切り取って描く（ゲージの中身用）
	/// </summary>
	/// <param name="left">描く範囲の左端（画面）</param>
	/// <param name="top">描く範囲の上端（画面）</param>
	/// <param name="width">満タンのときの幅（画面）</param>
	/// <param name="height">高さ（画面）</param>
	/// <param name="rate">残りの割合（0～1）</param>
	/// <param name="image">中身の画像</param>
	void DrawFillRate(int left, int top, int width, int height,
		float rate, const Img::ImageData& image) const;

	/// <summary>
	/// 枠付きゲージを描く（中身は枠の中央に、枠と同じ倍率で置く）
	/// </summary>
	/// <param name="frame">枠の画像</param>
	/// <param name="fill">中身の画像</param>
	/// <param name="centerX">ゲージの中心X（画面）</param>
	/// <param name="top">ゲージの上端（画面）</param>
	/// <param name="drawWidth">枠を表示する幅（高さは比率で決まる）</param>
	/// <param name="rate">残りの割合（0～1）</param>
	/// <returns>描いた枠の高さ（画面）</returns>
	int DrawFrameGauge(const Img::ImageData& frame, const Img::ImageData& fill,
		int centerX, int top, int drawWidth, float rate) const;

	/// <summary>
	/// 画像を中心に合わせて、指定の大きさで描く
	/// </summary>
	void DrawImageCenter(int centerX, int centerY, int width, int height,
		const Img::ImageData& image) const;

	/// <summary>
	/// 残りの割合を求める（最大値が0以下なら0・0～1におさめる）
	/// </summary>
	static float GetRate(float value, float maxValue);
};