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
//XINPUT_GAMEPAD_RIGHT_THUMB  âüÇµçûÇ›
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
	bool GetPadPress(WORD code);
	bool GetPadTrigger(WORD code);
	XMFLOAT2 GetRightThumb();
	XMFLOAT2 GetLeftThumb();

	float GetRightTrigger() { return ((float)m_inputState.Gamepad.bRightTrigger)/255; }
	float GetLeftTrigger() { return ((float)m_inputState.Gamepad.bLeftTrigger)/255; }

	XINPUT_STATE GetInputState() { return m_inputState; }
	
	static void Create();
	static void Destroy();
	static XInput* GetInstance();

};