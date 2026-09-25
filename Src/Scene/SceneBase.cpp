#include "SceneBase.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "../pch.h"

#include "../Application/Application.h"

#include "../Common/Vector2I.h"

#include "../Manager/Camera/CameraBase.h"
#include "../Manager/Collision/CollisionManager.h"
#include "../Object/Common/ActorBase/ActorBase.h"

SceneBase::SceneBase(void) :

	state(STATE::Created),

	postEffectScreen(-1),

	camera(nullptr),
	collision(nullptr),

	actors()
{
}

void SceneBase::Load(void)
{
	// 同じシーンへLoadが2回呼ばれた場合、モデルや画像の二重ロードにつながるため検出する
	if (state != STATE::Created) { throw std::logic_error("SceneBase::Load()が不正な状態で呼ばれました"); }

	// 派生先の読み込み（前）
	SubPreLoad();

	// 画面揺れを適用するため、一度このスクリーンへ3D描画をまとめる
	postEffectScreen = MakeScreen(App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, true);
	if (postEffectScreen < 0) { throw std::runtime_error("SceneBaseのメインスクリーン生成に失敗しました"); }

	// シーンごとに独立した当たり判定管理クラスを生成する
	if (UseCollisionManager()) { collision = new CollisionManager(); }

	// 派生先の読み込み（後）
	// Actorの生成はCollisionManager生成後に行う必要があるため、基本的にはここで行う
	SubPostLoad();

	// シーンごとに独立したカメラを生成する
	CreateCamera();

	// 全Actorが追加された後、巨大な静的オブジェクト等のチャンクを一度構築する
	if (collision != nullptr) { collision->InitBuildChunks(); }

	state = STATE::Loaded;
}

void SceneBase::Init(void)
{
	if (state != STATE::Loaded) { throw std::logic_error("SceneBase::Init()がLoad完了前、または二重に呼ばれました"); }

	// 派生先の初期化（前）
	SubPreInit();

	// カメラ初期化
	if (camera != nullptr) { camera->Init(); }

	// シーンが所有するActorをすべて初期化する
	for (ActorBase* actor : actors) { actor->Init(); }

	// 派生先の初期化（後）
	SubPostInit();

	state = STATE::Initialized;
}

void SceneBase::Update(void)
{
	// ロード中や解放済みのシーンは更新しない
	if (state != STATE::Initialized) { return; }

	// 入力判定やシーン固有の事前処理
	SubPreUpdate();

	// カメラ情報をDxLibへ反映
	if (camera != nullptr) { camera->Apply(); }

	// オブジェクト全ての更新処理
	for (ActorBase* actor : actors) { actor->Update(); }

	// 当たり判定更新
	if (collision != nullptr) { collision->Check(); }

	// 当たり判定による押し出し後に行う共通処理
	SubPostUpdate();

	// カメラ更新
	if (camera != nullptr) { camera->Update(); }
}

void SceneBase::Draw(void)
{
	// 安全処理
	if (state != STATE::Initialized || postEffectScreen < 0) { return; }

	// 描画先を設定
	SetDrawScreen(postEffectScreen);

	// 画面をクリア
	ClearDrawScreen();

	// カメラ情報をDxLibへ反映
	if (camera != nullptr) { camera->Apply(); }

	// エフェクシア設定
	Effekseer_Sync3DSetting();

#pragma region メイン描画

	// 通常描画～～～～～～～～～～～～～～～～～～～～

	// 派生先追加描画（前）
	SubPreDraw();

	// アクター全ての描画（通常描画）
	ActorsDraw(actors, ACTOR_DRAW_TYPE::Normal);

	// 派生先追加描画（前）
	SubPostDraw();

	// ～～～～～～～～～～～～～～～～～～～～通常描画

	// 半透明描画～～～～～～～～～～～～～～～～～～～

	// 描画モードを切り替える
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

	// アクター全ての描画（半透明描画）
	ActorsDraw(actors, ACTOR_DRAW_TYPE::Alpha);

	// 派生先追加描画
	SubAlphaDraw();

	// <デバッグ用>アクター全ての当たり判定デバッグ描画
	ActorsColliderDebugDraw(actors);

	// <デバッグ用>チャンク描画
	if (collision != nullptr && camera != nullptr) { collision->DrawChunkGrid(camera->GetPos()); }

	// 描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ～～～～～～～～～～～～～～～～～～～半透明描画

#pragma endregion

	// 描画先の画面を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	// 描画
	DrawGraph(0, 0, postEffectScreen, true);

#pragma region UI描画

	// アクター全ての描画（UI描画）
	ActorsDraw(actors, ACTOR_DRAW_TYPE::Ui);

	// 派生先追加描画（UI描画）
	SubUiDraw();

	// <デバッグ用>カメラのデバッグ描画
	if (camera != nullptr) { camera->DrawDebug(); }

#pragma endregion
}

void SceneBase::Release(void)
{
	// デストラクタとSceneManagerの両方から呼ばれても二重解放しない
	if (state == STATE::Released) { return; }

	// 派生先の解放（前）
	SubPreRelease();

	// 全てのオブジェクトを解放
	for (ActorBase*& actor : actors) {
		actor->Release();
		delete actor;
		actor = nullptr;
	}
	actors.clear();

	// 当たり判定管理解放
	if (collision != nullptr) {
		collision->Clear();
		delete collision;
		collision = nullptr;
	}

	// カメラ解放
	if(camera != nullptr) {
		camera->Release();
		delete camera;
		camera = nullptr;
	}

	// 画面演出用のスクリーン解放
	if (postEffectScreen >= 0) {
		DeleteGraph(postEffectScreen);
		postEffectScreen = -1;
	}

	// 派生先の解放（後）
	SubPostRelease();

	state = STATE::Released;
}

void SceneBase::AddActor(ActorBase* newActor)
{
	// 安全処理
	if (newActor == nullptr) { return; }

	// Actor共通読み込み
	newActor->Load();

	// Actorが持つコライダーをCollisionManagerへ登録
	if (collision != nullptr) { collision->Add(newActor->GetColliders()); }

	// 読み込み以外で呼ばれたら
	if(state == STATE::Initialized) {
		// Actor共通初期化
		newActor->Init();

		// 当たり判定管理クラスのチャンクを再構築する
		if (collision != nullptr) { collision->InitBuildChunks(); }
	}

	// Actorをシーンの所有リストへ追加する
	actors.emplace_back(newActor);
}

void SceneBase::ActorsDraw(const std::vector<ActorBase*>& actors, ACTOR_DRAW_TYPE drawType)
{
	for (ActorBase* actor : actors) {
		if (actor->GetDrawType() == drawType) { actor->Draw(); }

		ActorsDraw(actor->GetChildActors(), drawType);
	}
}

void SceneBase::ActorsColliderDebugDraw(const std::vector<ActorBase*>& actors)
{
	if (!App::GetIns().IsDrawDebug()) { return; }

	for (ActorBase* actor : actors) {
		actor->DrawColliderDebug();

		ActorsColliderDebugDraw(actor->GetChildActors());
	}
}