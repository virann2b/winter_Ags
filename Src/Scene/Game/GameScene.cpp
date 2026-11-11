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

#include "../../Object/Enemy/EnemyManager.h"
#include "../../Object/Enemy/EnemyBase.h"

#include "../../Object/UI/UIManager.h"
#include "../../Object/UI/Player/PlayerStatusUI.h"

#include "../../Object/Water.h"

GameScene::GameScene()
	: SceneBase(),
	elapsedSec(0.0f),
	isChanging(false),
	isClearSlow(false),
	gameCamera(nullptr),
	lockedEnemyId(EnemyBase::INVALID_ID)
{
}

void GameScene::SubPostLoad(void)
{
	Snd::GetIns().ChangeScene("Game");

	AddActor(new BoxDebugObject(Vector3(20000, 1000, 20000), Vector3::Yonly(-500), false));
	//敵
	AddActor(new EnemyManager);

	//プレイヤー
	AddActor(new Player);

	//UI
	AddActor(new UIManager);


}

void GameScene::SubPostInit(void)
{
	
	elapsedSec = 0.0f;

	isChanging = false;
	isClearSlow = false;
	lockedEnemyId = EnemyBase::INVALID_ID;

	// 前のシーンのスローが残っていても通常速度から始める
	TimeScale::GetIns().ChangeDefaultTimeScale();

	Player* player = ActorSerch<Player>(actors);
	EnemyManager* enemyManager = ActorSerch<EnemyManager>(actors);

	if (player && enemyManager)
	{
		enemyManager->SetTargetPos(&player->GetTrans().pos);

		// 番号から敵を探す関数をプレイヤーに渡す（プレイヤーはEnemyManagerを知らずに済む）
		player->SetEnemyFinder([enemyManager](int id) { return enemyManager->FindEnemy(id); });
	}

	UIManager* uiManager = ActorSerch<UIManager>(actors);

	if (player && uiManager)
	{
		uiManager->AddUI(new PlayerStatusUI(player->GetStatus()));
	}
}

void GameScene::SubPostUpdate(void)
{
	 //ゲーム終了処理
	if (Input::GetIns().GetInfo(KEY_TYPE::End).down) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title);
	}

	EnemyManager* enemyManager = ActorSerch<EnemyManager>(actors);

	if (enemyManager->IsAllDead())
	{
		// 最後の敵を倒した瞬間にスローにする（1回だけ）
		if (!isClearSlow)
		{
			isClearSlow = true;
			TimeScale::GetIns().ChangeTimeScale(CLEAR_SLOW_SCALE, CLEAR_SLOW_CHANGE_TIME);
		}

		// 死亡アニメーションが終わって敵が消えたら、元の速さに戻してクリアへ（1回だけ）
		if (enemyManager->IsAllDieAnimEnd() && !isChanging)
		{
			isChanging = true;
			TimeScale::GetIns().ChangeDefaultTimeScale();
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
		}
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

	

	// ロックオン中の敵（このフレームの中だけで使う）
	const EnemyBase* lockedEnemy = enemyManager->FindEnemy(lockedEnemyId);

	// 銃モード中は、ロックオン中の敵の方向にプレイヤーを向ける
	if (gameCamera && gameCamera->IsGunMode() && lockedEnemy && !lockedEnemy->IsDead())
	{
		Player* player = ActorSerch<Player>(actors);
		if (player)
		{
			Vector3 dir = lockedEnemy->GetTrans().pos - player->GetTrans().pos;
			dir.y = 0.0f;   // 水平方向のみ
			if (dir.LengthSq() > 0.000001f) { player->TargetDirection(dir); }
		}
	}

	// ロックオン中に切り替えキーが押されたら、次に近い敵へ
	if (gameCamera && gameCamera->GetMode() != GameCamera::Mode::Remote &&
		Input::GetIns().GetInfo(KEY_TYPE::PlayerNextLockOn).down)
	{
		Player* player = ActorSerch<Player>(actors);

		if (player && enemyManager)
		{
			lockedEnemyId = enemyManager->GetNextNearestEnemyId(player->GetTrans().pos, lockedEnemyId);
			lockedEnemy = enemyManager->FindEnemy(lockedEnemyId);
		}
	}

	// ロックオン中の敵の番号をプレイヤーに伝える（スキル発動時の向き・銃の照準用）
	if (Player* player = ActorSerch<Player>(actors))
	{
		const bool isLock =
			gameCamera && gameCamera->GetMode() != GameCamera::Mode::Remote &&
			lockedEnemy && !lockedEnemy->IsDead();

		player->SetLockOnTarget(isLock ? lockedEnemyId : EnemyBase::INVALID_ID);

		// 構えているかをカメラに伝える（モードの切り替えはカメラが判断する）
		if (gameCamera) { gameCamera->SetAiming(player->IsAiming()); }
	}
}

void GameScene::SubUiDraw(void)
{
	DrawStringToHandle(0, 0, "ゲームシーンってやつ", 0xffffff, Font::GetIns().GetFont(FontKinds::Gokusyou32));

	int remain = (int)(TIME_LIMIT_SEC - elapsedSec);
	if (remain < 0) { remain = 0; }
	DrawFormatString(0, 50, 0xffffff, "残り %d:%02d", remain / 60, remain % 60);

#pragma region 操作デバッグ表示

	/*DrawString(0, 120, "操作：WASD・左スティック", 0xffffff);
	DrawString(0, 140, "ジャンプ：スペース・Aボタン【最大２回】", 0xffffff);
	DrawString(0, 160, "回避：Rキー・Bボタン", 0xffffff);
	DrawString(0, 180, "ロックオン：Eキー・右スティック押し込み", 0xffffff);
	DrawString(0, 200, "通常攻撃：左クリック・Xボタン", 0xffffff);
	DrawString(0, 220, "スキル：Shift + Qキー・LB + Y", 0xffffff);
	DrawString(0, 240, "【 銃モード 】構え：右クリック・LT　長押し(ロックオンしている時のみ)", 0xffffff);
	DrawString(0, 260, "【 銃モード 】発射：左クリック・RT (弾に当たり判定はないです）", 0xffffff);
	DrawString(0, 340, "強攻撃：Fキー ・ Yボタン　長押し【タメ】離すと【攻撃】", 0xffffff);
	DrawString(0, 360, "空中攻撃：ジャンプ２回目中に【 強攻撃 】　", 0xffffff);*/

#pragma endregion

}

void GameScene::CreateCamera(void)
{
	Player* player = ActorSerch<Player>(actors);
	EnemyManager* enemyManager = ActorSerch<EnemyManager>(actors);

	auto* cam = new GameCamera(
		&player->GetTrans().pos,
		nullptr,
		Vector3::Zonly(-400),
		Vector3(0.0f, 100.0f, 0.0f),
		3.0f * (DX_PI_F / 180.0f),
		Vector3(),
		350.0f, 400.0f,
		80.0f * (DX_PI_F / 180.0f),
		true,							// フリーモードで開始
		KEY_TYPE::PlayerLockOn,			// ターゲットキー
		KEY_TYPE::PlayerGunMode			// 銃キー
		);

	gameCamera = cam;

	// 毎フレーム最寄りの敵を問い合わせる
	cam->SetFocusProvider([this, player, enemyManager](Vector3& out)
		{
			if (!enemyManager) { return false; }

			bool locking = gameCamera->GetMode() != GameCamera::Mode::Remote;
			const EnemyBase* locked = enemyManager->FindEnemy(lockedEnemyId);
			if (!locking || locked == nullptr || locked->IsDead())
			{
				lockedEnemyId = enemyManager->GetNearestEnemyId(player->GetTrans().pos);
				locked = enemyManager->FindEnemy(lockedEnemyId);
			}

			if (!locked) { return false; }
			out = locked->GetTrans().pos;
			return true;
		});

	camera = cam;

}