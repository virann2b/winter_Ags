#pragma once
#include"../Common/ActorBase/ActorBase.h"

class Stage : public ActorBase
{

public:

	Stage();
	~Stage() override = default;

	void Load(void) override;

private:
};

