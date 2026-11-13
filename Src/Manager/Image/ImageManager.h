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

		Operation_1,			//操作方法1
		Operation_2,			//操作方法2

		Element_Physics,		// 物理アイコン
		Element_Fire,			// 火アイコン
		Element_Ice,			// 氷アイコン
		Element_Wind,			// 風アイコン
		Element_Thunder,		// 雷アイコン

		HpFrame,				// HPの枠
		HpFill,					// HPバーの中身
		HpDelay,				// HPが遅れて減る部分

		MpFrame,				// HPの枠
		MpFill,					// HPバーの中身

		Weak,					// 弱点！！
		WeakFrame,				// 弱点硬直時間の枠
		WeakFill,				// 弱点時間のバーの中身
		WeakElementFrame,		// 弱点属性の枠

		GunAim,					// 銃の照準
		GunBullet,				// 弾丸

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

	// 初期化
	void Init(void);
	// 解放
	void Release(void);

	// 読み込んだ画像
	ImageData images[(int)IMAGE_TAG::Max];

	// 画像のパス（IMAGE_TAGと同じ順番）
	const char* PATH_TABLE[(int)IMAGE_TAG::Max] =
	{
		"Data/Image/Player/Operation_1.png",			// Operation_1
		"Data/Image/Player/Operation_2.png",			// Operation_2

		"Data/Image/Skill/Battle.png",					// Element_Physics
		"Data/Image/Skill/Fire.png",					// Element_Fire
		"Data/Image/Skill/Ice.png",						// Element_Ice
		"Data/Image/Skill/Wind.png",					// Element_Wind
		"Data/Image/Skill/Thunder.png",					// Element_Thunder

		"Data/Image/Player/HPFrame.png",				// HpFrame
		"Data/Image/Player/hp_bar_fill_yellow.png",		// HpFill
		"Data/Image/Player/hp_bar_fill_red.png",		// HpDelay

		"Data/Image/Player/MPFrame.png",				// MpFrame
		"Data/Image/Player/mp_bar_fill_green.png",		// MpFill

		"Data/Image/Enemy/Weak.png",					// Weak
		"Data/Image/Enemy/Weak_Frame.png",				// WeakFrame
		"Data/Image/Enemy/Weak_Fill.png",				// WeakFill
		"Data/Image/Enemy/Weak_Element_Frame.png",		// WeakElementFrame

		"Data/Image/Gun/Aim.png",						// GunAim
		"Data/Image/Gun/Bullet.png",					// Bullet

	};
};

using Img = ImageManager;