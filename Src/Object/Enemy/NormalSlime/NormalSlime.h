#pragma once
#include "../../Common/CharacterBase/CharacterBase.h"
class NormalSlime : 
	public CharacterBase
{

public :

	enum class STATE
	{
		None = -1,

		//ˆÚ“®ó‘Ô
		Move,

		Max
	};

	NormalSlime(
		const Vector3& pos
	) :
		CharacterBase(),
		INIT_POS(pos)
	{
		trans.pos = INIT_POS;
		SetGravityFlg(true);
	}

	void Load(void) override;

private:

	// ‰ŠúÀ•W
	const Vector3 INIT_POS;

	void CharacterInit(void) override;
	void CharacterUpdate(void) override;
	void CharacterDraw(void) override;
	void CharacterAlphaDraw(void) override;
	void CharacterUiDraw(void) override;
	void CharacterRelease(void) override;

	void ResetPos(void) { trans.pos = INIT_POS; }
};

