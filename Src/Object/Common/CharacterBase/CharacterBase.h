#pragma once

#include "../ActorBase/ActorBase.h"

#include <map>

class CharacterStateBase;

class AnimationController;

class CharacterBase : public ActorBase
{
public:

	CharacterBase();
	CharacterBase(const std::string& parameterPath);

	virtual ~CharacterBase()override = default;

	// ステートのゲット関数
	int GetState(void)const { return state; }

private:

	// 初期化
	void SubInit(void)override;
	// 更新
	void SubUpdate(void)override;
	// 描画
	void SubDraw(void)override;
	// 半透明描画
	void SubAlphaDraw(void)override;
	// 解放
	void SubRelease(void)override;

	// アニメーションコントローラーのインスタンス
	AnimationController* anime;
	
	// ステート管理用マップ（キー：ステート番号、値：状態クラスのポインタ）
	std::map<int, CharacterStateBase*> stateMap;

	// ステートの追加
	void AddState(int stateNum, CharacterStateBase* stateIns);

	/// 状態番号を指定して自動遷移判定を登録する
	void RegisterStateTransition(int beforeState, int afterState);

	// 指定のステートへ遷移
	void  ChangeState(int nextState);

	// 指定のステートインスタンスをゲットする関数
	CharacterStateBase& GetStateIns(int state);

protected:

	// ステート管理用変数
	int state;

	// ステートの追加
	template<typename StateEnum>
	void AddState(StateEnum stateNum, CharacterStateBase* stateIns) {
		AddState(static_cast<int>(stateNum), stateIns);
	}

	/// 状態間の自動遷移判定を登録する
	template<typename StateEnum>
	void RegisterStateTransition(StateEnum beforeState, StateEnum afterState) {
		RegisterStateTransition(static_cast<int>(beforeState), static_cast<int>(afterState));
	}

	// 指定のステートへ遷移
	template<typename StateEnum>
	void  ChangeState(StateEnum nextState) { ChangeState(static_cast<int>(nextState)); }

	// 指定のステートインスタンスをゲットする関数
	template<typename StateEnum>
	CharacterStateBase& GetStateIns(StateEnum state) { return GetStateIns(static_cast<int>(state)); }

	// キャラクター固有の処理をここに追加
	virtual void CharacterInit(void) {}
	virtual void CharacterUpdate(void) {}
	virtual void CharacterDraw(void) {}
	virtual void CharacterAlphaDraw(void) {}
	virtual void CharacterUiDraw(void) {}
	virtual void CharacterRelease(void) {}

#pragma region アニメーションコントローラー
	// アニメーションコントローラーの作成
	void CreateAnimationController(void);
	
	/// <summary>
	/// モデルにくっついてるFBXアニメーションを全部登録する
	/// </summary>
	/// <param name="inFbxMaxIndex">くっついてるアニメーションの数</param>
	/// <param name="speed">再生速度</param>
	void AddInFbxAnimation(int inFbxMaxIndex, float speed);

	/// <summary>
	/// モデルにくっついてるFBXアニメーションを全部登録する
	/// </summary>
	/// <param name="inFbxMaxIndex">くっついてるアニメーションの数</param>
	/// <param name="speed">再生速度配列</param>
	void AddInFbxAnimation(int inFbxMaxIndex, const float* speed);

	/// <summary>
	/// 別ファイルに保存されているFBXアニメーションを登録する
	/// </summary>
	/// <param name="index">参照番号</param>
	/// <param name="speed">再生速度</param>
	/// <param name="filePath">パス</param>
	void AddAnimation(int index, float speed, const char* filePath);

	// アニメーション再生
	virtual void AnimePlay(int type, bool loop = true);

	// アニメーション終了チェック
	bool IsAnimeEnd(void)const;

	float GetAnimeRatio(void)const;
	float GetAnimeTotalTime(void)const;

	int GetAnimePlayType(void)const;

	float GetAnimeStep(void)const;
	void SetAnimeStep(float step);
	
#pragma endregion アニメーションコントローラー
};