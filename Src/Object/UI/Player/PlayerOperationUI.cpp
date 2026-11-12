#include "PlayerOperationUI.h"

#include "../../../Application/Application.h"

PlayerOperationUI::PlayerOperationUI()
{
}

void PlayerOperationUI::Init(void)
{
}


void PlayerOperationUI::Draw(void)
{
	const Img::ImageData& operation_1 = Img::GetIns().GetImage(Img::IMAGE_TAG::Operation_1);

	DrawGraph(0, 0, operation_1.handle, true);
	
}
