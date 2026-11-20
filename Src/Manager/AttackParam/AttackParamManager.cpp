#include "AttackParamManager.h"

#include <fstream>
#include <stdexcept>

#include "../../Utility/Utility.h"
#include "../../Object/Common/Collider/ColliderBase.h"


AttackParamManager* AttackParamManager::ins = nullptr;

void AttackParamManager::Init(void)
{
	//CSVファイルを開く
	std::ifstream ifs(FILE_PATH);

	//例外処理 : ファイルが開けなかった場合
	if (!ifs) { throw std::runtime_error("AttackTable.csv を開けませんでした"); }

	//１行先入力
	std::string line;

	// 1行ずつカンマ区切りにされた状態で読み込む
	while (std::getline(ifs, line))
	{
		//改行コードの残り・空行は飛ばす
		if (!line.empty() && line.back() == '\r') { line.pop_back(); }
		if (line.empty()) { continue; }

		//カンマごとに分けて文字列として配列格納
		std::vector<std::string> cols = Split(line, ',');

		//列が足りない行は飛ばす（HIT SEが空でもカンマは残っている）
		if (cols.size() < COLUMN_MAX) { continue; }

		AttackParam param;
		param.element = ToElement(cols.at(ELEMENT_NAME));
		param.damage = std::stoi(cols.at(DAMAGE));
		param.stagger = std::stoi(cols.at(STAGGER));
		param.hitSE = cols.at(HIT_SE);
		param.knockback_Pow = std::stof(cols.at(KNOCKBACK_POW));

		//IDを添え時にしてデータを格納
		params[cols.at(ID)] = param;
	}
}

const AttackParam& AttackParamManager::Get(const std::string& id) const
{
	auto it = params.find(id);
	//IDが見つからなかった場合
	if (it == params.end()) { throw std::runtime_error("AttackTable.csv に無いIDです : " + id); }
	return it->second;
}

void AttackParamManager::Apply(ColliderBase& coll, const std::string& id) const
{
	//IDを取得
	const AttackParam& param = Get(id);

	coll.SetElement(param.element);
	coll.SetDamage(param.damage);
	coll.SetStagger(param.stagger);
	coll.SetHitSE(param.hitSE);
}

ELEMENT AttackParamManager::ToElement(const std::string& name)
{
	if (name == "Physics") { return ELEMENT::Physics; }
	if (name == "Fire") { return ELEMENT::Fire; }
	if (name == "Ice") { return ELEMENT::Ice; }
	if (name == "Wind") { return ELEMENT::Wind; }
	if (name == "Thunder") { return ELEMENT::Thunder; }
	return ELEMENT::None;
}
