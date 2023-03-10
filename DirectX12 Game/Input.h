#pragma once
#define JYOSTHICK_LIMIT 32767
#include"Main.h"
#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib, "Xinput.lib")

class Input{
private:
	BYTE m_oldKeyState[256];
	BYTE m_keyState[256];

	static Input* instance;

	Input();

public:
	~Input() {}
	void Update();

	bool GetKeyPress(BYTE KeyCode);
	bool GetKeyTrigger(BYTE KeyCode);

	static void Create();
	static void Destroy();
	static Input* GetInstance();
};

//XINPUT_GAMEPAD_A
//XINPUT_GAMEPAD_DPAD_UP
//XINPUT_GAMEPAD_START
//XINPUT_GAMEPAD_BACK
//XINPUT_GAMEPAD_RIGHT_THUMB  ????????
//XINPUT_GAMEPAD_RIGHT_SHOULDER RB
class XInput {
private:
	static XInput* instance;
	XINPUT_STATE m_inputState;
	XINPUT_STATE m_oldInputState;
	
	XInput();
public:
	~XInput();
	void Update();
	bool GetPadPress(BYTE code);
	bool GetPadTrigger(BYTE code);
	XMFLOAT2 GetRightThumb();
	XMFLOAT2 GetLeftThumb();

	float GetRightTrigger() { return m_inputState.Gamepad.bRightTrigger; }
	float GetLeftTrigger() { return m_inputState.Gamepad.bLeftTrigger; }
	
	static void Create();
	static void Destroy();
	static XInput* GetInstance();

};