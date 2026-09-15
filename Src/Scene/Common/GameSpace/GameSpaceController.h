#pragma once

#include "MovementRail.h"

struct Vector2;
class ActorBase;

/// 2D風移動・平面移動・レール移動・完全3D移動を一元管理するクラス
class GameSpaceController
{
public:
	GameSpaceController(void);
	~GameSpaceController(void) = default;

#pragma region 基本更新

	// 切り替え演出や対象Actorの座標補間を更新する
	void Update(void);

	// デバッグ描画
	void DrawDebug(void)const;

#pragma endregion

#pragma region 空間モード変更

	// 横スクロール形式へ即時変更する
	void ChangeModeSide2D(const MovementPlane& plane = MovementPlane());

	// 固定平面上の3D移動へ即時変更する
	void ChangeModePlane3D(const MovementPlane& plane = MovementPlane());

	// レール移動へ即時変更する
	void ChangeModeRail(const MovementRail& rail);

	// 完全3D移動へ即時変更する
	void ChangeModeFree3D(void);

	// 指定時間をかけて完全3Dへ変更する
	void StartChangeTo3D(ActorBase* target, const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

	// 指定時間をかけて横スクロール形式へ変更する
	void StartChangeToSide2D(ActorBase* target, const MovementPlane& plane, const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

	// 指定時間をかけて平面3Dへ変更する
	void StartChangeToPlane3D(ActorBase* target, const MovementPlane& plane, const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

	// 指定時間をかけてレール移動へ変更する
	void StartChangeToRail(ActorBase* target, const MovementRail& rail, const SpaceTransitionParameter& parameter = SpaceTransitionParameter());

#pragma endregion

#pragma region 入力・方向変換

	/// <summary>
	/// スティック入力を現在のゲーム空間に適したワールド方向へ変換する。
	/// FREE_3D時だけcameraPosを使用し、それ以外は平面またはレールの基準方向を使用する。
	/// </summary>
	Vector3 ConvertMoveInput(const Vector2& input, const Vector3& objectPos, const Vector3& cameraPos, SPACE_CONSTRAINT constraint = SPACE_CONSTRAINT::StageDefault)const;

	// 攻撃方向や回避方向を現在の空間へ合わせる
	Vector3 RestrictDirection(const Vector3& direction, const Vector3& objectPos)const;

	// Actor固有の空間制約を考慮して方向を補正する
	Vector3 RestrictDirection(const Vector3& direction, const Vector3& objectPos, SPACE_CONSTRAINT constraint)const;

	// 2点間の方向を現在の空間上で取得する。Enemyの追尾や弾の発射方向向け
	Vector3 GetDirection(const Vector3& from, const Vector3& to)const;

	// 現在の空間上での2点間距離を取得する。Enemyの索敵距離判定向け
	float GetDistance(const Vector3& from, const Vector3& to)const;

#pragma endregion

#pragma region 座標制約

	// 現在の空間設定に合わせて座標を補正する
	Vector3 RestrictPosition(const Vector3& pos, SPACE_CONSTRAINT constraint = SPACE_CONSTRAINT::StageDefault)const;

	// オブジェクトへ現在の空間制約を適用する
	void ApplyConstraint(ActorBase& object)const;

#pragma endregion

#pragma region ゲット関数

	GAME_SPACE_MODE GetMode(void)const { return mode; }
	SPACE_TRANSITION_STATE GetTransitionState(void)const { return transitionState; }
	const MovementPlane& GetPlane(void)const { return plane; }
	MovementRail& GetRail(void) { return rail; }
	const MovementRail& GetRail(void)const { return rail; }

	// 空間切り替え中か
	bool IsTransition(void)const { return transitionState != SPACE_TRANSITION_STATE::None; }

	// 切り替え中に入力を停止する設定か
	bool IsStopInput(void)const { return IsTransition() && transitionParameter.isStopInput; }

#pragma endregion

private:
	// 平面の各基準軸を安全な単位ベクトルへ補正する
	void NormalizePlane(void);

	// 空間切り替えを共通開始する
	void StartTransition(GAME_SPACE_MODE nextMode, ActorBase* target, const SpaceTransitionParameter& parameter);

	// 切り替え完了時に次のモードを反映する
	void FinishTransition(void);

	// 0～1の線形補間
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float ratio);

	// 現在の移動方式
	GAME_SPACE_MODE mode;

	// 切り替え完了後の移動方式
	GAME_SPACE_MODE nextMode;

	// 平面移動用情報
	MovementPlane plane;

	// レール移動用情報
	MovementRail rail;

	// 切り替え状態
	SPACE_TRANSITION_STATE transitionState;

	// 切り替え設定
	SpaceTransitionParameter transitionParameter;

	// 切り替え経過フレーム
	int transitionCounter;

	// 座標補間対象
	ActorBase* transitionTarget;

	// 補間開始座標
	Vector3 transitionStartPos;
};
