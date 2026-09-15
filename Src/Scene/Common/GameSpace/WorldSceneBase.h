#pragma once

#include "../../SceneBase.h"
#include "GameSpaceController.h"

/// <summary>
/// 3Dモデルを使用するゲームシーン向けの基底クラス。
/// SceneBaseのライフサイクルへゲーム空間管理を追加し、オブジェクト更新と当たり判定の後に空間制約を適用する。
/// </summary>
class WorldSceneBase : public SceneBase
{
public:
	WorldSceneBase(void);
	virtual ~WorldSceneBase(void)override = default;

protected:
	// ゲーム空間管理クラスを取得する
	GameSpaceController& GetGameSpace(void) { return gameSpace; }
	const GameSpaceController& GetGameSpace(void)const { return gameSpace; }

	// WorldScene派生先の事前更新
	virtual void SubWorldPreUpdate(void) {}

	// オブジェクト更新・当たり判定・空間制約適用後の更新
	virtual void SubWorldPostUpdate(void) {}

	// ゲーム空間のデバッグ描画後に追加したい描画
	virtual void SubWorldDebugDraw(void) {}

	virtual void SubWorldPreObjectAdd(ActorBase& object) {}

	virtual void SubWorldAlphaDraw(void) {}

private:
	// SceneBaseから呼ばれる共通フック
	void SubPreUpdate(void)final;
	void SubPostUpdate(void)final;
	void SubPreObjectAdd(ActorBase& object)final;
	void SubAlphaDraw(void)final;

	// 空間ルール管理
	GameSpaceController gameSpace;
};
