#pragma once

#include <string>
#include <unordered_map>

#include "../../Object/Skill/ElementDefine.h"

class ColliderBase;

//攻撃パラメータ
struct AttackParam
{
	ELEMENT element = ELEMENT::None;	//属性

	int damage = 1;						//ダメージ量
	int stagger = 0;					//怯み値

	std::string hitSE;					//当たった時のサウンド

	float knockback_Pow = 0.0f;			//ノックバックの強さ
};

class AttackParamManager
{

public:

	//インスタンスの生成・初期化
	static void CreateInc(void) { if (ins == nullptr) { ins = new AttackParamManager(); ins->Init(); } }
	//取得
	static AttackParamManager& GetIns(void) {return *ins;}
	//削除
	static void DeleteIns(void) { if (ins != nullptr) { delete ins; ins = nullptr; } }

	//IDのパラメータを取得
	const AttackParam& Get(const std::string& id) const;

	//IDのパラメータをコライダーにまとめて適用
	void Apply(ColliderBase& coll, const std::string& id) const;

private:

	//読み込むCSV
	static constexpr const char* FILE_PATH = "Data/Attack/AttackTable.csv";

	//CSVの列番号
	enum COLUMN { ID, ELEMENT_NAME,DAMAGE,STAGGER,HIT_SE,KNOCKBACK_POW,COLUMN_MAX};


	static AttackParamManager* ins;

	std::unordered_map<std::string, AttackParam> params;

	AttackParamManager(void) = default;
	~AttackParamManager(void) = default;

	//初期化処理
	void Init(void);

	//属性名取得
	static ELEMENT ToElement(const std::string& name);

};

using AttackParamMng = AttackParamManager;

