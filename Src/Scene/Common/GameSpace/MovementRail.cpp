#include "MovementRail.h"

#include <algorithm>
#include <cfloat>

#include <DxLib.h>

MovementRail::MovementRail(void) :
	points()
{
}

void MovementRail::AddPoint(const Vector3& point)
{
	points.emplace_back(point);
}

void MovementRail::Clear(void)
{
	points.clear();
}

Vector3 MovementRail::ProjectPosition(const Vector3& pos)const
{
	if (!IsValid()) { return pos; }

	int segmentIndex = 0;
	float ratio = 0.0f;
	GetNearestSegment(pos, segmentIndex, ratio);

	const Vector3& start = points[segmentIndex];
	const Vector3& end = points[segmentIndex + 1];

	return start + (end - start) * ratio;
}

Vector3 MovementRail::GetTangent(const Vector3& pos)const
{
	if (!IsValid()) { return Vector3::Xonly(1.0f); }

	int segmentIndex = 0;
	float ratio = 0.0f;
	GetNearestSegment(pos, segmentIndex, ratio);

	Vector3 tangent = points[segmentIndex + 1] - points[segmentIndex];
	tangent.y = 0.0f;

	if (tangent.LengthSq() <= 0.000001f) { return Vector3::Xonly(1.0f); }

	tangent.Normalize();
	return tangent;
}

void MovementRail::DrawDebug(unsigned int color)const
{
	if (!IsValid()) { return; }

	for (int i = 0; i < static_cast<int>(points.size()) - 1; ++i)
	{
		DrawLine3D(points[i].ToVECTOR(), points[i + 1].ToVECTOR(), color);
		DrawSphere3D(points[i].ToVECTOR(), 10.0f, 8, color, color, false);
	}

	DrawSphere3D(points.back().ToVECTOR(), 10.0f, 8, color, color, false);
}

void MovementRail::GetNearestSegment(const Vector3& pos, int& segmentIndex, float& ratio)const
{
	segmentIndex = 0;
	ratio = 0.0f;

	if (!IsValid()) { return; }

	float nearestDistanceSq = FLT_MAX;

	for (int i = 0; i < static_cast<int>(points.size()) - 1; ++i)
	{
		const Vector3 segment = points[i + 1] - points[i];
		const float segmentLengthSq = segment.LengthSq();

		if (segmentLengthSq <= 0.000001f) { continue; }

		float currentRatio = (pos - points[i]).Dot(segment) / segmentLengthSq;
		currentRatio = (std::max)(0.0f, (std::min)(1.0f, currentRatio));

		const Vector3 nearestPos = points[i] + segment * currentRatio;
		const float distanceSq = (pos - nearestPos).LengthSq();

		if (distanceSq < nearestDistanceSq)
		{
			nearestDistanceSq = distanceSq;
			segmentIndex = i;
			ratio = currentRatio;
		}
	}
}
