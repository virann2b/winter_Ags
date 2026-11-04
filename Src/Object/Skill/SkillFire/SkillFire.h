#pragma once

#include"../SkillBase.h"

class SkillFire : public SkillBase
{
public:

	SkillFire(const Transform& userTrans);
	~SkillFire()override = default;

	void Load(void) override;

	//ファイアの開始処理
	void SkillStart(void) override;
	//ファイア終了処理
	void SkillEnd(void) override;

private:

};

