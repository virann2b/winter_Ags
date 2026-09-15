#pragma once

#include "../Common/GameSpace/WorldSceneBase.h"

class DebugObjectBase;

class TestScene : public WorldSceneBase
{
public:

	TestScene(void);
	~TestScene(void)override = default;

private:

	// オブジェクト生成
	void SubPostLoad(void)override;

	// 初期空間・カメラ設定
	void SubPostInit(void)override;

	// 空間モードと個別制約の切り替え
	void SubWorldPreUpdate(void)override;

	// UI描画
	void SubUiDraw(void)override;

	// レール情報を生成する
	void BuildRail(void);

	// 現在モード文字列
	const char* GetModeName(void)const;

	// 現在制約文字列
	const char* GetConstraintName(void)const;

	// 操作対象
	ActorBase* operatorObject;

	// Railモード確認用
	MovementRail testRail;

	void CreateCamera(void)override;
};
