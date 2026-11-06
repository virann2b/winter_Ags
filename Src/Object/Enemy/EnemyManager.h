#pragma once
#include <vector>

#include "../Common/ActorBase/ActorBase.h"

#include "EnemyBase.h"

class EnemyManager: public ActorBase
{
public:

	EnemyManager();
	~EnemyManager()override = default;

	//“Ç‚İ‚İ
	void Load(void) override;

	//w’è‚Ì‘ÎÛ‚©‚çÅ‚à‹ß‚¢“G‚ğæ“¾‚·‚é
	const EnemyBase* GetNearestEnemy(const Vector3& from) const;
	//¡‘ÎÛ‚É‚µ‚Ä‚¢‚é“G‚©‚ç•Ê‚Ì“G‚ÉØ‚è‘Ö‚¦
	const EnemyBase* GetNextNearestEnemy(const Vector3& from, const EnemyBase* current) const;
	//“GÀ•Wæ“¾
	void SetTargetPos(const Vector3* targetPos);

	//‘S‚Ä‚Ì“G‚Ì€–S”»’è
	bool IsAllDead(void);

private:

	//“G‚Ìí—ŞEÀ•W‚ğw’è‚µ‚Ä“G‚ğ¶¬‚·‚é
	EnemyBase* CreateEnemy(EnemyBase::ENEMY_TYPE type, const Vector3& pos);

	std::vector<EnemyBase*> enemies;

	const Vector3* targetPos = nullptr;
};

