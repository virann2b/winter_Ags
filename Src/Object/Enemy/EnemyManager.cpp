#include <algorithm>

#include "EnemyManager.h"

#include "Chilbi/Chilbi.h"

EnemyManager::EnemyManager()
{
}

void EnemyManager::Load(void)
{
	CreateEnemy(EnemyBase::ENEMY_TYPE::Chilbi, Vector3(300, 50, 200));

	CreateEnemy(EnemyBase::ENEMY_TYPE::Chilbi, Vector3(300, 50, 1000));
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
}

const EnemyBase* EnemyManager::GetNextNearestEnemy(const Vector3& from, const EnemyBase* current) const
{
	// ¶‘¶’†‚Ì“G‚ðW‚ß‚é
	std::vector<const EnemyBase*> alive;
	for (const EnemyBase* enemy : enemies)
	{
		if (!enemy->IsDead()) { alive.push_back(enemy); }
	}
	
	if (alive.empty()) { return nullptr; }

	// ‹——£‚Ì‹ß‚¢‡‚É•À‚×‚é
	auto distSq = [&from](const EnemyBase* enemy)
	{
		Vector3 d = enemy->GetTrans().pos - from;
		return d.x * d.x + d.y * d.y + d.z * d.z;
	};

	std::sort(alive.begin(), alive.end(),
		[&distSq](const EnemyBase* a, const EnemyBase* b) { return distSq(a) < distSq(b); });

	// current ‚ÌŽŸ‚ð•Ô‚·(ÅŒã‚È‚çæ“ª‚Ö–ß‚é)
	for (size_t i = 0; i < alive.size(); ++i)
	{
		if (alive[i] == current) { return alive[(i + 1) % alive.size()]; }
	}

	// current ‚ªŒ©‚Â‚©‚ç‚È‚¯‚ê‚ÎÅŠñ‚è
	return alive[0];
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
	enemies.push_back(enemy);
	return enemy;
}
