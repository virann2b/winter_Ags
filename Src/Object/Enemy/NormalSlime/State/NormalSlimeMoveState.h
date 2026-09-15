#pragma once
#include "../../../Common/CharacterBase/CharacterStateBase.h"
#include "../../../../Common/Vector3.h"

class NormalSlimeMoveState 
	: public CharacterStateBase
{ 
public:

	NormalSlimeMoveState(
		const Vector3& normalSlimePos,
		std::function<void(const Vector3& vec)> normalSlimeMoveAccel
	);
	~NormalSlimeMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;
	// 更新処理
	void Update(void);

private:

	//現在の巡回番号
	unsigned char nowMovePoint = 0;

	//巡回座標リスト
	const std::vector<Vector3> MOVE_POINT_LIST =
	{
		Vector3(200,0,0),
		Vector3(-200,0,0),
	};

#pragma region 受け取る参照

	//ノーマルスライム座標
	const Vector3& normalSlimePos;
	// 加速移動関数
	std::function<void(const Vector3& vec)> normalSlimeMoveAccel;

#pragma endregion
};

