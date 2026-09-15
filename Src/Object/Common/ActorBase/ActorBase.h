#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Transform/Transform.h"

#include "../Collider/ColliderTagDefine.h"

#include "../../../Scene/Common/GameSpace/SpaceDefine.h"

class ColliderBase;
class GameSpaceController;
struct CollisionResult;

class ParameterLoad;

struct Vector2;
struct Vector2I;

class ActorBase
{
public:
	// デフォルトコンストラクタ
	ActorBase();
	// パラメーターを外部から読み込む場合に使うコンストラクタ
	ActorBase(const std::string& parameterPath);
	virtual ~ActorBase() = default;

	virtual void Load(void) = 0;
	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void AlphaDraw(void);
	virtual void UiDraw(void) {}
	virtual void Release(void);

	// モデルを複製する
	void ModelDuplicate(int model) { trans.Duplicate(model); }

	// モデル制御情報構造体のゲット関数
	const Transform& GetTrans(void)const { return trans; }

	/// 座標を設定する
	void SetPos(const Vector3& pos) { trans.pos = pos; }

	/// 現在座標に移動量を加算する
	void AddMove(const Vector3& move) { trans.pos += move; }

#pragma region ゲーム空間制約
	/// ゲーム空間制約を管理するコントローラーを設定する
	void SetGameSpaceController(const GameSpaceController* controller);

	/// ゲーム空間制約を管理するコントローラーを取得する
	const GameSpaceController& GetGameSpaceController(void)const { return *gameSpace; }

	/// このオブジェクトに適用する空間制約を設定する
	void SetSpaceConstraint(SPACE_CONSTRAINT constraint) { spaceConstraint = constraint; }

	/// このオブジェクトに適用されている空間制約を取得する
	const SPACE_CONSTRAINT& GetSpaceConstraint(void)const { return spaceConstraint; }

	/// 指定した方向ベクトルを現在の空間制約に合わせて制限する
	Vector3 RestrictSpaceDirection(const Vector3& direction)const;

	/// 現在の速度をゲーム空間の制約に合わせて制限する
	void RestrictVelocity(void);
#pragma endregion

	// 当たり判定の通知
	virtual void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) {}

	// 接地判定の通知
	virtual void OnGrounded() {
		if (dynamicFlg) { velocity.y = (velocity.y < 0.0f) ? 0.0f : velocity.y; }
		isGroundMaster = true;
	}

	/// <summary>
	/// コライダーすべてを取得
	/// </summary>
	virtual std::vector<ColliderBase*> GetCollider(void)const { return collider; }

	/// <summary>
	/// 当たり判定フラグの取得
	/// </summary>
	/// <returns>どれか一つでも「判定する」状態ならtrue</returns>
	bool GetJudgeFlg(void);

	// 描画判定の取得
	bool GetIsDraw(void)const { return isDraw; }

private:
	// 当たり判定情報
	std::vector<ColliderBase*> collider;

	// 動的オブジェクトか否か（true = 動的、false = 静的）
	bool dynamicFlg;

	// 重力を適用するかどうか(true = する、false = しない)
	bool gravityFlg;

	// 衝突時押し出しを行うか否か（true = 押し出す、false = 押し出さず通り抜ける）
	bool pushFlg;

	// 押し出しを行う際の重さ（0 ～ 100 で設定）
	unsigned char pushWeight;

	// 加速度の更新
	void VelocityUpdate(bool deceleration);

	// 重力更新
	void Gravity(void);

	// 重力
	static constexpr float GRAVITY = -0.6f;
	static constexpr float GRAVITY_MAX = -30.0f;

	// 接地判定 管理用(派生先で変更不可で参照渡し)
	bool isGroundMaster;

	// 描画判定 （true = 「描画する」、false = 「描画しない」）
	bool isDraw;

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool isAlphaDraw;

	// WorldSceneBaseが所有する空間制約管理への非所有参照
	const GameSpaceController* gameSpace;
	SPACE_CONSTRAINT spaceConstraint;

	// パラメーター外部ファイル管理クラス
	ParameterLoad* parameter;

protected:
	// モデル制御情報構造体
	Transform trans;

	// 移動量
	Vector3 velocity;

	// 横軸加速度の1フレームごとの加速量
	float ACCEL_RATE = 3.0f;
	// 横軸加速度の1フレームごとの減速量
	float DECEL_RATE = 3.0f;
	// 横軸加速度の最大値
	float ACCEL_MAX = 30.0f;

	// 接地判定(派生先で参照用)
	const bool& isGround = isGroundMaster;

#pragma region 初期設定
	// 当たり判定情報を生成
	void ColliderCreate(ColliderBase* newClass);

	/// <summary>
	/// 移動するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「移動する」に切り替える、false = 「移動しない」に切り替える</param>
	void SetDynamicFlg(bool flg);

	/// <summary>
	/// 重力を適用するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「する」に切り替える、false = 「しない」に切り替える</param>
	void SetGravityFlg(bool flg) { gravityFlg = flg; }

	/// <summary>
	/// 衝突時押し出しを行うかを設定する
	/// </summary>
	/// <param name="flg">true = 押し出す、false = 押し出さず通り抜ける</param>
	void SetPushFlg(bool flg);

	/// <summary>
	/// 押し出しを行う際の重さ
	/// </summary>
	/// <param name="weight">0 ～ 100 で設定（数値が大きいほど重い）</param>
	void SetPushWeight(unsigned char weight);

#pragma region パラメーター外部ファイル管理に関する関数

	bool IsParameterExist(const std::string& fileName, const std::string& parameterName)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけを取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	float GetParameter(const std::string& fileName, const std::string& parameterName, int index = 0)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターを配列ごと取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	const std::vector<float>& GetParameterArray(const std::string& fileName, const std::string& parameterName)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけをint型にキャストして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	int GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index = 0)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector3構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector3 GetParameterToVector3(const std::string& fileName, const std::string& parameterName);

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector2構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector2 GetParameterToVector2(const std::string& fileName, const std::string& parameterName);

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector2I構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector2I GetParameterToVector2I(const std::string& fileName, const std::string& parameterName);

#pragma endregion

#pragma endregion

	// 当たり判定の設定（true = 「判定する」、false = 「判定しない」）
	void SetJudge(bool flg);

	// 移動するオブジェクトかの設定を取得
	bool GetDynamicFlg(void)const { return dynamicFlg; }
	// 重力の設定の状態を取得
	bool GetGravityFlg(void)const { return gravityFlg; }

	// 描画判定の設定（true = 「描画する」、false = 「描画しない」）
	void SetIsDraw(bool flg) { isDraw = flg; }
	// 描画判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsDraw(void) { isDraw = !isDraw; }

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool GetIsAlphaDraw(void)const { return isAlphaDraw; }

	// アルファ判定の設定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	void SetIsAlphaDraw(bool flg) { isAlphaDraw = flg; }
	// アルファ判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsAlphaDraw(void) { isAlphaDraw = !isAlphaDraw; }

	// 指定の方向に向かって加速度を用いて移動する
	void MoveAccel(const Vector3& vec) {

		if (vec == 0.0f) { return; }

		// 単位ベクトルを取得
		Vector3 unitVector = vec.Normalized();

		// 加速
		velocity += unitVector * ACCEL_RATE;

		// 目標角度
		float targetAngle = atan2f(unitVector.x, unitVector.z);

		// 現在角度から目標角度までの角度差
		float diffAngle = targetAngle - trans.angle.y;

		// -π ～ +π に正規化して最短方向を求める
		while (diffAngle > DX_PI_F) { diffAngle -= DX_TWO_PI_F; }
		while (diffAngle < -DX_PI_F) { diffAngle += DX_TWO_PI_F; }

		// 最短方向に補間
		trans.angle.y += diffAngle * 0.1f;
	}

	// 派生先追加初期化
	virtual void SubInit(void) {}
	// 派生先追加更新
	virtual void SubUpdate(void) {}
	// 派生先追加描画
	virtual void SubDraw(void) {}
	// 派生先追加アルファ描画
	virtual void SubAlphaDraw(void) {}
	// 派生先追加解放
	virtual void SubRelease(void) {}
};