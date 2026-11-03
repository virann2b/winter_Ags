#include "EnemyManager.h"

#include "Chilbi/Chilbi.h"

EnemyManager::EnemyManager()
{
}

void EnemyManager::Load(void)
{
	CreateEnemy(EnemyBase::ENEMY_TYPE::Chilbi, Vector3(300, 50, 200));
}

const EnemyBase* EnemyManager::GetNearestEnemy(const Vector3& from) const
{
	const EnemyBase* nearest = nullptr;
	float minDistSq = FLT_MAX;

	for (const EnemyBase* enemy : enemies)
	{
		//“G‚ªŽ€–S‚µ‚Ä‚¢‚éê‡‚ÍŽŸ‚Ö
		if (enemy->IsDead()) { continue; }

		Vector3 d = enemy->GetTrans().pos - from;
		float distSq = d.x * d.x + d.y * d.y + d.z * d.z;

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			nearest = enemy;
		}
	}

	return nearest;
};

EnemyBase* EnemyManager::CreateEnemy(EnemyBase::ENEMY_TYPE type, const Vector3& pos)
{
	EnemyBase* enemy = nullptr;

	switch (type)
	{
	case EnemyBase::ENEMY_TYPE::Chilbi:
	{
		enemy = new Chilbi(pos);
		break;
	}
	default:
		return nullptr;
	}

	//“G‚Ì¶¬
	AddChildActor(enemy);
	return enemy;
}
