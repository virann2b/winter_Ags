#pragma once
#include "../UIBase.h"

#include "../../../Manager/Image/ImageManager.h"


class Title : public UIBase
{
public:

	Title(void);
	~Title(void) override = default;

	//初期化
	void Init(void) override;

	void Update(void) override;

	//描画
	void Draw(void) override;




private:

	//コマンド
	enum class SELECT
	{
		None = -1,

		Start,
		End,
		Option,

		Max
	};

#pragma region 定数

	static constexpr int START_POS_Y = 400;
	static constexpr int NEXT_COMAND_POS_Y = 80;


#pragma endregion

	const Img::ImageData& title_Start = Img::GetIns().GetImage(Img::IMAGE_TAG::Title_Start);
	const Img::ImageData& title_End = Img::GetIns().GetImage(Img::IMAGE_TAG::Title_End);
	const Img::ImageData& title_Option = Img::GetIns().GetImage(Img::IMAGE_TAG::Title_Option);


	int selectHandle[(int)SELECT::Max]
	{
		title_Start.handle,
		title_End.handle,
		title_Option.handle
	};

	int selectIndex = 0;	// 選択中の番号

	SELECT selct;

};

