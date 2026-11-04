#pragma once
#include "../Common/ActorBase/ActorBase.h"

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


	//スキルの種類
	enum class SKILL_TYPE
	{
		None = -1,

		Physics,	

		Fire,

		Ice,

		Wind,

		Max
	};

	//スキルの種類
	enum class SKILL_NAME
	{
		None = -1,

		Fire,

		Max
	};


	SkillBase(const Transform& userTrans);
	~SkillBase() override = default;



protected:

	//攻撃サイズ
	enum class SCALE
	{
		None = -1,

		Min,//小

		Normal,//中

		Big,//大

		Max
	};

	//スキル専用更新処理
	virtual void  SkillUpdate(void) {}

	//スキルの開始処理
	virtual void SkillStart(void) {}
	//スキルの終了処理
	virtual void SkillEnd(void) {}

	//スキル共有の初期設定
	void SetUpSkillObject(void);


	//スキルサイズ
	SCALE scale;
	//発動条件距離
	DISTANCE distance;

	//使用者のトランス
	const Transform& userTrans;


};

