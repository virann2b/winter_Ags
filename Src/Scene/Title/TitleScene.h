#pragma once

#include "../SceneBase.h"
#include "../../Common/Vector2I.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene()override = default;

private:

	void SubPreLoad(void)override;
	void SubPreInit(void)override;
	void SubPreUpdate(void)override;
	void SubPostUpdate(void)override;
	void SubUiDraw(void)override;

	enum class TitleState
	{
		Title = 5,	// タイトル画面
		Movie = 10,	// ロゴの後ろでゲームムービーを流す
		MoveToDemo,	// デモシーンへ遷移
	};

	// タイトルロゴの横幅
	static constexpr int TITLE_LOGO_WIDTH = 552;
	// タイトルロゴの縦幅
	static constexpr int TITLE_LOGO_HEIGHT = 106;
	// タイトルロゴの横幅の半分
	static constexpr int TITLE_LOGO_WIDTH_HALF = 276;
	// タイトルロゴの縦幅の半分
	static constexpr int TITLE_LOGO_HEIGHT_HALF = 53;

#pragma region メンバ変数
	// タイトルロゴのハンドル
	int titleLogoHandle_;

	// タイトルシーンの状態
	TitleState state_;

	// タイトルロゴの座標
	Vector2I pos_;

	// タイトルロゴのアニメーション時間
	float animTime_;
#pragma endregion
};
