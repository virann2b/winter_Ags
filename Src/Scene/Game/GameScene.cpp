#include "GameScene.h"

#include "../../Utility/Utility.h"

#include "../../Application/Application.h"

#include "../../Manager/TimeScale/TimeScale.h"
#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Sound/SoundManager.h"
#include "../../Manager/Font/FontManager.h"

#include "../../Manager/Camera/Game/GameCamera.h"

#include "../SceneManager.h"

#include "../ActorUseDefine.h"

#include "../Common/PostEffect/CRTPostEffect/CRTPostEffect.h"
#include "../Common/PostEffect/FocusLinesPostEffect/FocusLinesPostEffect.h"

#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"
#include "../../Object/Common/DebugObject/MeshDebugObject.h"

#include "../../Object/Player/Player.h"

#include "../../Object/Enemy/Chilbi/Chilbi.h"


#include "../../Object/Water.h"

GameScene::GameScene()
	: SceneBase(),
	elapsedSec(0.0f),
	isChanging(false)
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	AddActor(new BoxDebugObject(Vector3(20000, 1000, 20000), Vector3::Yonly(-500), false));

	AddActor(new Player);

	//敵生成
	AddActor(new Chilbi(Vector3(300,50, 200)));

}

void GameScene::SubPostInit(void)
{
	//AddPostEffect(new FocusLinesPostEffect(1.5f, 18.0f, 100.0f));

	elapsedSec = 0.0f;
	isChanging = false;
}

void GameScene::SubPostUpdate(void)
{
	 //ゲーム終了処理
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

	// 制限時間の更新
	if (!isChanging)
	{
		elapsedSec += 1.0f / 60.0f * TimeScale::Get();

		if (elapsedSec >= TIME_LIMIT_SEC)
		{
			isChanging = true;
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameOver);
			return;
		}
	}

	auto* chilbi = ActorSerch<Chilbi>(actors);
	if (chilbi && chilbi->IsDead() && !isChanging)
	{
		isChanging = true;
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
		return;
	}
}

void GameScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ゲームシーンってやつ", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));

	int remain = (int)(TIME_LIMIT_SEC - elapsedSec);
	if (remain < 0) { remain = 0; }
	DrawFormatString(0, 50, 0xffffff, "残り %d:%02d", remain / 60, remain % 60);

}

void GameScene::CreateCamera(void)
{

	camera = new GameCamera(
		&ActorSerch<Player>(actors)->GetTrans().pos,
		&ActorSerch<Chilbi>(actors)->GetTrans().pos,
		Vector3::Zonly(-400),
		Vector3(0.0f, 100.0f, 0.0f),
		3.0f * (DX_PI_F / 180.0f),
		Vector3(),
		350.0f, 400.0f,
		80.0f * (DX_PI_F / 180.0f),
		true,                // Remoteで開始
		KEY_TYPE::PlayerLockOn,      // ターゲットキー
		KEY_TYPE::PlayerGunMode        // 銃キー
		);

}