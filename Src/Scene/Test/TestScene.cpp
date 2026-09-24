#include "TestScene.h"

#include "../../pch.h"

#include "../../Manager/Input/InputManager.h"
#include "../SceneManager.h"
#include "../ObjectUseDefine.h"

#include "../../Manager/Camera/GameSpaceFollow/GameSpaceFollowCamera.h"
#include "../../Manager/Camera/FollowAuto/FollowAutoCamera.h"

#include "../../Object/Common/DebugObject/SphereDebugObject.h"
#include "../../Object/Common/DebugObject/BoxDebugObject.h"
#include "../../Object/Common/DebugObject/CapsuleDebugObject.h"
#include "../../Object/Common/DebugObject/FeatureDebugMesh.h"

#include "../../Object/Enemy/NormalSlime/NormalSlime.h"
#include "../../Object/Enemy/Cactus/Cactus.h"
#include "../../Object/Enemy/Thorn/Thorn.h"
#include "../../Object/Enemy/Thorn/Wepon/Icicle.h"

#include "../../Object/Player/Player.h"
#include "../../Object/Player/Wepon/Sword.h"

TestScene::TestScene(void) :
	WorldSceneBase(),
	operatorObject(nullptr),
	testRail()
{
}

void TestScene::SubPostLoad(void)
{
	// 操作対象
	//operatorObject = new SphereDebugObject(75, Vector3(0, 180, 0), true, true, true, 100, true);
	//ObjAdd(operatorObject);

	// 操作対象（モデルあり）
	operatorObject = new Player(Vector3(0, 180, 0), true, true, true, 100);
	ObjAdd(operatorObject);

	// 通常Box床
	ObjAdd(new BoxDebugObject(Vector3(5000, 100, 5000), Vector3(0, -50, 0), false));

	// MeshCollider・BVH・坂の接地確認用
	//ObjAdd(new FeatureDebugMesh(Vector3(500.0f, 0.0f, 350.0f)));

	//仮敵生成
	testObject = new Cactus(Vector3(200, 200, 0));
	ObjAdd(testObject);

	//プレイヤー座標渡し
	ObjSerch<Cactus>(objects)->SetPlayerPos(&operatorObject->GetTrans().pos);
	

	BuildRail();
}

void TestScene::SubPostInit(void)
{
	MovementPlane plane;
	plane.origin = Vector3();
	plane.right = Vector3::Xonly(1.0f);
	plane.depth = Vector3::Zonly(1.0f);
	plane.up = Vector3::Yonly(1.0f);

	GetGameSpace().ChangeModeFree3D();
}

void TestScene::SubWorldPreUpdate(void)
{
	// 空間モード切り替え
	//if (CheckHitKey(KEY_INPUT_1) != 0) { GetGameSpace().ChangeModeSide2D(); }
	//if (CheckHitKey(KEY_INPUT_2) != 0) { GetGameSpace().ChangeModePlane3D(); }
	//if (CheckHitKey(KEY_INPUT_3) != 0) { GetGameSpace().ChangeModeRail(testRail); }
	//if (CheckHitKey(KEY_INPUT_4) != 0) { GetGameSpace().ChangeModeFree3D(); }

	// タイトルへ戻る
	if (Input::GetIns().GetInfo(KEY_TYPE::Pause).down) { SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Title); }
}

void TestScene::SubUiDraw(void)
{
	DrawFormatString(20, 20, 0xffffff, "テストシーン");

	DrawFormatString(20, 50, 0xffffff, "空間制限：%s", GetModeName());

	DrawFormatString(20, 80, 0xffffff, "操作対象：%s", GetConstraintName());

	DrawFormatString(
		20,
		150,
		0xffff88,
		"操作対象の操作ボタン -> [Z] 空間制限に従う  [X] 自分だけ2Dに縛られる  [C] 自分だけ設定したレール上に縛られる  [V] 自分だけ完全フリー3D"
	);

	DrawFormatString(
		20,
		180,
		0xffff88,
		"WASD / 左スティック : 移動   SPACE : ジャンプ"
	);

	DrawFormatString(
		20,
		210,
		0xffff88,
		"マウス / 右スティック : カメラ操作"
	);

	DrawFormatString(
		20,
		240,
		0xffff88,
		"[8] 当たり判定描画 / 設定したレールの描画   [R] 操作キャラを初期位置に戻す   [Esc] タイトルに帰る"
	);
}

void TestScene::BuildRail(void)
{
	testRail.Clear();

	testRail.AddPoint(Vector3(-700.0f, 0.0f, 0.0f));

	testRail.AddPoint(Vector3(-350.0f, 0.0f, 220.0f));

	testRail.AddPoint(Vector3(0.0f, 0.0f, 0.0f));

	testRail.AddPoint(Vector3(350.0f, 0.0f, -220.0f));

	testRail.AddPoint(Vector3(700.0f, 0.0f, 0.0f));
}

const char* TestScene::GetModeName(void)const
{
	switch (GetGameSpace().GetMode())
	{
	case GAME_SPACE_MODE::Side2D:
		return "2Dモード";

	case GAME_SPACE_MODE::Plane3D:
		return "定点カメラの3Dモード";

	case GAME_SPACE_MODE::Rail:
		return "設定したレール上に縛るモード";

	case GAME_SPACE_MODE::Free3D:
		return "完全フリーの3Dモード";

	default:
		return "未設定（バグ）";
	}
}

const char* TestScene::GetConstraintName(void)const
{
	switch (operatorObject->GetSpaceConstraint())
	{
	case SPACE_CONSTRAINT::StageDefault:
		return "空間制限に従う";

	case SPACE_CONSTRAINT::FixedPlane:
		return "自分だけ2Dに縛られる";

	case SPACE_CONSTRAINT::Rail:
		return "自分だけ設定したレール上に縛られる";

	case SPACE_CONSTRAINT::None:
		return "自分だけ完全フリー3D";

	default:
		return "未設定（バグ）";
	}
}

void TestScene::CreateCamera(void)
{
	if (operatorObject != nullptr) {

		operatorObject->SetSpaceConstraint(SPACE_CONSTRAINT::StageDefault);

		GameSpaceCameraParameter parameter;
		parameter.sideDistance = 900.0f;
		parameter.sideHeight = 260.0f;
		parameter.planeDistance = 850.0f;
		parameter.planeHeight = 380.0f;
		parameter.freeDistance = 650.0f;
		parameter.freeHeight = 180.0f;
		parameter.smooth = 0.12f;

		// カメラ生成
		//camera = new GameSpaceFollowCamera(operatorObject->GetTrans(), GetGameSpace(), parameter);

		camera = new FollowAutoCamera(
			&operatorObject->GetTrans().pos,
			&testObject->GetTrans().pos,
			350.0f,400.0f,
			Deg2Rad(80.0f));
	}
}