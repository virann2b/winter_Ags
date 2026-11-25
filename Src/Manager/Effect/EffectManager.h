#pragma once

#include <vector>

#include "../../Common/Vector3.h"

class EffectManager
{
private:

	// コンストラクタ/デストラクタ
	EffectManager(void);
	~EffectManager(void) = default;

	// コピー・ムーブ操作を禁止
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator=(EffectManager&&) = delete;

	static EffectManager* ins;

public:

#pragma region シングルトン定義
	// 生成/初期化処理
	static void CreateIns(void) { if (ins == nullptr) { ins = new EffectManager(); ins->Init(); } }
	// 取得
	static EffectManager& GetIns(void) { return *ins; }
	// 終了処理/削除
	static void DeleteIns(void) { if (ins != nullptr) { ins->Release(); delete ins; ins = nullptr; } }
#pragma endregion

	// エフェクトのタグ（追加したらINFO_TABLEにも同じ順番で追加する）
	enum class EFFECT_TAG
	{
		Test,			// 動作確認用

		AddMp,

		HitFire,

		Max
	};

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="tag">再生するエフェクト</param>
	/// <param name="pos">再生位置（ワールド座標）</param>
	/// <param name="rot">回転（ラジアン・XYZ）</param>
	/// <param name="scale">大きさの倍率（INFO_TABLEの倍率にさらに掛ける）</param>
	/// <returns>再生中のハンドル（失敗時は-1）</returns>
	int Play(EFFECT_TAG tag, const Vector3& pos, const Vector3& rot = Vector3(), float scale = 1.0f);

	// 再生中のエフェクトの位置を変更（追従させたい時に毎フレーム呼ぶ）
	void SetPos(int playHandle, const Vector3& pos);
	// 再生中のエフェクトの回転を変更（ラジアン・XYZ）
	void SetRot(int playHandle, const Vector3& rot);
	// 再生中のエフェクトの大きさを変更
	void SetScale(int playHandle, float scale);

	// 再生中か
	bool IsPlaying(int playHandle) const;

	// 指定したエフェクトを停止
	void Stop(int playHandle);
	// 全てのエフェクトを停止（シーン切り替え時など）
	void StopAll(void);

	// 更新（時間倍率を反映してから進める。1フレームに1回）
	void Update(void);
	// 描画（3D描画の後に呼ぶ）
	void Draw(void);

private:

	// 初期化（全エフェクトの読み込み）
	void Init(void);
	// 解放
	void Release(void);

	// エフェクトの情報
	struct EffectInfo
	{
		const char* path;	// ファイルパス
		float scale;		// 読み込み時の倍率（エディタとゲームの大きさの差を合わせる）
	};

	// エフェクトの情報（EFFECT_TAGと同じ順番）
	const EffectInfo INFO_TABLE[(int)EFFECT_TAG::Max] =
	{
		{ "Data/Effect/Test.efkefc", 15.0f },		// Test
		{ "Data/Effect/AddMp.efkefc", 30.0f },		// AddMp
		{ "Data/Effect/HitFire.efkefc", 30.0f },		// AddMp

	};

	// 読み込んだエフェクトのハンドル
	int resources[(int)EFFECT_TAG::Max];

	// 再生中のエフェクトのハンドル
	std::vector<int> playHandles;
};

using Eff = EffectManager;
