#include "GameScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../../Manager/Camera/GameSpaceFollow/GameSpaceFollowCamera.h"

#include "../SceneManager.h"

#include "../ObjectUseDefine.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/MeshDebugObject.h"

GameScene::GameScene() : WorldSceneBase()
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	ObjAdd(new BoxDebugObject(Vector3(2000, 1000, 2000), Vector3::Yonly(-500), false));

	ObjAdd(new SphereDebugObject(50.0f, Vector3(), true, true, true, 50, true));
}

void GameScene::SubWorldPostUpdate(void)
{
	// ÉQÅ[ÉÄèIóπèàóù
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

	// åàíË
	if (Input::GetIns().GetInfo(KEY_TYPE::Enter).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
	}
}

void GameScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ÉQÅ[ÉÄ", 0xffffff, Font::GetIns().GetFont(FontKinds::Default45));
}

void GameScene::CreateCamera(void)
{
	SphereDebugObject* target = ObjSerch<SphereDebugObject>(objects);
	if (target == nullptr) { camera = nullptr; return; }
	camera = new GameSpaceFollowCamera(target->GetTrans(), GetGameSpace());
}