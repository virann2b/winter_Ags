#include "EnemyManager.h"

#include "Chilbi/Chilbi.h"

EnemyManager::EnemyManager()
{
}

void EnemyManager::Load(void)
{
	//ƒ`ƒ‹ƒr[‚Ì¶¬
	AddChildActor(new Chilbi(Vector3(300, 50, 200)));

}
