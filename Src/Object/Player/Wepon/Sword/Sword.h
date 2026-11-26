#pragma once
#include "../../../Common/ActorBase/ActorBase.h"


class Sword:public ActorBase
{
public:

	//剣の位置
	enum class SWORD_POSITION
	{
		None = -1,

		Hand,	//手

		Back,	//背中

		Guard,	//ガード専用

		//手から背中の間
		HandtoBack,
		//背中から手の間
		BacktoHand,
		
		MAX
	};

	//剣の状態
	enum class ACTIVE
	{
		None = -1,

		//判定と描画
		Active,
		//描画のみ
		DrawOnly,
		//両方OFF
		NotActive,

		MAX
	};

	Sword(const Transform& playerTrans, bool isActive);

	~Sword() override = default;

	//読み込み
	void Load(void) override;

	//剣の位置変更
	void ChangePosition(SWORD_POSITION nextPostion);
	//剣の判定・描画切り替え
	void ChangeActive(ACTIVE active);

	// 怯みダメージの設定
	void SetStagger(int stagger);

	//攻撃パラメータをIDで切り替える
	void SetAttackParam(const std::string& id);

private:

#pragma region 定数


	// モデル表示倍率
	const float MODEL_SCALE = 1.3f;

	//生成位置の相対座標
	const Vector3 CREATE_LOCAL_POS = Vector3(0.0f, 400.0f, 0.0f);

	//ガード時の回転補正（手のローカル軸基準・度）
	const Vector3 GUARD_ROT_OFFSET_DEG = Vector3(-20.0f, -80.0f, -30.0f);
	//ガード時の位置補正（手のローカル軸基準）
	const Vector3 GUARD_POS_OFFSET = Vector3(0.0f, 0.0f, 0.0f);

	// Wait状態の待ち時間
	const unsigned short WAIT_TIME = 60;
	//落下時の最大Y座標
	static constexpr float MAX_FALL_POS_Y = -200.0f;
	//拡大量
	static constexpr float SCALE_POW = 0.05f;

	//右手のフレーム番号
	static constexpr int RIGHT_HAND_FLAME_INDEX = 17;

#pragma endregion

	//コライダー
	ColliderBase* swordColl = nullptr;

	//剣の待機位置
	SWORD_POSITION swordPosition;

	ACTIVE active;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	//調整座標パラメータ
	Vector3 justPos;

	//右手のフレームインデックス
	int handIndex;

	bool isActive;
	bool isJudge;


	void SubUpdate(void) override;
	void SubDraw(void)override;

	//剣の位置更新
	void SwordUpdate(void);
	//剣の判定更新
	void ActiveUpdate(void);

	void AttachToHand(const Vector3& rotOffsetDeg, const Vector3& posOffset);
};
