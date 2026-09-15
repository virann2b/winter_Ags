#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

#include <memory>
class Player;

class CactusWait : public CharacterStateBase
{
public:

	CactusWait(
		const Vector3& cactusPos,
		const Vector3*& playerPos,
		std::function<void(void)> changeStateAttack,
		std::function<void(void)> playAnimationWait


	);
	~CactusWait()override = default;

	void Enter() override;

	// 更新処理
	void Update(void);

private:


#pragma region 受け取る参照

	//プレイヤー座標参照
	const Vector3* const& playerPos_;

	//サボテン座標
	const Vector3& cactusPos;

	//攻撃状態に遷移する
	std::function<void(void)> changeStateAttack;

	//アニメーション再生
	std::function<void(void)> playAnimationWait;

#pragma endregion
};

