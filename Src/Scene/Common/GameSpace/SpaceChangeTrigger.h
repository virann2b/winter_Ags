#pragma once

#include "../../../Object/Common/ActorBase/ActorBase.h"
#include "GameSpaceController.h"

/// <summary>
/// 進入したActorをきっかけに2D・3D空間を切り替えるギミック。
/// BoxColliderを使用し、表示モデルを持たないTriggerとして動作する。
/// </summary>
class SpaceChangeTrigger : public ActorBase
{
public:
	// Triggerが行う切り替え種類
	enum class CHANGE_TYPE
	{
		Side2D,
		Plane3D,
		Rail,
		Free3D,
	};

	SpaceChangeTrigger(GameSpaceController& gameSpace,
		ActorBase* target,
		CHANGE_TYPE changeType,
		COLLIDER_TAG enterTag,
		const Vector3& pos,
		const Vector3& size,
		const MovementPlane& plane = MovementPlane(),
		const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

	SpaceChangeTrigger(GameSpaceController& gameSpace,
		ActorBase* target,
		COLLIDER_TAG enterTag,
		const Vector3& pos,
		const Vector3& size,
		const MovementRail& rail,
		const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

	~SpaceChangeTrigger(void)override = default;

	void Load(void)override;
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// 一度だけ発動するかどうか
	void SetOneShot(bool flg) { isOneShot = flg; }

private:
	void SubInit(void)override;
	void SubUpdate(void)override {}
	void SubDraw(void)override {}
	void SubAlphaDraw(void)override {}
	void SubRelease(void)override {}

	// 空間変更を実行する
	void ChangeSpace(void);

	GameSpaceController* gameSpace;
	ActorBase* target;
	CHANGE_TYPE changeType;
	COLLIDER_TAG enterTag;
	MovementPlane plane;
	MovementRail rail;
	SpaceTransitionParameter parameter;
	Vector3 triggerSize;
	bool isOneShot;
	bool isUsed;
};
