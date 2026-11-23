#include "TitleScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../ActorUseDefine.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../../Object/UI/UIManager.h"
#include "../../Object/UI/Title/Title.h"

#include "../SceneManager.h"

TitleScene::TitleScene() : SceneBase()
{
}

void TitleScene::SubPostLoad(void)
{
	// 音声のシーン切り替え
	Snd::GetIns().ChangeScene("Title");

	AddActor(new UIManager);
}

void TitleScene::SubPostInit(void)
{
	UIManager* uiManager = ActorSerch<UIManager>(actors);
	if (uiManager == nullptr) { return; }

	//タイトル画像
	uiManager->AddUI(new Title);
}

void TitleScene::SubPostUpdate(void)
{
	// ゲーム終了処理
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		App::GetIns().GameEnd();
	}

	// 決定
	if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Game);
	}
}

void TitleScene::SubUiDraw(void)
{
	/*DrawStringToHandle(0, 0, "タイトルってやつ", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));
	DrawStringToHandle(400, 300, "せいげんじかんで【　ゲームオーバー　】", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));
	DrawStringToHandle(400, 400, "てきをたおすと【　ゲームクリア　】", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));
	DrawStringToHandle(600, 700, "スペースでスタート", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));*/


}