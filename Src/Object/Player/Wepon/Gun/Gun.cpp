#include "Gun.h"

#include "../../../../Utility/Utility.h"

#include "../../../../Manager/TimeScale/TimeScale.h"

#include "../../../Enemy/EnemyBase.h"

#include "../../Player.h"



Gun::Gun(
	const Transform& playerTrans)
	:
	ActorBase("Data/Parameter/Wepon/"),
	playerTrans(playerTrans),
	isActive(false),
	remainingBullet(0),
	recoverTimer(0.0f),
	aimTargetId(EnemyBase::INVALID_ID),
	aimCount(0),
	aimingTimer(0.0f),
	aimImage(0),
	prevPlayerPos()
{
}

void Gun::Load(void)
{
	// モデルをロード
	trans.LoadModel("Gun/Gun");

	//サイズ設定
	trans.scale = 0.2f;

	trans.localRotation = Quaternion::FromRotationY(Deg2Rad(-90.0f));

	isActive = false;
	//描画ON
	SetIsDraw(isActive);
	//判定オフ
	SetJudgeFlg(false);

#pragma region 外部読み込み

	GunParams loadParams;
	//読み込みファイル
	const std::string fileName = "Gun";

	// MAX_AMMOパラメーター名
	std::string parameterName = "MaxBulletPossession";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName)) 
	{ loadParams.MAX_AMMO = GetParameterToInt(fileName, parameterName);}

	// MAX_SHOTパラメーター名
	parameterName = "MaxShot";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName)) 
	{ loadParams.MAX_SHOT = GetParameterToInt(fileName, parameterName); }

	// RECOVER_TIMEパラメーター名
	parameterName = "RecoverTime";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName)) 
	{ loadParams.RECOVER_TIME = GetParameter(fileName, parameterName);}

	// MAX_AIMING_TIMEパラメーター名
	parameterName = "MaxAimingTime";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ loadParams.MAX_AIMING_TIME = GetParameter(fileName, parameterName);}

	// AIM_TIME_DECREASEパラメーター名
	parameterName = "AimTimeDecRease";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ loadParams.AIM_TIME_DECREASE = GetParameter(fileName, parameterName); }

	// MAX_AIMING_TIMEパラメーター名
	parameterName = "AimSpread";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ loadParams.AIM_SPREAD = GetParameter(fileName, parameterName);}

	// STUN_TIMEパラメーター名
	parameterName = "StunTime";
	// そのパラメータが存在するかどうか
	if (IsParameterExist(fileName, parameterName))
	{ loadParams.STUN_TIME = GetParameter(fileName, parameterName); }

	params = loadParams;

#pragma endregion

#pragma region 照準画像読み込み

	//画像ハンドル読み込み
	aimImage = LoadGraph("Data/Image/Gun/Aim.png");

	// 画像の幅を、照準の直径（AIM_DRAW_RADIUS × 2）に合わせる
	int width = 0, height = 0;
	GetGraphSize(aimImage, &width, &height);
	const int shortSide = (std::min)(width, height);
	if (shortSide > 0) { aimImageScale = (AIM_DRAW_RADIUS * 2.0f) / shortSide; }

#pragma endregion


}

void Gun::SubInit(void)
{
	//初期最大所持数初期化
	remainingBullet = params.MAX_AMMO;

	//弾の回復時間の初期化
	recoverTimer = 0.0f;

}

void Gun::UseBullet(int useBullet)
{
	if (remainingBullet <= 0) return;

	remainingBullet = (std::max)(remainingBullet - useBullet, 0);

	//照準リセット
	ResetAim();

}

void Gun::SubUpdate(void)
{
	
	SetIsDraw(isActive);
}

void Gun::SubRelease(void)
{
	//画像解放
	DeleteGraph(aimImage);
}

float Gun::GetAimTime(int aimCount) const
{

	//最大照準時間から所持数増えるごとに最大照準時間を減少
	const float aimTime = params.MAX_AIMING_TIME - params.AIM_TIME_DECREASE * aimCount;

	return aimTime;
}

void Gun::GetAimScreenOffset(int index, float& offsetX, float& offsetY) const
{
	// 置く順：中心 → 上 → 左 → 右 → 左下 → 右下（AIMING_POS の順）
	// x = 画面の右、y = 画面の上 への割合
	static const float OFFSET_X[(int)AIMING_POS::Max] = { 0.0f, 0.0f, -1.0f, 1.0f, -0.6f,  0.6f };
	static const float OFFSET_Y[(int)AIMING_POS::Max] = { 0.0f, 1.0f,  0.0f, 0.0f, -0.9f, -0.9f };

	// 画面上のピクセルでずらす（敵との距離で広がりが変わらない）
	// 画面座標は下がプラスなので、上方向は符号を反転する
	offsetX = OFFSET_X[index] * params.AIM_SPREAD;
	offsetY = -OFFSET_Y[index] * params.AIM_SPREAD;
}

void Gun::UpdateRecover(void)
{
	//弾が最大数持っていた場合のみ
	if (remainingBullet >= params.MAX_AMMO)
	{
		recoverTimer = 0.0f;
		return;
	}

	//回復時間をタイムスケールに合わせる
	recoverTimer += TimeScale::Get();

	if (recoverTimer >= params.RECOVER_TIME)
	{
		//弾残数追加
		remainingBullet++;
		//弾クールタイムリセット
		recoverTimer = 0.0f;

		if (remainingBullet >= params.MAX_AMMO)
		{
			remainingBullet = params.MAX_AMMO;
		}
	}
}

void Gun::UpdateAim(int targetId)
{
	//ロックオンの対象が変更した場合
	if (targetId != aimTargetId)
	{
		//リセット(再照準）
		ResetAim();
		aimTargetId = targetId;
	}

	//ロックオンしていない場合
	if (aimTargetId == EnemyBase::INVALID_ID) { return; }

	//一度に射撃できる弾数・所持弾数以上だと更新しない
	if (aimCount >= params.MAX_SHOT) { return; }
	if (aimCount >= remainingBullet) { return; }

	//照準時間の加算
	aimingTimer += TimeScale::Get();

	//リセット
	if (aimingTimer >= GetAimTime(aimCount))
	{
		aimingTimer = 0.0f;
		aimCount++;
	}

}

void Gun::UpdateHit(void)
{
	for (Hit& hit : hitBullet)
	{
		hit.timer -= TimeScale::Get();
		if (hit.timer > 0.0f) { continue; }

		// 着弾：その敵がまだ生きていれば当てる（死んでいたら何もしない＝残りの弾は消える）
		EnemyBase* enemy = findEnemy ? findEnemy(hit.targetId) : nullptr;
		if (enemy == nullptr || enemy->IsDead()) { continue; }

		//ダメージ処理
		enemy->TakeGunDamege(damage);

		//MP回復 : 初弾は２回復、２発目以降は加算
		if (addMp) { addMp(firstRecoveryMp + hitAddMp * hit.index); }

		//最後の1発で、まだ生きていれば硬直させる
		if (hit.isLast && !enemy->IsDead()) { enemy->Stun(params.STUN_TIME); }

		// 手順6でここをダメージ、手順7でMP回復に置き換える
		debugHitCount++;
	}

	// 着弾済みを消す
	hitBullet.erase(
		std::remove_if(hitBullet.begin(), hitBullet.end(),
			[](const Hit& hit) { return hit.timer <= 0.0f; }),
		hitBullet.end());
}


void Gun::FollowHand(void)
{
	const int handFrame =
		MV1SearchFrame(playerTrans.model, "smartrig:RightHand");
	if (handFrame < 0) { return; }

	if (handFrame < 0) { return; }

	MATRIX hand =
		MV1GetFrameLocalWorldMatrix(playerTrans.model, handFrame);

	// 手のワールド座標
	Vector3 handPos(
		hand.m[3][0],
		hand.m[3][1],
		hand.m[3][2]
	);
	// 回転行列からスケールを除去
	MATRIX rotationMat = hand;
	for (int i = 0; i < 3; i++)
	{
		float length = sqrtf(
			rotationMat.m[i][0] * rotationMat.m[i][0] +
			rotationMat.m[i][1] * rotationMat.m[i][1] +
			rotationMat.m[i][2] * rotationMat.m[i][2]
		);
		if (length > 0.00001f)
		{
			for (int j = 0; j < 3; j++)
			{
				rotationMat.m[i][j] /= length;
			}
		}
	}
	// 手の回転を取得
	Quaternion handRotation =
		Quaternion::FromMatrix(rotationMat);

	// 手の回転に銃の持ち方の補正を加える
	trans.rotation = handRotation * gripRotation;

	// 手のローカル座標系で位置を補正
	trans.pos = handPos + handRotation.Rotate(gripOffset);
	Vector3 playerDelta = playerTrans.pos - prevPlayerPos;
	trans.pos += playerDelta;
	prevPlayerPos = playerTrans.pos;


}

void Gun::ResetAim(void)
{
	//ロックオンしている番号リセット
	aimTargetId = EnemyBase::INVALID_ID;
	//照準時間リセット
	aimingTimer = 0.0f;
	//照準数リセット
	aimCount = 0;
}

void Gun::Shot(void)
{
	// 照準がない・対象がいない
	if (aimCount <= 0 || aimTargetId == EnemyBase::INVALID_ID) { return; }

	for (int i = 0; i < aimCount; ++i)
	{
		// 1発目は0フレーム後（すぐ）、以降 HIT_INTERVAL ずつ遅れて着弾
		// 最後の1発だけ硬直させる
		const bool isLast = (i == aimCount - 1);
		hitBullet.push_back({ aimTargetId, i, HIT_INTERVAL * i, isLast });

		// 1発ごとに所持弾を1減らす（照準は所持弾の数までしか置けないので0未満にはならない）
		remainingBullet--;
	}

	// 照準は撃ち切ったので0に戻す（同じ敵に1つ目から置き直す）
	aimCount = 0;
	aimingTimer = 0.0f;
}

void Gun::DrawBulletUI(void) const
{
	// 画面中央を円の中心にする(仮)
	int sw = 0, sh = 0;
	GetScreenState(&sw, &sh, nullptr);
	const float cx = sw * 0.5f;
	const float cy = sh * 0.5f;

	const float PI = 3.14159265f;
	const float RING_RADIUS = 150.0f;	// リングの半径
	const float BULLET_UI_R = 12.0f;	// 弾アイコンの半径

	for (int i = 0; i < params.MAX_AMMO; ++i)
	{
		// 真上から時計回りに等間隔で配置
		float rad = -PI * 0.5f + (PI * 2.0f) * i / params.MAX_AMMO;
		float x = cx + cosf(rad) * RING_RADIUS;
		float y = cy + sinf(rad) * RING_RADIUS;

		if (i < remainingBullet)
		{
			// 残っている弾:金色で塗りつぶし+縁取り
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0xd4af37, TRUE);
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0x6b5a14, FALSE, 2.0f);
		}
		else
		{
			// 空の弾:暗い輪郭のみ
			DrawCircleAA(x, y, BULLET_UI_R, 24, 0x555555, FALSE, 2.0f);
		}
	}

	// リング中央に数字も表示(確認用)
	DrawFormatString((int)cx - 20, (int)cy - 8, 0xffffff,
		"%d / %d", remainingBullet, params.MAX_AMMO);
}

void Gun::DrawAimUI(void) const
{
	if (!findEnemy) { return; }

	//ロックオンしている敵を取得
	const EnemyBase* enemy = findEnemy(aimTargetId);
	if (enemy == nullptr || enemy->IsDead()) { return; }

	// 敵の中心を画面座標に変換（照準はここを基準に画面上で並べる）
	const VECTOR center = ConvWorldPosToScreenPos(enemy->GetTrans().pos.ToVECTOR());
	// カメラの後ろ側なら描かない
	if (center.z <= 0.0f || center.z >= 1.0f) { return; }

	for (int i = 0; i < aimCount; ++i)
	{
		// 敵の中心から画面上でずらした位置
		float offsetX = 0.0f, offsetY = 0.0f;
		GetAimScreenOffset(i, offsetX, offsetY);

		const float x = center.x + offsetX, y = center.y + offsetY;

		// 画像の中心を照準の位置に合わせて描く（第5引数 TRUE = 透過あり）
		DrawRotaGraphF(x, y, aimImageScale, 0.0, aimImage, true);

		/*const float x = screen.x, y = screen.y, rad = AIM_DRAW_RADIUS;
		DrawCircleAA(x, y, rad, 32, 0xffffff, FALSE, 2.0f);
		DrawLineAA(x - rad * 1.4f, y, x + rad * 1.4f, y, 0xffffff, 2.0f);
		DrawLineAA(x, y - rad * 1.4f, x, y + rad * 1.4f, 0xffffff, 2.0f);*/
	}

	DrawFormatString(0, 260, 0xffffff, "Hit : %d", debugHitCount);
}
