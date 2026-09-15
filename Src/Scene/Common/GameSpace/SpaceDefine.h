#pragma once

#include "../../../Common/Vector3.h"

// ゲーム空間内で使用する移動方式
// 「2Dか3Dか」だけではなく、奥行きのある固定カメラや曲がった道にも対応できるように分けている
enum class GAME_SPACE_MODE
{
	// 横スクロール形式
	// 指定した平面上の横方向だけを操作に使用し、奥行き方向は固定する
	Side2D,

	// 固定された移動平面上を前後左右に移動する
	// 見た目は固定カメラでも、ゲーム上は奥行きを使う場面向け
	Plane3D,

	// 登録されたレールに沿って移動する
	// 曲がった横スクロールステージや円形ステージ向け
	Rail,

	// カメラ基準で自由に前後左右へ移動する
	Free3D,
};

// オブジェクトごとの空間制約（ステージ全体が2Dでも、背景ギミックやボスだけは3Dを自由に使えるようにする）
enum class SPACE_CONSTRAINT
{
	// WorldSceneBaseが持つ現在のゲーム空間設定に従う
	StageDefault,

	// 常に現在の移動平面へ固定する
	FixedPlane,

	// 常にレールへ固定する
	Rail,

	// 空間制約を一切適用しない
	None,
};

// 2Dと3Dの切り替え状態
enum class SPACE_TRANSITION_STATE
{
	// 切り替えを行っていない
	None,

	// 2D系の空間から完全3Dへ切り替え中
	To3D,

	// 完全3Dから2D系の空間へ切り替え中
	To2D,
};

/// <summary>
/// 2D風移動や平面移動に使用する基準情報。
/// ワールドX軸やZ軸へ固定せず、任意方向を横・奥行き・上として扱える。
/// </summary>
struct MovementPlane
{
	// 平面の基準座標
	Vector3 origin;

	// 画面上での横移動方向
	Vector3 right;

	// 平面上での奥行き移動方向
	Vector3 depth;

	// 上方向。通常は{ 0, 1, 0 }
	Vector3 up;

	MovementPlane(void) :
		origin(),
		right(1.0f, 0.0f, 0.0f),
		depth(0.0f, 0.0f, 1.0f),
		up(0.0f, 1.0f, 0.0f)
	{
	}

	MovementPlane(const Vector3& origin, const Vector3& right, const Vector3& depth, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f)) :
		origin(origin),
		right(right),
		depth(depth),
		up(up)
	{
	}
};

/// <summary>
/// 空間切り替え時の共通設定。
/// </summary>
struct SpaceTransitionParameter
{
	// 切り替えに使用するフレーム数
	int time;

	// 切り替え中に対象Actorの座標を補間するかどうか
	bool isMoveTarget;

	// 切り替え完了時の座標
	Vector3 targetPos;

	// 切り替え中にPlayer等の入力を止めるかどうか
	bool isStopInput;

	SpaceTransitionParameter(void) :
		time(30),
		isMoveTarget(true),
		targetPos(),
		isStopInput(true)
	{
	}
};
