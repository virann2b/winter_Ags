#include "ChilbiStunState.h"

#include "../../../../Manager/TimeScale/TimeScale.h"

ChilbiStunState::ChilbiStunState(
	std::function<void(void)> playAnimeHit,
	std::function<float(void)> getStunFrame,
	std::function<float(void)> getAnimeRatio,
	std::function<float(void)> getAnimeStep,
	std::function<void(float)> setAnimeStep,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> changeStateIdle,
	std::function<void(void)> endStun)
	:
	playAnimeHit(playAnimeHit),
	getStunFrame(getStunFrame),
	getAnimeRatio(getAnimeRatio),
	getAnimeStep(getAnimeStep),
	setAnimeStep(setAnimeStep),
	isAnimeEnd(isAnimeEnd),
	changeStateIdle(changeStateIdle),
	endStun(endStun)
{
}

void ChilbiStunState::Enter(void)
{
	//HITアニメーション再生
	playAnimeHit();

	stunCounter = getStunFrame();
	isStop = false;
	stopStep = 0.0f;
}

void ChilbiStunState::Update(void)
{
	//硬直時間が残っている間
	if (stunCounter > 0.0f)
	{
		//止める位置まで来たら、その再生位置で止める
		if (!isStop && getAnimeRatio() >= STOP_RATIO)
		{
			isStop = true;
			stopStep = getAnimeStep();
		}

		if (isStop)
		{
			//毎フレーム同じ再生位置に戻して止める
			setAnimeStep(stopStep);
			stunCounter -= TimeScale::Get();
		}
		return;
	}

	//硬直が終わったらアニメーションの続きを再生し、終わったら待機へ
	if (isAnimeEnd())
	{
		changeStateIdle();
	}
}

void ChilbiStunState::Exit(void)
{
	//死亡などで途中で抜けた場合も含めて硬直を終える
	endStun();
}
