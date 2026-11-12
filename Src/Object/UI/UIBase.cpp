#include "UIBase.h"

#include <algorithm>

float UIBase::GetRate(float value, float maxValue)
{
	//最大値が0以下なら0（0除算防止）
	if (maxValue <= 0.0f) { return 0.0f; }

	//0～1におさめる
	return std::clamp(value / maxValue, 0.0f, 1.0f);
}

void UIBase::DrawFillRate(int left, int top, int width, int height,
	float rate, const Img::ImageData& image) const
{
	//割合を0～1におさめる
	rate = std::clamp(rate, 0.0f, 1.0f);

	//画像の左から割合分の幅（0なら描かない）
	const int srcW = (int)(image.sizeX * rate);
	if (srcW <= 0) { return; }

	//画像の左から切り取って、画面にも割合分の幅で描く
	DrawRectExtendGraph(
		left, top, left + (int)(width * rate), top + height,
		0, 0, srcW, image.sizeY,
		image.handle, true);
}

int UIBase::DrawFrameGauge(const Img::ImageData& frame, const Img::ImageData& fill,
	int centerX, int top, int drawWidth, float rate) const
{
	if (frame.sizeX <= 0) { return 0; }

	//枠の画像を表示したい幅に合わせる倍率
	const float scale = (float)drawWidth / (float)frame.sizeX;

	//枠の表示サイズと位置（中心をそろえる）
	const int frameW = drawWidth;
	const int frameH = (int)(frame.sizeY * scale);
	const int frameLeft = centerX - frameW / 2;

	//枠
	DrawExtendGraph(frameLeft, top, frameLeft + frameW, top + frameH, frame.handle, true);

	//中身の表示サイズ（枠と同じ倍率）
	const int fillW = (int)(fill.sizeX * scale);
	const int fillH = (int)(fill.sizeY * scale);

	//中身は枠の中央に置く
	const int fillLeft = centerX - fillW / 2;
	const int fillTop = top + (frameH - fillH) / 2;

	//中身（割合分）
	DrawFillRate(fillLeft, fillTop, fillW, fillH, rate, fill);

	return frameH;
}

void UIBase::DrawImageCenter(int centerX, int centerY, int width, int height, const Img::ImageData& image) const
{
	const int left = centerX - width / 2;
	const int top = centerY - height / 2;

	DrawExtendGraph(left, top, left + width, top + height, image.handle, true);
}
