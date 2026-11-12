#pragma once

class ImageManager
{
private:

	// コンストラクタ/デストラクタ
	ImageManager(void);
	~ImageManager(void) = default;

	// コピー・ムーブ操作を禁止
	ImageManager(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;
	ImageManager(ImageManager&&) = delete;
	ImageManager& operator=(ImageManager&&) = delete;

	static ImageManager* ins;

public:

#pragma region シングルトン定義
	// 生成/初期化処理
	static void CreateIns(void) { if (ins == nullptr) { ins = new ImageManager(); ins->Init(); } }
	// 取得
	static ImageManager& GetIns(void) { return *ins; }
	// 終了処理/削除
	static void DeleteIns(void) { if (ins != nullptr) { ins->Release(); delete ins; ins = nullptr; } }
#pragma endregion

	// 画像のタグ（追加したらPATH_TABLEにも同じ順番で追加する）
	enum class IMAGE_TAG
	{

		Operation_1,	//操作方法1
		Operation_2,	//操作方法2

		HpFrame,		// HPの枠
		HpFill,			// HPバーの中身
		HpDelay,		// HPが遅れて減る部分

		MpFrame,		// HPの枠
		MpFill,			// HPバーの中身

		GunAim,			// 銃の照準

		Max
	};

	// 画像の情報
	struct ImageData
	{
		int handle = -1;	// 画像ハンドル
		int sizeX = 0;		// 横幅（画像のピクセル）
		int sizeY = 0;		// 高さ（画像のピクセル）
	};

	// 画像の取得
	const ImageData& GetImage(IMAGE_TAG tag) const { return images[(int)tag]; }

private:

	// 初期化（全部読み込む）
	void Init(void);
	// 解放（全部削除する）
	void Release(void);

	// 読み込んだ画像
	ImageData images[(int)IMAGE_TAG::Max];

	// 画像のパス（IMAGE_TAGと同じ順番）
	const char* PATH_TABLE[(int)IMAGE_TAG::Max] =
	{
		"Data/Image/Player/Operation_1.png",			// Operation_1
		"Data/Image/Player/Operation_2.png",			// Operation_2

		"Data/Image/Player/HPFrame.png",			// HpFrame
		"Data/Image/Player/hp_bar_fill_yellow.png",	// HpFill
		"Data/Image/Player/hp_bar_fill_red.png",	// HpDelay

		"Data/Image/Player/MPFrame.png",			// MpFrame
		"Data/Image/Player/mp_bar_fill_green.png",	// MpFill

		"Data/Image/Gun/Aim.png",					// GunAim
	};
};

using Img = ImageManager;