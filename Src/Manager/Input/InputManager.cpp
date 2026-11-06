#include"InputManager.h"

#include"../../Utility/Utility.h"
#include"../../Application/Application.h"

InputManager* InputManager::ins = nullptr;

InputManager::InputManager():
	keyInfo(),
	keyboardFormat(),
	triggerInfo(),
	mouseButtonFormat(),
	controllerButtonFormat(),
	mouseFixed(false),

	lastInputKinds(false)
{
}

void InputManager::Init(void)
{
	// キーボードを割り振るとき
#define SET_KEYBOARD(type,key)keyboardFormat[(int)type].emplace_back(key)

	// コントローラーのボタンを割り振るとき
#define SET_C_BUTTON(type,key)controllerButtonFormat[(int)type].emplace_back(key)

	// マウスのボタンを割り振るとき
#define SET_MOUSE_BUTTON(type,key)mouseButtonFormat[(int)type].emplace_back(key)

	// コントローラーのボタン以外(スティックやトリガーなど)を割り振るとき
#define SET_C_OTHERS(type,key)controllerOthersFormat[(int)type].emplace_back(key)

#pragma region プレイヤー操作

	// 移動（前）
	SET_KEYBOARD(KEY_TYPE::PlayerMoveFront, KEY_INPUT_W);
	SET_C_BUTTON(KEY_TYPE::PlayerMoveFront, XINPUT_BUTTON_DPAD_UP);
	SET_C_OTHERS(KEY_TYPE::PlayerMoveFront, CONTROLLER_OTHERS::LeftstickUp);

	// 移動（後）
	SET_KEYBOARD(KEY_TYPE::PlayerMoveBack, KEY_INPUT_S);
	SET_C_BUTTON(KEY_TYPE::PlayerMoveBack, XINPUT_BUTTON_DPAD_DOWN);
	SET_C_OTHERS(KEY_TYPE::PlayerMoveBack, CONTROLLER_OTHERS::LeftstickDown);

	// 移動（右）
	SET_KEYBOARD(KEY_TYPE::PlayerMoveRight, KEY_INPUT_D);
	SET_C_BUTTON(KEY_TYPE::PlayerMoveRight, XINPUT_BUTTON_DPAD_RIGHT);
	SET_C_OTHERS(KEY_TYPE::PlayerMoveRight, CONTROLLER_OTHERS::LeftstickRight);

	// 移動（左）
	SET_KEYBOARD(KEY_TYPE::PlayerMoveLeft, KEY_INPUT_A);
	SET_C_BUTTON(KEY_TYPE::PlayerMoveLeft, XINPUT_BUTTON_DPAD_LEFT);
	SET_C_OTHERS(KEY_TYPE::PlayerMoveLeft, CONTROLLER_OTHERS::LeftstickLeft);

	// ジャンプ
	SET_KEYBOARD(KEY_TYPE::PlayerJump, KEY_INPUT_SPACE);
	SET_C_BUTTON(KEY_TYPE::PlayerJump, XINPUT_BUTTON_A);

	//通常攻撃
	SET_MOUSE_BUTTON(KEY_TYPE::PlayerAttack, MOUSE_INPUT_LEFT);
	SET_C_BUTTON(KEY_TYPE::PlayerAttack, XINPUT_BUTTON_X);

	//強攻撃
	SET_KEYBOARD(KEY_TYPE::PlayerAttackStrong, KEY_INPUT_F);
	SET_C_BUTTON(KEY_TYPE::PlayerAttackStrong, XINPUT_BUTTON_Y);
	
	//回避
	SET_KEYBOARD(KEY_TYPE::PlayerRoll, KEY_INPUT_R);
	SET_C_BUTTON(KEY_TYPE::PlayerRoll, XINPUT_BUTTON_B);

	//スキルメニュー（押している間、下のスキル1〜4が使える）
	SET_KEYBOARD(KEY_TYPE::PlayerSkillMenu, KEY_INPUT_LSHIFT);
	SET_C_BUTTON(KEY_TYPE::PlayerSkillMenu, XINPUT_BUTTON_LEFT_SHOULDER);

	//スキル1〜4（スキルメニューと同時押し）
	SET_KEYBOARD(KEY_TYPE::PlayerSkill1, KEY_INPUT_Q);
	SET_C_BUTTON(KEY_TYPE::PlayerSkill1, XINPUT_BUTTON_X);

	SET_KEYBOARD(KEY_TYPE::PlayerSkill2, KEY_INPUT_E);
	SET_C_BUTTON(KEY_TYPE::PlayerSkill2, XINPUT_BUTTON_A);

	SET_KEYBOARD(KEY_TYPE::PlayerSkill3, KEY_INPUT_R);
	SET_C_BUTTON(KEY_TYPE::PlayerSkill3, XINPUT_BUTTON_B);

	SET_KEYBOARD(KEY_TYPE::PlayerSkill4, KEY_INPUT_F);
	SET_C_BUTTON(KEY_TYPE::PlayerSkill4, XINPUT_BUTTON_Y);

	//コントローラーの登録はPlayerGunStateの中に
	//武器から銃切り替え
	SET_MOUSE_BUTTON(KEY_TYPE::PlayerGunMode, MOUSE_INPUT_RIGHT);
	//銃の発射
	SET_MOUSE_BUTTON(KEY_TYPE::PlayerGunShot, MOUSE_INPUT_LEFT);

	//ロックオン
	SET_KEYBOARD(KEY_TYPE::PlayerLockOn, KEY_INPUT_E);
	SET_C_BUTTON(KEY_TYPE::PlayerLockOn, XINPUT_BUTTON_RIGHT_THUMB);

	//ロックオンの切り替え
	SET_KEYBOARD(KEY_TYPE::PlayerNextLockOn, KEY_INPUT_T);
	SET_C_BUTTON(KEY_TYPE::PlayerNextLockOn, XINPUT_BUTTON_RIGHT_SHOULDER);




#pragma endregion

#pragma region カメラ操作

	// 移動（前）
	SET_KEYBOARD(KEY_TYPE::CameraMoveFront, KEY_INPUT_W);
	SET_C_BUTTON(KEY_TYPE::CameraMoveFront, XINPUT_BUTTON_DPAD_UP);
	SET_C_OTHERS(KEY_TYPE::CameraMoveFront, CONTROLLER_OTHERS::LeftstickUp);

	// 移動（後）
	SET_KEYBOARD(KEY_TYPE::CameraMoveBack, KEY_INPUT_S);
	SET_C_BUTTON(KEY_TYPE::CameraMoveBack, XINPUT_BUTTON_DPAD_DOWN);
	SET_C_OTHERS(KEY_TYPE::CameraMoveBack, CONTROLLER_OTHERS::LeftstickDown);

	// 移動（右）
	SET_KEYBOARD(KEY_TYPE::CameraMoveRight, KEY_INPUT_D);
	SET_C_BUTTON(KEY_TYPE::CameraMoveRight, XINPUT_BUTTON_DPAD_RIGHT);
	SET_C_OTHERS(KEY_TYPE::CameraMoveRight, CONTROLLER_OTHERS::LeftstickRight);

	// 移動（左）
	SET_KEYBOARD(KEY_TYPE::CameraMoveLeft, KEY_INPUT_A);
	SET_C_BUTTON(KEY_TYPE::CameraMoveLeft, XINPUT_BUTTON_DPAD_LEFT);
	SET_C_OTHERS(KEY_TYPE::CameraMoveLeft, CONTROLLER_OTHERS::LeftstickLeft);

	// 移動（上）
	SET_KEYBOARD(KEY_TYPE::CameraMoveUp, KEY_INPUT_E);
	SET_C_OTHERS(KEY_TYPE::CameraMoveUp, CONTROLLER_OTHERS::RightTrigger);

	// 移動（下）
	SET_KEYBOARD(KEY_TYPE::CameraMoveDown, KEY_INPUT_Q);
	SET_C_OTHERS(KEY_TYPE::CameraMoveDown, CONTROLLER_OTHERS::LeftTrigger);

	// 回転（上）
	SET_KEYBOARD(KEY_TYPE::CameraRotUp, KEY_INPUT_UP);
	SET_C_OTHERS(KEY_TYPE::CameraRotUp, CONTROLLER_OTHERS::RightstickUp);

	// 回転（下）
	SET_KEYBOARD(KEY_TYPE::CameraRotDown, KEY_INPUT_DOWN);
	SET_C_OTHERS(KEY_TYPE::CameraRotDown, CONTROLLER_OTHERS::RightstickDown);

	// 回転（右）
	SET_KEYBOARD(KEY_TYPE::CameraRotRight, KEY_INPUT_RIGHT);
	SET_C_OTHERS(KEY_TYPE::CameraRotRight, CONTROLLER_OTHERS::RightstickRight);

	// 回転（左）
	SET_KEYBOARD(KEY_TYPE::CameraRotLeft, KEY_INPUT_LEFT);
	SET_C_OTHERS(KEY_TYPE::CameraRotLeft, CONTROLLER_OTHERS::RightstickLeft);

#pragma endregion


#pragma region システム操作

	// 汎用決定キー
	SET_KEYBOARD(KEY_TYPE::Enter, KEY_INPUT_SPACE);
	SET_KEYBOARD(KEY_TYPE::Enter, KEY_INPUT_RETURN);
	SET_C_BUTTON(KEY_TYPE::Enter, XINPUT_BUTTON_B);

	// 汎用キャンセルキー
	SET_KEYBOARD(KEY_TYPE::Cancel, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::Cancel, XINPUT_BUTTON_A);
	SET_C_BUTTON(KEY_TYPE::Cancel, XINPUT_BUTTON_START);

	// 汎用ポーズキー
	SET_KEYBOARD(KEY_TYPE::Pause, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::Pause, XINPUT_BUTTON_START);

	// 汎用終了キー
	SET_KEYBOARD(KEY_TYPE::End, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::End, XINPUT_BUTTON_START);

	// 汎用方向キー（上）
	SET_KEYBOARD(KEY_TYPE::Up, KEY_INPUT_W);
	SET_KEYBOARD(KEY_TYPE::Up, KEY_INPUT_UP);
	SET_C_BUTTON(KEY_TYPE::Up, XINPUT_BUTTON_DPAD_UP);
	SET_C_OTHERS(KEY_TYPE::Up, CONTROLLER_OTHERS::LeftstickUp);
	SET_C_OTHERS(KEY_TYPE::Up, CONTROLLER_OTHERS::RightstickUp);

	// 汎用方向キー（下）
	SET_KEYBOARD(KEY_TYPE::Down, KEY_INPUT_S);
	SET_KEYBOARD(KEY_TYPE::Down, KEY_INPUT_DOWN);
	SET_C_BUTTON(KEY_TYPE::Down, XINPUT_BUTTON_DPAD_DOWN);
	SET_C_OTHERS(KEY_TYPE::Down, CONTROLLER_OTHERS::LeftstickDown);
	SET_C_OTHERS(KEY_TYPE::Down, CONTROLLER_OTHERS::RightstickDown);

	// 汎用方向キー（左）
	SET_KEYBOARD(KEY_TYPE::Left, KEY_INPUT_A);
	SET_KEYBOARD(KEY_TYPE::Left, KEY_INPUT_LEFT);
	SET_C_BUTTON(KEY_TYPE::Left, XINPUT_BUTTON_DPAD_LEFT);
	SET_C_OTHERS(KEY_TYPE::Left, CONTROLLER_OTHERS::LeftstickLeft);
	SET_C_OTHERS(KEY_TYPE::Left, CONTROLLER_OTHERS::RightstickLeft);

	// 汎用方向キー（右）
	SET_KEYBOARD(KEY_TYPE::Right, KEY_INPUT_D);
	SET_KEYBOARD(KEY_TYPE::Right, KEY_INPUT_RIGHT);
	SET_C_BUTTON(KEY_TYPE::Right, XINPUT_BUTTON_DPAD_RIGHT);
	SET_C_OTHERS(KEY_TYPE::Right, CONTROLLER_OTHERS::LeftstickRight);
	SET_C_OTHERS(KEY_TYPE::Right, CONTROLLER_OTHERS::RightstickRight);

#pragma endregion


#pragma region デバッグキー

	// デバッグオブジェクト操作（前）
	SET_KEYBOARD(KEY_TYPE::DebugObjectFront, KEY_INPUT_W);
	// デバッグオブジェクト操作（後）
	SET_KEYBOARD(KEY_TYPE::DebugObjectBack, KEY_INPUT_S);
	// デバッグオブジェクト操作（右）
	SET_KEYBOARD(KEY_TYPE::DebugObjectRight, KEY_INPUT_D);
	// デバッグオブジェクト操作（左）
	SET_KEYBOARD(KEY_TYPE::DebugObjectLeft, KEY_INPUT_A);
	// デバッグオブジェクト操作（上）
	SET_KEYBOARD(KEY_TYPE::DebugObjectUp, KEY_INPUT_E);
	// デバッグオブジェクト操作（下）
	SET_KEYBOARD(KEY_TYPE::DebugObjectDown, KEY_INPUT_Q);

	// デバッグ描画スイッチ
	SET_KEYBOARD(KEY_TYPE::DebugDrawSwitch, KEY_INPUT_8);

	// デバッグシーンの切り替え
	SET_KEYBOARD(KEY_TYPE::DebugModeSwitch, KEY_INPUT_9);

	// デバッグモード時元のシーンの更新
	SET_KEYBOARD(KEY_TYPE::DebugModeToUpdate, KEY_INPUT_0);

	// シーンの再ロード
	SET_KEYBOARD(KEY_TYPE::DebugReload, KEY_INPUT_MINUS);

#pragma endregion
}

void InputManager::Update(void)
{
	KeyUpdate();
	TriggerUpdate();
	MouseUpdate();
}

void InputManager::Release(void)
{
	for (auto& input : keyboardFormat) { input.clear(); }			keyboardFormat->clear();
	for (auto& input : controllerButtonFormat) { input.clear(); }	controllerButtonFormat->clear();
	for (auto& input : mouseButtonFormat) { input.clear(); }		mouseButtonFormat->clear();
	for (auto& input : controllerOthersFormat) { input.clear(); }	controllerOthersFormat->clear();
}

void InputManager::KeyUpdate(void)
{
	bool windowActive = GetWindowActiveFlag();

	for (int i = 0; i < (int)KEY_TYPE::Max; i++) {
		keyInfo[i].prev = keyInfo[i].now;

		bool b = false;

		for (auto& input : keyboardFormat[i]) {
			if (b) { break; }

			if (CheckHitKey(input) != 0) { b = true; lastInputKinds = false; }
		}
		for (auto& input : controllerButtonFormat[i]) {
			if (b) { break; }

			XINPUT_STATE state = {};
			if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

			if (state.Buttons[input] != 0) { b = true; lastInputKinds = true; }
		}
		for (auto& input : mouseButtonFormat[i]) {
			if (b) { break; }
			if (GetMouseInput() & input) { b = true; lastInputKinds = false; }
		}
		for (CONTROLLER_OTHERS input : controllerOthersFormat[i]) {
			if (b) { break; }
			if (ControllerOthersInput(input)) { b = true; lastInputKinds = true; }
		}
		b = (windowActive) ? b : false;

		keyInfo[i].now = b;

		keyInfo[i].up = (keyInfo[i].prev && !keyInfo[i].now);
		keyInfo[i].down = (!keyInfo[i].prev && keyInfo[i].now);
	}

	// スキルメニューによる入力の打ち消し
	SkillMenuUpdate();
}

void InputManager::SkillMenuUpdate(void)
{
	// スキルメニューを押している間は使えなくなるキー（同じボタンにスキルが割り当てられているもの）
	static const KEY_TYPE MENU_BLOCK_KEYS[] = {
		KEY_TYPE::PlayerAttack,
		KEY_TYPE::PlayerJump,
		KEY_TYPE::PlayerRoll,
		KEY_TYPE::PlayerAttackStrong,
		KEY_TYPE::PlayerLockOn,
	};

	// スキルメニューを押している間だけ使えるキー
	static const KEY_TYPE SKILL_KEYS[] = {
		KEY_TYPE::PlayerSkill1,
		KEY_TYPE::PlayerSkill2,
		KEY_TYPE::PlayerSkill3,
		KEY_TYPE::PlayerSkill4,
	};

	const bool isSkillMenu = keyInfo[(int)KEY_TYPE::PlayerSkillMenu].now;

	for (KEY_TYPE type : MENU_BLOCK_KEYS) { SuppressKey(type, isSkillMenu); }
	for (KEY_TYPE type : SKILL_KEYS) { SuppressKey(type, !isSkillMenu); }
}

void InputManager::SuppressKey(KEY_TYPE type, bool suppress)
{
	KEY_INFO& info = keyInfo[(int)type];
	bool& suppressed = suppressedKey[(int)type];

	// キーを離したら打ち消しを解除
	if (!info.now) { suppressed = false; }
	// 押している最中に打ち消し条件になったら、離すまで打ち消す
	else if (suppress) { suppressed = true; }

	if (suppressed) { info.now = false; }

	// 打ち消し後の状態で押した瞬間・離した瞬間を計算し直す
	info.up = (info.prev && !info.now);
	info.down = (!info.prev && info.now);
}

bool InputManager::ControllerOthersInput(const CONTROLLER_OTHERS& input)
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

	short lenge = 10000;

	switch (input)
	{
	case InputManager::CONTROLLER_OTHERS::LeftstickUp:
		if (state.ThumbLY > lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::LeftstickDown:
		if (state.ThumbLY < -lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::LeftstickRight:
		if (state.ThumbLX > lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::LeftstickLeft:
		if (state.ThumbLX < -lenge) { return true; }
		break;


	case InputManager::CONTROLLER_OTHERS::RightstickUp:
		if (state.ThumbRY > lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::RightstickDown:
		if (state.ThumbRY < -lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::RightstickRight:
		if (state.ThumbRX > lenge) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::RightstickLeft:
		if (state.ThumbRX < -lenge) { return true; }
		break;


	case InputManager::CONTROLLER_OTHERS::LeftTrigger:
		if (state.LeftTrigger > 0) { return true; }
		break;
	case InputManager::CONTROLLER_OTHERS::RightTrigger:
		if (state.RightTrigger > 0) { return true; }
		break;
	default:
		return false;
		break;
	}

	return false;
}

void InputManager::MouseUpdate(void)
{
	// ウィンドウがアクティブなときのみ処理する
	if (!GetWindowActiveFlag()) {

		// アクティブじゃないときはリセット
		mouseInfo.moveVec = Vector2();

		return;
	}

	// 1フレーム前のマウス座標を保存
	mouseInfo.prev = mouseFixed ? Vector2I(Application::SCREEN_SIZE_X_HALF, Application::SCREEN_SIZE_Y_HALF) : mouseInfo.now;

	// 現在のマウス座標を取得
	GetMousePoint(&mouseInfo.now.x, &mouseInfo.now.y);

	// 1フレーム前のマウス座標から現在のマウス座標までのベクトルを取得
	Vector2 move = mouseInfo.now - mouseInfo.prev;

	// しきい値を超えていたらマウス移動ベクトルを保存
	mouseInfo.moveVec = (move.Length() > MOUSE_THRESHOLD) ? move : Vector2(0, 0);

	// マウス座標固定のフラグが立っている場合はマウス座標を中心に移動させる
	if (mouseFixed) { SetMousePoint(Application::SCREEN_SIZE_X_HALF, Application::SCREEN_SIZE_Y_HALF); }
}

void InputManager::TriggerUpdate(void)
{
	bool windowActive = GetWindowActiveFlag();

	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

	const unsigned char values[(int)TRIGGER_TYPE::Max] = { state.LeftTrigger, state.RightTrigger };

	for (int i = 0; i < (int)TRIGGER_TYPE::Max; i++)
	{
		TRIGGER_INFO& info = triggerInfo[i];

		info.prev = info.now;
		info.value = windowActive ? values[i] : 0;
		info.now = (info.value > TRIGGER_THRESHOLD);

		info.up = (info.prev && !info.now);
		info.down = (!info.prev && info.now);
	}
}

bool InputManager::GetControllerConnect(void) const
{
	return (GetJoypadNum() > 0);
}

Vector2 InputManager::GetRightStickVec(bool normalize) const
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { return { 0.0f,0.0f }; }
	short lenge = 10000;

	Vector2 vec = { (abs(state.ThumbRX) > lenge) ? (float)state.ThumbRX : 0.0f,(abs(state.ThumbRY) > lenge) ? (float)-state.ThumbRY : 0.0f };

	if (vec == 0.0f) { return{ 0.0f,0.0f }; }

	if (normalize) { vec.Normalize(); }
	else { vec /= 32768; }

	return vec;
}


Vector2 InputManager::GetLeftStickVec(bool normalize) const
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { return { 0.0f,0.0f }; }
	short lenge = 10000;

	Vector2 vec = { (abs(state.ThumbLX) > lenge) ? (float)state.ThumbLX : 0.0f,(abs(state.ThumbLY) > lenge) ? (float)-state.ThumbLY : 0.0f };

	if (vec == 0.0f) { return{ 0.0f,0.0f }; }

	if (normalize) { vec.Normalize(); }
	else { vec /= 32768; }

	return vec;
}

void InputManager::SetMouseFixed(bool fixed)
{
	mouseFixed = fixed;
	SetMouseDispFlag(!fixed);
	if (mouseFixed) { mouseInfo.prev = mouseInfo.now = { Application::SCREEN_SIZE_X_HALF,Application::SCREEN_SIZE_Y_HALF }; }
}