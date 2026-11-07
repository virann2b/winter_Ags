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

	//“G‚Ì¶¬”Ô†æ“¾—p
	const EnemyBase* FindEnemy(int id) const;

	//î•ñ‘‚«Š·‚¦—p
	EnemyBase* FindEnemy(int id);

	//w’è‚Ì‘ÎÛ‚©‚çÅ‚à‹ß‚¢¶‘¶’†‚Ì“G‚Ì”Ô†‚ğæ“¾‚·‚é
	int GetNearestEnemyId(const Vector3& from) const;
	//Ÿ‚É‹ß‚¢“G‚Ì”Ô†‚ÉØ‚è‘Ö‚¦
	int GetNextNearestEnemyId(const Vector3& from, int currentId) const;

	//“GÀ•Wæ“¾
	void SetTargetPos(const Vector3* targetPos);

	//‘S‚Ä‚Ì“G‚Ì€–S”»’è
	bool IsAllDead(void);
	//‘S‚Ä‚Ì“G‚Ì€–SƒAƒjƒ[ƒVƒ‡ƒ“”»’è
	bool IsAllDieAnimEnd(void) const;

private:

	//“G‚Ìí—ŞEÀ•W‚ğw’è‚µ‚Ä“G‚ğ¶¬‚·‚é
	EnemyBase* CreateEnemy(EnemyBase::ENEMY_TYPE type, const Vector3& pos);

	std::vector<EnemyBase*> enemies;

	//Ÿ‚ÉU‚é“G‚Ì”Ô†
	int nextId = 0;

	const Vector3* targetPos = nullptr;
};

