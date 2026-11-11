#include "UIManager.h"

#include "UIBase.h"


UIManager::UIManager(void)
	:
	ActorBase(),
	uiList()
{
}

void UIManager::Load(void)
{
	//描画設定
	SetDrawType(ACTOR_DRAW_TYPE::Ui);

	//動かない・重力なし・押し出しなし
	SetDynamicFlg(false);
	SetGravityFlg(false);
	SetPushFlg(false);
}

UIBase* UIManager::AddUI(UIBase* ui)
{

	if (ui == nullptr) { return nullptr; }

	//登録後初期化
	ui->Init();

	//UIリストに追加
	uiList.emplace_back(ui);
	return ui;
}

void UIManager::SubUpdate(void)
{
	for (UIBase* ui : uiList)
	{
		ui->Update();
	}
}

void UIManager::SubDraw(void)
{
	for (UIBase* ui : uiList)
	{
		//非表示の場合は描画しない
		if (!ui->GetIsDraw()) { continue; }

		ui->Draw();
	}
}

void UIManager::SubRelease(void)
{
	for (UIBase* ui : uiList)
	{
		//空の場合は次へ
		if (ui == nullptr) { continue; }
		//解放
		ui->Release();

		//インスタンスの削除
		delete ui;
		ui = nullptr;
	}

	//UIリストを空にする
	uiList.clear();

}
