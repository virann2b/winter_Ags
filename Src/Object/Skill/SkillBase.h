#pragma once
#include "../Common/ActorBase/ActorBase.h"
#include "ElementDefine.h"

class SkillBase : public ActorBase
{
public:

	//発動条件の距離(敵.・友達のみ)
	enum class DISTANCE
	{
		None = -1,

		Short,		//短い

		Middle,		//中間

		Long,		//長い

		Max

	};

	//スキル使用者
	enum class SKILL_USER
	{
		None = -1,

		Player,			//プレイヤー

		PlayerFriend,	//お友達

		Enemy,			//敵

		Max
	};


	//スキルの名前
	enum class SKILL_NAME
	{
		None = -1,

		Fire,

		Max
	};

	SkillBase(const Transform& userTrans, SKILL_USER user);
	~SkillBase() override = default;

	/// <summary>
	/// スキルを発動する
	/// </summary>
	/// <returns>発動できたらtrue（すでに発動中ならfalse）</returns>
	bool Activate(void);

	//発動中かどうか
	bool IsActive(void) const { return step != STEP::None; }

	//スキル使用者の取得
	SKILL_USER GetUser(void) const { return user; }

	//消費MPの取得（プレイヤー・お友達のみ支払う。敵は支払わない）
	int GetMpCost(void) const { return mpCost; }

	//属性取得
	ELEMENT GetElement(void) const { return element; }

protected:

	enum class STEP
	{
		None = -1,	

		Start,		//スキル開始

		Active,		//発動中

		End,		//終了

		Max
	};

	//攻撃サイズ
	enum class SCALE
	{
		None = -1,

		Min,//小

		Normal,//中

		Big,//大

		Max
	};

#pragma region 派生先で書く処理

	//スキルの開始処理(位置合わせ・エフェクトなど）発動した瞬間に1回
	virtual void SkillStart(void) {}
	//スキル発動中の更新処理（移動など）毎フレーム
	virtual void SkillUpdate(void) {}
	//スキルの終了処理　終了時に1回
	virtual void SkillEnd(void) {}

#pragma endregion

	//スキル共有の初期設定（派生先のLoadで最初に呼ぶ）
	void SetUpSkillObject(void);

	//スキル用の球コライダーを追加（使用者に合わせてタグを自動で設定）
	void AddSkillCollider(float radius);

	//発動中のスキルを途中で終了させる（命中時など）
	void Finish(void);

	//このスキルが当たる相手かどうか
	bool IsTarget(COLLIDER_TAG otherTag) const;

#pragma region 定数

	//スキル使用発動位置(デフォルト）
	static constexpr float SKILL_OFFSET_POS_Y = -20.0f;
	static constexpr float SKILL_OFFSET_POS_Z = 100.0f;

#pragma endregion

#pragma region 変数

	ELEMENT element;

	//スキル使用者
	SKILL_USER user;

	//スキルサイズ
	SCALE scale;
	//発動条件距離
	DISTANCE distance;
	//ステップ
	STEP step;

	//スキルの進む向き
	Vector3 moveDir;

	//消費MP
	int mpCost;

	//スキルの生存時間（フレーム数・派生先のLoadで設定）
	float skillAliveTime;
	//スキルの経過時間（フレーム数）
	float skillAliveCount;

#pragma endregion

	//スキル使用者のトランスを参照
	const Transform& userTrans;

private:

	//ステップ管理（派生先では触らない）
	void SubUpdate(void) override;
};
