#pragma once
#include "../Common/ActorBase/ActorBase.h"

class EnemyManager: public ActorBase
{
public:

	EnemyManager();
	~EnemyManager()override = default;

	void Load(void) override;

private:
};

