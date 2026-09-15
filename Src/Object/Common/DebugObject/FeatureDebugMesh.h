#pragma once

#include "../ActorBase/ActorBase.h"

class MeshCollider;

class FeatureDebugMesh : public ActorBase
{
public:

	FeatureDebugMesh(
		const Vector3& pos = Vector3()
	);

	~FeatureDebugMesh(void)override = default;

	// 手作り三角形メッシュとBVHを生成する
	void Load(void)override;

private:

	// メッシュの見た目を描画する
	void SubDraw(void)override;

	// 描画用三角形
	std::vector<Vector3> vertices;
	std::vector<unsigned int> indices;

	// 生成したMeshColliderへの参照
	MeshCollider* meshCollider;
};
