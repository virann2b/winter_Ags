#pragma once

#include <vector>

#include "SpaceDefine.h"

/// <summary>
/// 曲がった横スクロールステージ等で使用する移動レール。
/// 登録した点同士を直線で結び、最も近い線分への吸着と進行方向の取得を行う。
/// </summary>
class MovementRail
{
public:
	MovementRail(void);
	~MovementRail(void) = default;

	// レール上の点を追加する
	void AddPoint(const Vector3& point);

	// 登録済みの点をすべて削除する
	void Clear(void);

	// レールとして使用可能な点数が登録されているか
	bool IsValid(void)const { return points.size() >= 2; }

	// 指定座標から最も近いレール上の座標を取得する
	Vector3 ProjectPosition(const Vector3& pos)const;

	// 指定座標に最も近いレールの進行方向を取得する
	Vector3 GetTangent(const Vector3& pos)const;

	// デバッグ描画
	void DrawDebug(unsigned int color = 0x00ffff)const;

private:
	// 指定座標に最も近い線分番号と、その線分内の比率を取得する
	void GetNearestSegment(const Vector3& pos, int& segmentIndex, float& ratio)const;

	// レールを構成する点
	std::vector<Vector3> points;
};
