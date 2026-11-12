#include "ImageManager.h"

#include "../../pch.h"

ImageManager* ImageManager::ins = nullptr;

ImageManager::ImageManager(void) :
	images()
{
}

void ImageManager::Init(void)
{
	for (int i = 0; i < (int)IMAGE_TAG::Max; ++i)
	{
		//ƒpƒX‚Ì‘‚«–Y‚ê‚Í”ò‚Î‚·
		if (PATH_TABLE[i] == nullptr) { continue; }

		ImageData& data = images[i];

		//“Ç‚İ‚İ
		data.handle = LoadGraph(PATH_TABLE[i]);

	#ifdef _DEBUG

		//“Ç‚İ‚İ¸”siƒpƒX‚ÌŠÔˆá‚¢‚È‚Çj
		if (data.handle == -1)
		{
			printfDx("‰æ‘œ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½ : %s\n", PATH_TABLE[i]);
			continue;
		}

	#endif // DEBUG


		//‘å‚«‚³‚Ìæ“¾
		GetGraphSize(data.handle, &data.sizeX, &data.sizeY);
	}
}

void ImageManager::Release(void)
{
	for (ImageData& data : images)
	{
		if (data.handle != -1) { DeleteGraph(data.handle); }
		data = ImageData();
	}
}