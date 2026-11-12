#pragma once
#include "../UIBase.h"

#include "../../../Manager/Image/ImageManager.h"

class PlayerOperationUI : public UIBase
{

public:

	PlayerOperationUI();
	~PlayerOperationUI(void) override = default;

	//‰Šú‰»
	void Init(void) override;

	// •`‰æ
	void Draw(void) override;

private:

};

