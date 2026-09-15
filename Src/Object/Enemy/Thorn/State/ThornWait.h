#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

class Icicle;

class ThornWait : public CharacterStateBase
{
public:

	ThornWait(
		const Vector3& thornPos,
		const Vector3*& playerPos,
		std::function<void(void)> changeStateAttack,
		std::function<void(void)> playAnimationWait
	);
	~ThornWait()override = default;


	void Enter() override;

	// 更新処理
	void Update(void);

private:


#pragma region 受け取る参照

	//とげ座標
	const Vector3& thornPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	//ステート切替
	std::function<void(void)> changeStateAttack;

	//アニメーション再生
	std::function<void(void)> playAnimationWait;

#pragma endregion

};

