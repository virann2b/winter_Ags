#include "TitleScene.h"

#include "../../pch.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"
#include "../../Manager/Font/FontManager.h"
#include "../../Manager/Input/InputManager.h"
#include "../SceneManager.h"

#include "../Test/TestScene.h"

TitleScene::TitleScene(void) :
	SceneBase(),

	animTime_()
{
	titleLogoHandle_ = -1;
	state_ = TitleState::Title;
}

void TitleScene::SubPreInit(void)
{
	pos_ = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	animTime_ = float(TitleState::Title);

}

void TitleScene::SubPreLoad(void)
{
	titleLogoHandle_ = LoadGraph("Data/Image/Title/TitleLogo.png");
}

void TitleScene::SubPreUpdate(void)
{
	switch (state_)
	{
	case TitleState::Title:
		if (animTime_ <= 0.0f)
		{
			state_ = TitleState::Movie;
		}
		break;
	case TitleState::Movie:
	{
		//タイトルロゴの座標を右下に移動させる
		int targetX = App::SCREEN_SIZE_X - TITLE_LOGO_WIDTH_HALF;
		int targetY = App::SCREEN_SIZE_Y - TITLE_LOGO_HEIGHT_HALF;
		float dx = (float)(targetX - pos_.x);
		float dy = (float)(targetY - pos_.y);

		float length = std::sqrt(dx * dx + dy * dy);

		if (length <= 2.0f)
		{
			pos_.x = targetX;
			pos_.y = targetY;
		}
		else
		{
			pos_.x += Round(dx / length * 2.0f);
			pos_.y += Round(dy / length * 2.0f);
		}

		break;
	}
	case TitleState::MoveToDemo:
		break;
	default:
		break;
	}
}

void TitleScene::SubPostUpdate(void)
{
	// ゲーム終了
	if (Input::GetIns().GetInfo(KEY_TYPE::Pause).down)
	{
		App::GetIns().GameEnd();
		return;
	}

	// フェード付きでゲームシーンへ遷移
	if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down)
	{
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);
	}


	if (Input::GetIns().GetInfo(KEY_TYPE::DebugReload).down) {
		SceneManager::GetIns().ChangeSceneFade(std::make_unique<TestScene>());
	}

	// タイトルロゴのアニメーション
	animTime_ -= 1.0f / 60.0f;	//仮のやつ


}

void TitleScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "タイトル", 0xffffff, Font::GetIns().GetFont(FontKinds::Default45));
	DrawStringToHandle(0, 45, "「-」キーでテストシーンに遷移", 0xffffff, Font::GetIns().GetFont(FontKinds::Default20));

	//タイトルロゴ
	DrawRotaGraph(pos_.x, pos_.y, 1.0f, 0.0f, titleLogoHandle_, TRUE);
}
