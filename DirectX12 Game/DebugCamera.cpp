#include"DebugCamera.h"


void DebugCamera::Update() {
	m_position = XMFLOAT3(0, 2, -5);
	m_target = XMFLOAT3(0.0f, 0, 5.0f);
}
void DebugCamera::Draw() {
	Camera::Draw();
}