#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class Cactus : 
	public CharacterBase
{

public:

	enum class STATE
	{
		None = -1,

		//待機状態
		Wait,
		Attack,

		Max
	};

	//サボテンアニメーション
	enum class ANIM
	{
		//待機
		Wait,
		//攻撃
		Attack,
		//移動
		Move,
		//HIT
		Hit,
		//死亡
		Die,

		MAX

	};

	Cactus(
		const Vector3& pos
	) :
		CharacterBase(),
		INIT_POS(pos)
	{
		trans.pos = INIT_POS;
		SetGravityFlg(true);
	}

	void Load(void) override;

	//座標渡し
	void SetPlayerPos(const Vector3* pos) { playerPos_ = pos; }

private:

	// 初期座標
	const Vector3 INIT_POS;

	//アニメーションスピード
	const float animationSpeedTabel_[(int)ANIM::MAX] 
		= { 1.0f,0.5f,1.0f,1.0f,1.0f };

	void CharacterInit(void) override;
	void CharacterUpdate(void) override;
	void CharacterDraw(void) override;
	void CharacterAlphaDraw(void) override;
	void CharacterUiDraw(void) override;
	void CharacterRelease(void) override;

	void ResetPos(void) { trans.pos = INIT_POS; }

	//座標参照
	const Vector3 * playerPos_;
};


