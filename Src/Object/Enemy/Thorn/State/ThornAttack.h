#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

#include "../Wepon/Icicle.h"


class ThornAttack : public CharacterStateBase
{
public:

	ThornAttack(
		const Vector3& thornPos,
		const Vector3*& playerPos,
		std::function<void(void)> createIcicle,
		std::function<void(void)> changeStateWait,
		float& cactusAngle_Y,
		Icicle& icicle
	);
	~ThornAttack()override = default;


	// 更新処理
	void Update(void);

private:

#pragma region 定数

	//最大クールタイム
	static constexpr short ATTACK_COOLTIME_MAX = 160;

#pragma endregion

#pragma region 変数

	//攻撃クールタイム
	short attackCoolTime;
	//クールタイムの有無
	bool isCoolTime;

#pragma endregion


#pragma region 受け取る参照

	//とげ座標
	const Vector3& thornPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	//アングル調整
	float& cactusAngle_Y;

	Icicle& icicle_;

	//アイシクル生成+アタックアニメ再生
	std::function<void(void)> createIcicle;

	//ステート切替
	std::function<void(void)> changeStateWait;

#pragma endregion

};

