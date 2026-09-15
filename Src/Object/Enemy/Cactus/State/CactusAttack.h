#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

#include <memory>

class CactusAttack : public CharacterStateBase
{
public:

	CactusAttack(
		const Vector3& cactusPos,
		const Vector3*& playerPos,
		std::function<void(void)> changeStateWait,
		std::function<void(void)> playAnimationAttack,
		float& cactusAngle_Y
	);
	~CactusAttack()override = default;

	 //更新処理
	void Update(void);

private:

#pragma region 定数

	//最大クールタイム
	static constexpr short ATTACK_COOLTIME_MAX = 80;

#pragma endregion

#pragma region 変数

	//攻撃クールタイム
	short attackCoolTime;
	//クールタイムの有無
	bool isCoolTime;

#pragma endregion

#pragma region 受け取る参照

	//サボテン座標
	const Vector3& cactusPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	//サボテンの向き
	float& cactusAngle_Y;

	//待機状態に遷移する
	std::function<void(void)> changeStateWait;

	//アニメーション再生
	std::function<void(void)> playAnimationAttack;

#pragma endregion

};

