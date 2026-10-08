#pragma once
#include "../../Common/CharacterBase/CharacterStateBase.h"
struct Vector3;

class PlayerNormalAttack : public CharacterStateBase
{
public:

	PlayerNormalAttack(
		std::function<void(void)> ChangeStateIdle,
		std::function<void(void)> playAnimeNormalAttack_1,
		std::function<void(void)> playAnimeNormalAttack_2,
		std::function<void(void)> playAnimeNormalAttack_3,
		std::function<bool(void)> isAnimeEnd,
		std::function<float(void)> animRate

	);

	~PlayerNormalAttack()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:


#pragma region 定数

	//最大コンボ回数
	static constexpr int COMBO_MAX = 3;
	//受付開始入力比率
	static constexpr float INPUT_RATIO_START = 0.4;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 通常攻撃_1アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeNormalAttack_1;
	// 通常攻撃_1アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeNormalAttack_2;
	// 通常攻撃_1アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeNormalAttack_3;


	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> ChangeStateIdle;

	// 通常攻撃ごとのアニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;
	//アニメーション再生比率取得
	const std::function<float(void)> animRate;

#pragma endregion

	//生成アニメーション関数テーブル
	const std::function<void(void)> PLAY_ANIM_TABLE[(int)COMBO_MAX]
	{
		playAnimeNormalAttack_1,	//normalAttack1
		playAnimeNormalAttack_2,	//normalAttack2
		playAnimeNormalAttack_3		//normalAttack3
	};

	// 現在のコンボ段階 (0始まり)
	int  comboIndex_;		// 現在のコンボ段階 (0始まり)
	bool nextInput_;		// 次段への先行入力

	//次回コンボ入力受付(再生比率で判断)
	const bool IsComboInputWindow(void);
	
};