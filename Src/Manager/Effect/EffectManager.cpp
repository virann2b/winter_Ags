#include "EffectManager.h"

#include <algorithm>

#include "../../pch.h"

#include "../TimeScale/TimeScale.h"

EffectManager* EffectManager::ins = nullptr;

EffectManager::EffectManager(void) :
	resources(),
	playHandles()
{
}

void EffectManager::Init(void)
{
	for (int i = 0; i < (int)EFFECT_TAG::Max; ++i)
	{
		resources[i] = -1;

		//パスの書き忘れは飛ばす
		if (INFO_TABLE[i].path == nullptr) { continue; }

		//読み込み（倍率もここで反映）
		resources[i] = LoadEffekseerEffect(INFO_TABLE[i].path, INFO_TABLE[i].scale);

	#ifdef _DEBUG

		//読み込み失敗（パスの間違いなど）
		if (resources[i] == -1)
		{
			printfDx("エフェクトの読み込みに失敗しました : %s\n", INFO_TABLE[i].path);
		}

	#endif // _DEBUG
	}
}

void EffectManager::Release(void)
{
	//再生中のものを止める
	StopAll();

	//読み込んだエフェクトを削除
	for (int& res : resources)
	{
		if (res != -1) { DeleteEffekseerEffect(res); }
		res = -1;
	}
}

int EffectManager::Play(EFFECT_TAG tag, const Vector3& pos, const Vector3& rot, float scale)
{
	if (tag >= EFFECT_TAG::Max) { return -1; }

	//読み込みに失敗しているものは再生しない
	const int res = resources[(int)tag];
	if (res == -1) { return -1; }

	//再生
	const int handle = PlayEffekseer3DEffect(res);
	if (handle == -1) { return -1; }

	//位置・回転・大きさ
	SetPosPlayingEffekseer3DEffect(handle, pos.x, pos.y, pos.z);
	SetRotationPlayingEffekseer3DEffect(handle, rot.x, rot.y, rot.z);
	SetScalePlayingEffekseer3DEffect(handle, scale, scale, scale);

	//今の時間倍率で再生
	SetSpeedPlayingEffekseer3DEffect(handle, TimeScale::Get());

	//管理リストに追加
	playHandles.emplace_back(handle);

	return handle;
}

void EffectManager::SetPos(int playHandle, const Vector3& pos)
{
	if (playHandle == -1) { return; }
	SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);
}

void EffectManager::SetRot(int playHandle, const Vector3& rot)
{
	if (playHandle == -1) { return; }
	SetRotationPlayingEffekseer3DEffect(playHandle, rot.x, rot.y, rot.z);
}

void EffectManager::SetScale(int playHandle, float scale)
{
	if (playHandle == -1) { return; }
	SetScalePlayingEffekseer3DEffect(playHandle, scale, scale, scale);
}

bool EffectManager::IsPlaying(int playHandle) const
{
	if (playHandle == -1) { return false; }

	//0なら再生中
	return IsEffekseer3DEffectPlaying(playHandle) == 0;
}

void EffectManager::Stop(int playHandle)
{
	if (playHandle == -1) { return; }

	StopEffekseer3DEffect(playHandle);

	//管理リストから外す
	playHandles.erase(std::remove(playHandles.begin(), playHandles.end(), playHandle), playHandles.end());
}

void EffectManager::StopAll(void)
{
	for (int handle : playHandles) { StopEffekseer3DEffect(handle); }
	playHandles.clear();
}

void EffectManager::Update(void)
{
	//再生が終わったものを管理リストから外す
	playHandles.erase(
		std::remove_if(playHandles.begin(), playHandles.end(),
			[](int handle) { return IsEffekseer3DEffectPlaying(handle) != 0; }),
		playHandles.end());

	//時間倍率（スロー・ヒットストップ）を反映
	const float speed = TimeScale::Get();
	for (int handle : playHandles) { SetSpeedPlayingEffekseer3DEffect(handle, speed); }

	//全エフェクトを進める
	UpdateEffekseer3D();
}

void EffectManager::Draw(void)
{
	DrawEffekseer3D();
}
