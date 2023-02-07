#pragma once


class Input
{
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
