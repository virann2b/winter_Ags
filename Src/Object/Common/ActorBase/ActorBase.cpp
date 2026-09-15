#include "ActorBase.h"

#include <stdexcept>

#include "../../../Application/Application.h"

#include "../../../Common/Vector2.h"
#include "../../../Common/Vector2I.h"

#include "../Collider/ColliderBase.h"

#include "../DataLoad/ParameterLoad.h"
#include "../../../Scene/Common/GameSpace/GameSpaceController.h"


ActorBase::ActorBase() :
	trans(),
	collider(),

	dynamicFlg(true),
	gravityFlg(false),

	pushFlg(true),
	pushWeight(0),

	velocity(),

	isGroundMaster(false),

	isDraw(true),
	isAlphaDraw(false),

	parameter(nullptr),

	gameSpace(nullptr),
	spaceConstraint(SPACE_CONSTRAINT::StageDefault)
{
}

ActorBase::ActorBase(const std::string& parameterPath) :
	trans(),
	collider(),

	dynamicFlg(true),
	gravityFlg(false),

	pushFlg(true),
	pushWeight(0),

	velocity(),

	isGroundMaster(false),

	isDraw(true),
	isAlphaDraw(false),

	parameter(new ParameterLoad(parameterPath)),

	gameSpace(nullptr),
	spaceConstraint(SPACE_CONSTRAINT::StageDefault)
{
}

void ActorBase::Init(void)
{
	SubInit();

	trans.Attach();
	
	// ƒpƒ‰ƒ[ƒ^‚Ì‰ğ•ú
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}
}

void ActorBase::Update(void)
{
	// 1ƒtƒŒ[ƒ€‘O‚Ì‰Á‘¬“x‚ğ•Û
	Vector3 prevVelocity = velocity;
	// “®“IƒIƒuƒWƒFƒNƒg‚Ìê‡1ƒtƒŒ[ƒ€‘O‚ÌÀ•W‚ğ•Û
	if (dynamicFlg) { trans.prevPos = trans.pos; }

	// ”h¶æ’Ç‰ÁXV
	SubUpdate();

	if (dynamicFlg) {
		// ‰Á‘¬“xXV
		VelocityUpdate(velocity == prevVelocity);

		// Ú’n”»’è‚ÌƒŠƒZƒbƒg
		isGroundMaster = false;
	}
}

void ActorBase::Draw(void)
{
	// ”h¶æ’Ç‰Á•`‰æ
	SubDraw();

	// •`‰æ”»’è
	if (!isDraw) { return; }

	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	if (!isAlphaDraw) { trans.Draw(); }
}

void ActorBase::AlphaDraw(void)
{
	// ”h¶æ’Ç‰ÁƒAƒ‹ƒtƒ@•`‰æ
	SubAlphaDraw();

	// •`‰æ”»’è
	if (!isDraw) { return; }

	// ƒ‚ƒfƒ‹‚Ì•`‰æiƒAƒ‹ƒtƒ@•`‰æj
	if (isAlphaDraw) { trans.Draw(); }

	// “–‚½‚è”»’è‚ÌƒfƒoƒbƒO•`‰æ
	if (App::GetIns().IsDrawDebug()) {
		for (ColliderBase*& c : collider) { if (c->GetJudge()) c->DrawDebug(); }
	}	
}

void ActorBase::Release(void)
{
	// ”h¶æ’Ç‰Á‰ğ•ú
	SubRelease();

	// ƒpƒ‰ƒ[ƒ^‚Ì‰ğ•ú
	if (parameter != nullptr) {
		parameter->Release();
		delete parameter;
		parameter = nullptr;
	}

	// “–‚½‚è”»’èî•ñ‚ğ‰ğ•ú
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		delete c;
		c = nullptr;
	}
	collider.clear();

	// ƒ‚ƒfƒ‹§Œäî•ñ‚Ì‰ğ•ú
	trans.Release();
}

bool ActorBase::GetJudgeFlg(void)
{
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		if (c->GetJudge()) { return true; }
	}
	return false;
}

void ActorBase::Gravity(void)
{
	if (!gravityFlg) { return; }

	velocity.y += GRAVITY;
	if (velocity.y < GRAVITY_MAX) { velocity.y = GRAVITY_MAX; }
}

void ActorBase::VelocityUpdate(bool	deceleration)
{
	// d—Íˆ—
	Gravity();

#pragma region ‰¡²iX,Zj
	// ‰¡²‚Ì‚İ‚Ì‰Á‘¬“x‚Ì‘å‚«‚³‚ğ‹‚ß‚é
	float widthAccelLen = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

	// ‰¡²‚Ì‰Á‘¬“x‚ª‚ ‚éê‡
	if (widthAccelLen > 0.0f) {

		// •â³”{—¦
		float scale = 1.0f;

		// Œ¸‘¬ˆ—
		if (deceleration) {
			// ‰¡²‚Ì‰Á‘¬“x‚ªŒ¸‘¬‚Ì”¼•ªˆÈ‰º‚Ìê‡‚ÍA‰¡²‚Ì‰Á‘¬“x‚ğ0‚É‚·‚é
			if (widthAccelLen <= DECEL_RATE * 0.5f) { velocity.x = velocity.z = 0.0f; }

			// Œ¸‘¬
			scale = (widthAccelLen - DECEL_RATE) / widthAccelLen;
		}

		// Å‘å‰Á‘¬§ŒÀ`````````````````````````````

		// Å‘å‰Á‘¬‚ğ’´‚¦‚Ä‚¢‚ê‚ÎA’´‰ß•ª‚ğ‘Å‚¿Á‚·•â³”{—¦‚ğ‘g‚İ‚Ş
		if (widthAccelLen > ACCEL_MAX) { scale = ACCEL_MAX / widthAccelLen; }

		// ÀÛ‚É•â³
		velocity.x *= scale;
		velocity.z *= scale;
		// `````````````````````````````Å‘å‰Á‘¬§ŒÀ
	}
#pragma endregion

	// ‰Á‘¬“x‚ğÀ•W‚É”½‰f
	if (velocity != 0.0f) { trans.pos += velocity; }
}

void ActorBase::ColliderCreate(ColliderBase* newClass)
{
	collider.emplace_back(newClass);
	collider.back()->SetTransformPtr(&trans);
	collider.back()->SetDynamicFlgPtr(&dynamicFlg);
	collider.back()->SetPushFlgPtr(&pushFlg);
	collider.back()->SetPushWeightPtr(&pushWeight);
	collider.back()->SetGameSpaceControllerPtr(gameSpace);
	collider.back()->SetSpaceConstraintPtr(&spaceConstraint);
	collider.back()->SetOnCollisionFunc([this](COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result) { this->OnCollision(ownTag, other, result); });
	collider.back()->SetOnGroundedFunc([this](void) { this->OnGrounded(); });
}

void ActorBase::SetDynamicFlg(bool flg)
{
	dynamicFlg = flg;
	trans.dynamicFlg = flg;
}

void ActorBase::SetPushFlg(bool flg)
{
	pushFlg = flg;
}

void ActorBase::SetPushWeight(unsigned char weight)
{
	pushWeight = weight;
}
#pragma region ƒpƒ‰ƒ[ƒ^[ŠO•”ƒtƒ@ƒCƒ‹ŠÇ—‚ÉŠÖ‚·‚éŠÖ”

bool ActorBase::IsParameterExist(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->IsParameterExist(fileName, parameterName);
}

float ActorBase::GetParameter(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameter(fileName, parameterName, index);
}

const std::vector<float>& ActorBase::GetParameterArray(const std::string& fileName, const std::string& parameterName)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterArray(fileName, parameterName);
}

int ActorBase::GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index)const
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToInt(fileName, parameterName, index);
}

Vector3 ActorBase::GetParameterToVector3(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToVector3(fileName, parameterName);
}

Vector2 ActorBase::GetParameterToVector2(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToVector2(fileName, parameterName);
}

Vector2I ActorBase::GetParameterToVector2I(const std::string& fileName, const std::string& parameterName)
{
	if (parameter == nullptr) { throw std::runtime_error("ParameterLoadƒNƒ‰ƒX‚ª¶¬‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ"); }
	return parameter->GetParameterToVector2I(fileName, parameterName);
}

#pragma endregion

void ActorBase::SetJudge(bool flg)
{
	for (ColliderBase*& c : collider) {
		if (!c) { continue; }
		c->SetJudgeFlg(flg);
	}
}
void ActorBase::SetGameSpaceController(const GameSpaceController* controller)
{
	gameSpace = controller;
	for (ColliderBase* coll : collider) {
		if (coll == nullptr) { continue; }
		coll->SetGameSpaceControllerPtr(gameSpace);
		coll->SetSpaceConstraintPtr(&spaceConstraint);
	}
}

Vector3 ActorBase::RestrictSpaceDirection(const Vector3& direction)const
{
	if (gameSpace == nullptr) { return direction; }
	return gameSpace->RestrictDirection(direction, trans.pos, spaceConstraint);
}

void ActorBase::RestrictVelocity(void)
{
	if (gameSpace == nullptr) { return; }

	velocity = gameSpace->RestrictDirection(velocity, trans.pos, spaceConstraint);
}
