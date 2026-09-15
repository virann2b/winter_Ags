#include "GameSpaceController.h"

#include <algorithm>

#include "../../../Common/Vector2.h"
#include "../../../Object/Common/ActorBase/ActorBase.h"

GameSpaceController::GameSpaceController(void) :
	mode(GAME_SPACE_MODE::Side2D),
	nextMode(GAME_SPACE_MODE::Side2D),
	plane(),
	rail(),
	transitionState(SPACE_TRANSITION_STATE::None),
	transitionParameter(),
	transitionCounter(0),
	transitionTarget(nullptr),
	transitionStartPos()
{
	NormalizePlane();
}

void GameSpaceController::Update(void)
{
	if (!IsTransition()) { return; }

	transitionCounter++;

	const int transitionTime = (std::max)(1, transitionParameter.time);
	const float ratio = (std::min)(1.0f, static_cast<float>(transitionCounter) / static_cast<float>(transitionTime));

	// 3Dから2Dへ戻る際など、瞬間移動を避けるため指定座標へ補間する
	if (transitionTarget != nullptr && transitionParameter.isMoveTarget) {
		transitionTarget->SetPos(Lerp(transitionStartPos, transitionParameter.targetPos, ratio));
	}

	if (transitionCounter >= transitionTime) { FinishTransition(); }
}

void GameSpaceController::DrawDebug(void)const
{
	if (mode == GAME_SPACE_MODE::Rail) { rail.DrawDebug(); }
}

void GameSpaceController::ChangeModeSide2D(const MovementPlane& plane)
{
	this->plane = plane;
	NormalizePlane();
	mode = GAME_SPACE_MODE::Side2D;
	nextMode = mode;
	transitionState = SPACE_TRANSITION_STATE::None;
	transitionTarget = nullptr;
}

void GameSpaceController::ChangeModePlane3D(const MovementPlane& plane)
{
	this->plane = plane;
	NormalizePlane();
	mode = GAME_SPACE_MODE::Plane3D;
	nextMode = mode;
	transitionState = SPACE_TRANSITION_STATE::None;
	transitionTarget = nullptr;
}

void GameSpaceController::ChangeModeRail(const MovementRail& rail)
{
	this->rail = rail;
	mode = GAME_SPACE_MODE::Rail;
	nextMode = mode;
	transitionState = SPACE_TRANSITION_STATE::None;
	transitionTarget = nullptr;
}

void GameSpaceController::ChangeModeFree3D(void)
{
	mode = GAME_SPACE_MODE::Free3D;
	nextMode = mode;
	transitionState = SPACE_TRANSITION_STATE::None;
	transitionTarget = nullptr;
}

void GameSpaceController::StartChangeTo3D(ActorBase* target, const SpaceTransitionParameter& parameter)
{
	StartTransition(GAME_SPACE_MODE::Free3D, target, parameter);
}

void GameSpaceController::StartChangeToSide2D(ActorBase* target, const MovementPlane& plane, const SpaceTransitionParameter& parameter)
{
	this->plane = plane;
	NormalizePlane();
	StartTransition(GAME_SPACE_MODE::Side2D, target, parameter);
}

void GameSpaceController::StartChangeToPlane3D(ActorBase* target, const MovementPlane& plane, const SpaceTransitionParameter& parameter)
{
	this->plane = plane;
	NormalizePlane();
	StartTransition(GAME_SPACE_MODE::Plane3D, target, parameter);
}

void GameSpaceController::StartChangeToRail(ActorBase* target, const MovementRail& rail, const SpaceTransitionParameter& parameter)
{
	this->rail = rail;
	StartTransition(GAME_SPACE_MODE::Rail, target, parameter);
}

Vector3 GameSpaceController::ConvertMoveInput(const Vector2& input, const Vector3& objectPos, const Vector3& cameraPos, SPACE_CONSTRAINT constraint)const
{
	if (IsStopInput()) { return Vector3(); }

	GAME_SPACE_MODE applyMode = mode;

	if (constraint == SPACE_CONSTRAINT::FixedPlane) { applyMode = GAME_SPACE_MODE::Side2D; }
	if (constraint == SPACE_CONSTRAINT::Rail) { applyMode = GAME_SPACE_MODE::Rail; }
	if (constraint == SPACE_CONSTRAINT::None) { applyMode = GAME_SPACE_MODE::Plane3D; }

	Vector3 direction;

	switch (applyMode) {

	case GAME_SPACE_MODE::Side2D: {
		// 横スクロール中は横入力だけを使用する
		direction = plane.right * input.x;
		break;
	}

	case GAME_SPACE_MODE::Plane3D: {
		// 固定平面上では横と奥行きの両方を使用する
		direction = plane.right * input.x + plane.depth * input.y;
		break;
	}

	case GAME_SPACE_MODE::Rail: {
		// レール上ではスティック横方向をレールの接線方向へ変換する
		direction = rail.GetTangent(objectPos) * input.x;
		break;
	}

	case GAME_SPACE_MODE::Free3D: {
		// 完全3Dではカメラからオブジェクトへ向かう地面方向を前方向として使用する
		Vector3 cameraForward = objectPos - cameraPos;
		cameraForward.y = 0.0f;

		if (cameraForward.LengthSq() <= 0.000001f) { cameraForward = Vector3::Zonly(1.0f); }
		else { cameraForward.Normalize(); }

		Vector3 cameraRight = Vector3::Yonly(1.0f).Cross(cameraForward);
		if (cameraRight.LengthSq() > 0.000001f) { cameraRight.Normalize(); }

		direction = cameraRight * input.x + cameraForward * input.y;
		break;
	}
	}

	if (direction.LengthSq() > 1.0f) { direction.Normalize(); }
	return direction;
}

Vector3 GameSpaceController::RestrictDirection(const Vector3& direction, const Vector3& objectPos)const
{
	return RestrictDirection(direction, objectPos, SPACE_CONSTRAINT::StageDefault);
}

Vector3 GameSpaceController::RestrictDirection(const Vector3& direction, const Vector3& objectPos, SPACE_CONSTRAINT constraint)const
{
	// 空間制約を使用しないオブジェクトは、元の3D方向をそのまま返す
	if (constraint == SPACE_CONSTRAINT::None) { return direction; }

	GAME_SPACE_MODE applyMode = mode;

	// オブジェクト固有の制約が設定されている場合は、現在のステージ設定より優先する
	if (constraint == SPACE_CONSTRAINT::FixedPlane) { applyMode = GAME_SPACE_MODE::Side2D; }
	if (constraint == SPACE_CONSTRAINT::Rail) { applyMode = GAME_SPACE_MODE::Rail; }

	Vector3 ret = direction;

	switch (applyMode)
	{
	case GAME_SPACE_MODE::Side2D:
		ret = plane.right * ret.Dot(plane.right) + plane.up * ret.Dot(plane.up);
		break;

	case GAME_SPACE_MODE::Plane3D:
		ret = plane.right * ret.Dot(plane.right) + plane.depth * ret.Dot(plane.depth) + plane.up * ret.Dot(plane.up);
		break;

	case GAME_SPACE_MODE::Rail:
	{
		const Vector3 tangent = rail.GetTangent(objectPos);
		ret = tangent * ret.Dot(tangent) + plane.up * ret.Dot(plane.up);
		break;
	}

	case GAME_SPACE_MODE::Free3D:
		break;
	}

	return ret;
}

Vector3 GameSpaceController::GetDirection(const Vector3& from, const Vector3& to)const
{
	Vector3 direction = RestrictDirection(to - from, from);
	if (direction.LengthSq() > 0.000001f) { direction.Normalize(); }
	return direction;
}

float GameSpaceController::GetDistance(const Vector3& from, const Vector3& to)const
{
	return RestrictDirection(to - from, from).Length();
}

Vector3 GameSpaceController::RestrictPosition(const Vector3& pos, SPACE_CONSTRAINT constraint)const
{
	if (constraint == SPACE_CONSTRAINT::None) { return pos; }

	GAME_SPACE_MODE applyMode = mode;
	if (constraint == SPACE_CONSTRAINT::FixedPlane) { applyMode = GAME_SPACE_MODE::Side2D; }
	if (constraint == SPACE_CONSTRAINT::Rail) { applyMode = GAME_SPACE_MODE::Rail; }

	switch (applyMode) {

	case GAME_SPACE_MODE::Side2D: {
		// 横方向と上方向の成分だけを残し、奥行き方向を平面の基準へ戻す
		const Vector3 diff = pos - plane.origin;
		return plane.origin + plane.right * diff.Dot(plane.right) + plane.up * diff.Dot(plane.up);
	}

	case GAME_SPACE_MODE::Plane3D: {
		// 平面を構成する横・奥行き・上の成分へ正規化する
		const Vector3 diff = pos - plane.origin;
		return plane.origin + plane.right * diff.Dot(plane.right) + plane.depth * diff.Dot(plane.depth) + plane.up * diff.Dot(plane.up);
	}

	case GAME_SPACE_MODE::Rail: {
		// 高さは維持し、XZ方向だけレールへ吸着する
		Vector3 projected = rail.ProjectPosition(pos);
		projected.y = pos.y;
		return projected;
	}

	case GAME_SPACE_MODE::Free3D: { return pos; }
	}

	return pos;
}

void GameSpaceController::ApplyConstraint(ActorBase& object)const
{
	const SPACE_CONSTRAINT constraint = object.GetSpaceConstraint();
	if (constraint == SPACE_CONSTRAINT::None) { return; }

	const Vector3 restrictedPos = RestrictPosition(object.GetTrans().pos, constraint);
	object.SetPos(restrictedPos);

	// 衝突押し出しや加速度で奥行き方向の速度が残らないように、加速度も同じ空間へ制限する
	object.RestrictVelocity();
}

void GameSpaceController::NormalizePlane(void)
{
	if (plane.up.LengthSq() <= 0.000001f) { plane.up = Vector3::Yonly(1.0f); }
	plane.up.Normalize();

	if (plane.right.LengthSq() <= 0.000001f) { plane.right = Vector3::Xonly(1.0f); }

	// rightからup方向の成分を除き、直交する横方向にする
	plane.right -= plane.up * plane.right.Dot(plane.up);
	if (plane.right.LengthSq() <= 0.000001f) { plane.right = Vector3::Xonly(1.0f); }
	plane.right.Normalize();

	// depthはrightとupから作り直し、常に直交する基準にする
	plane.depth = plane.right.Cross(plane.up);
	if (plane.depth.LengthSq() <= 0.000001f) { plane.depth = Vector3::Zonly(1.0f); }
	plane.depth.Normalize();
}

void GameSpaceController::StartTransition(GAME_SPACE_MODE nextMode, ActorBase* target, const SpaceTransitionParameter& parameter)
{
	// 切り替え中の多重要求は受け付けない
	if (IsTransition()) { return; }

	this->nextMode = nextMode;
	transitionParameter = parameter;
	transitionParameter.time = (std::max)(1, transitionParameter.time);
	transitionCounter = 0;
	transitionTarget = target;
	transitionStartPos = (target != nullptr) ? target->GetTrans().pos : Vector3();

	transitionState = (nextMode == GAME_SPACE_MODE::Free3D) ?
		SPACE_TRANSITION_STATE::To3D : SPACE_TRANSITION_STATE::To2D;
}

void GameSpaceController::FinishTransition(void)
{
	mode = nextMode;
	transitionState = SPACE_TRANSITION_STATE::None;
	transitionCounter = 0;

	// 最終フレームでは誤差なく目標座標へ合わせる
	if (transitionTarget != nullptr && transitionParameter.isMoveTarget)
	{
		transitionTarget->SetPos(transitionParameter.targetPos);
	}

	// 2D系へ戻った場合、切り替え完了時に必ず空間制約を適用する
	if (transitionTarget != nullptr && mode != GAME_SPACE_MODE::Free3D)
	{
		ApplyConstraint(*transitionTarget);
	}

	transitionTarget = nullptr;
}

Vector3 GameSpaceController::Lerp(const Vector3& start, const Vector3& end, float ratio)
{
	ratio = (std::max)(0.0f, (std::min)(1.0f, ratio));
	return start + (end - start) * ratio;
}
