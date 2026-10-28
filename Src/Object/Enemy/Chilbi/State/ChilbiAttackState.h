#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include <vector>
#include <array>

class ChilbiAttackState : public CharacterStateBase
{
public:

	//攻撃パターン
	enum class PATTERN
	{
		None = -1,

		Pattern_1,

		Pattern_2,

		Max
	};

	ChilbiAttackState(
		std::function<void(void)> playAnimeAttack_1,
		std::function<void(void)> playAnimeAttack_2,
		std::function<void(void)> ChangeStateInit,
		std::function<bool(void)> isAnimeEnd);

	~ChilbiAttackState() override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);


	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	//更新
	void Update(void)override;

	// 使うパターンを指定する(Enterより前に呼ぶ)
	void SetPattern(PATTERN pattern);
private:

#pragma region 受け取る参照変数・関数

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeAttack_1;
	const std::function<void(void)> playAnimeAttack_2;

	// 待機状態に遷移
	const std::function<void(void)> ChangeStateIdle;

	//アニメーション終了判定
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion

	//パターンの最大行動数
	static constexpr int PATTERN_ACTION_MAX = 3;

	using ActionFunc = std::function<void(void)>;

	std::array<std::vector<ActionFunc>, (size_t)PATTERN::Max> patternTable_;

	// 現在のパターンと、何手目か
	PATTERN currentPattern_ = PATTERN::Pattern_1;
	size_t actionIndex_ = 0;

	// 現在の手順のアニメーションを再生
	void PlayCurrentAction(void);

};

