#include"ShadowCamera.h"


void ShadowCamera::Update() {
	m_position = XMFLOAT3(-15, 20, -15);
	m_target = XMFLOAT3(0.0f, -0.5f, 0.0f);
}
void ShadowCamera::Draw() {
	Camera::Draw();
}