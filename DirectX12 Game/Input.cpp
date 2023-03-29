
#include<Windows.h>
#include"Main.h"
#include "input.h"


Input* Input::instance=nullptr;
XInput* XInput::instance = nullptr;

//キーボード用
Input::Input() {
	memset(m_oldKeyState, 0, 256);
	memset(m_keyState, 0, 256);
}
void Input::Update(){

	memcpy(m_oldKeyState, m_keyState, 256);

	GetKeyboardState(m_keyState);

}
bool Input::GetKeyPress(BYTE KeyCode){
	return (m_keyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode){
	return ((m_keyState[KeyCode] & 0x80) && !(m_oldKeyState[KeyCode] & 0x80));
}
void Input::Create() {
	if (!instance) {
		instance = new Input();

	}
}
void Input::Destroy() {
	if (instance) {
		delete instance;
	}
}
Input* Input::GetInstance(){
	return instance;
}



//ゲームパッド用
XInput::XInput() {
	ZeroMemory(&m_inputState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_oldInputState, sizeof(XINPUT_STATE));
}
XInput::~XInput() {

}
void XInput::Update() {
	
	ZeroMemory(&m_inputState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_oldInputState, sizeof(XINPUT_STATE));

	memcpy(&m_oldInputState, &m_inputState, sizeof(XINPUT_STATE));

	//コントローラの状態を取得
	DWORD result = XInputGetState(0, &m_inputState);
}
bool XInput::GetPadPress(WORD code) {
	return m_inputState.Gamepad.wButtons & code;
}
bool XInput::GetPadTrigger(WORD code) {
	return (m_inputState.Gamepad.wButtons & code) && !(m_oldInputState.Gamepad.wButtons & code);
}
XMFLOAT2 XInput::GetRightThumb() {
	XMFLOAT2 thumb;
	thumb.x = m_inputState.Gamepad.sThumbRX;
	thumb.y = m_inputState.Gamepad.sThumbRY;

	if (thumb.x <= -JYOSTHICK_LIMIT) {
		thumb.x = -JYOSTHICK_LIMIT;

	}
	else if (thumb.x >= JYOSTHICK_LIMIT) {
		thumb.x = JYOSTHICK_LIMIT;
	}
	thumb.x= thumb.x / JYOSTHICK_LIMIT;


	if (thumb.y <= -JYOSTHICK_LIMIT) {
		thumb.y = -JYOSTHICK_LIMIT;

	}
	else if (thumb.y >= JYOSTHICK_LIMIT) {
		thumb.y = JYOSTHICK_LIMIT;
	}
	thumb.y = thumb.y / JYOSTHICK_LIMIT;

	return thumb;
}
XMFLOAT2 XInput::GetLeftThumb() {
	XMFLOAT2 thumb;
	thumb.x = m_inputState.Gamepad.sThumbLX;
	thumb.y = m_inputState.Gamepad.sThumbLY;

	if (thumb.x <= -JYOSTHICK_LIMIT) {
		thumb.x = -JYOSTHICK_LIMIT;

	}
	else if (thumb.x >= JYOSTHICK_LIMIT) {
		thumb.x = JYOSTHICK_LIMIT;
	}
	thumb.x = thumb.x / JYOSTHICK_LIMIT;


	if (thumb.y <= -JYOSTHICK_LIMIT) {
		thumb.y = -JYOSTHICK_LIMIT;

	}
	else if (thumb.y >= JYOSTHICK_LIMIT) {
		thumb.y = JYOSTHICK_LIMIT;
	}
	thumb.y = thumb.y / JYOSTHICK_LIMIT;

	return thumb;
}

void XInput::Create() {
	if (!instance) {
		instance = new XInput();
	}
}
void XInput::Destroy() {
	if (instance) {
		delete instance;
	}
}
XInput* XInput::GetInstance() {
	return instance;
}