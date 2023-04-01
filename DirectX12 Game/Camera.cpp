#include"Camera.h"


void Camera::Draw() {
	m_viewMatrix = XMMatrixLookAtLH({ m_position.x,m_position.y,m_position.z }, { m_target.x,m_target.y,m_target.z }, { m_up.x, m_up.y, m_up.z });
	m_projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 500000);
}