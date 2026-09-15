#include "FeatureDebugMesh.h"

#include "../../../pch.h"

#include "../Collider/MeshCollider.h"

FeatureDebugMesh::FeatureDebugMesh(const Vector3& pos) :
	ActorBase(),
	vertices(),
	indices(),
	meshCollider(nullptr)
{
	trans.pos = pos;

	SetDynamicFlg(false);
	SetGravityFlg(false);
	SetPushFlg(true);
	SetPushWeight(100);
	SetSpaceConstraint(SPACE_CONSTRAINT::None);
}

void FeatureDebugMesh::Load(void)
{
	// 床と坂を組み合わせた簡単な三角形メッシュ
	vertices =
	{
		Vector3(-350.0f, 0.0f, -250.0f),
		Vector3( 350.0f, 0.0f, -250.0f),
		Vector3(-350.0f, 0.0f,  250.0f),
		Vector3( 350.0f, 0.0f,  250.0f),

		Vector3( 350.0f,   0.0f, -250.0f),
		Vector3( 700.0f, 180.0f, -250.0f),
		Vector3( 350.0f,   0.0f,  250.0f),
		Vector3( 700.0f, 180.0f,  250.0f),
	};

	indices =
	{
		0, 2, 1,
		1, 2, 3,

		4, 6, 5,
		5, 6, 7,
	};

	meshCollider = new MeshCollider(COLLIDER_TAG::DebugObject);

	meshCollider->Setup(vertices, indices);

	ColliderCreate(meshCollider);
}

void FeatureDebugMesh::SubDraw(void)
{
	for (std::size_t i = 0; i + 2 < indices.size(); i += 3) {
		const Vector3 p0 = trans.pos + vertices[indices[i + 0]];

		const Vector3 p1 = trans.pos + vertices[indices[i + 1]];

		const Vector3 p2 = trans.pos + vertices[indices[i + 2]];

		DrawTriangle3D(p0.ToVECTOR(), p1.ToVECTOR(), p2.ToVECTOR(), 0x6688ff, true);
	}
}
