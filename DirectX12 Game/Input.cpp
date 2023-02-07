
#include<Windows.h>
#include "input.h"


Input* Input::instance;


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