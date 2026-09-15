#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class Thorn
	:public CharacterBase
{

public:

	enum class STATE
	{
		None = -1,

		//攻撃状態
		Attack,

		//待機状態
		Wait,
		Max
	};

	//アニメーション
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

	Thorn(
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
	void SetPlayerPos(const Vector3* pos) { playerPos = pos; }

private:

	// 初期座標
	const Vector3 INIT_POS;

	//アニメーションスピード
	const float animationSpeedTabel_[(int)ANIM::MAX]
		= { 1.0f,0.5f,1.0f,1.0f,1.0f };

	// プレイヤーが抱える下位アクター格納配列
	std::vector<ActorBase*> subObjects;

	// 初期化処理
	void CharacterInit(void)override;
	// 更新処理
	void CharacterUpdate(void)override;
	// 描画処理
	void CharacterDraw(void)override;
	// 半透明描画処理
	void CharacterAlphaDraw(void)override;
	void CharacterUiDraw(void) override;
	// 解放処理
	void CharacterRelease(void)override;



	Vector3 GetMoveDirection(void) const;

	void ResetPos(void) { trans.pos = INIT_POS; }

#pragma region 受けとる参照

	//座標参照
	const Vector3* playerPos;

#pragma endregion


};