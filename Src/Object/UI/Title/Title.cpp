#include "Title.h"

#include "../../../Manager/Input/InputManager.h"

#include "../../../Application/Application.h"

Title::Title(void)
{
}

void Title::Init(void)
{
	//初期セレクトコマンド
	selectIndex = (int)SELECT::Start;
}

void Title::Update(void)
{
	const int max = (int)SELECT::Max;

	//上
	if (Input::GetIns().GetInfo(KEY_TYPE::Up).down) { selectIndex = (selectIndex + max - 1) % max; }
	//下
	if (Input::GetIns().GetInfo(KEY_TYPE::Down).down) { selectIndex = (selectIndex + 1) % max; }
}

void Title::Draw(void)
{

	const Img::ImageData& title_FrameOn = Img::GetIns().GetImage(Img::IMAGE_TAG::Title_Frame_On);
	const Img::ImageData& title_FrameOff = Img::GetIns().GetImage(Img::IMAGE_TAG::Title_Frame_Off);
	
	//タイトルフレーム
	for (int i = 0; i < (int)SELECT::Max; i++)
	{
		int posY = START_POS_Y + NEXT_COMAND_POS_Y * i;

		if (selectIndex == i)
		{
			DrawGraph(400, posY, title_FrameOn.handle, true);
		}

		DrawGraph(400, posY, selectHandle[i], true);
	}

	//タイトル画像
	const Img::ImageData& title = Img::GetIns().GetImage(Img::IMAGE_TAG::Title);
	DrawGraph(200, 50, title.handle, true);
}
