#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "Player.h"
#include"Scene.h"
#include"SceneManager.h"

Camera* Camera::instance;

Camera::Camera() {
	m_position[Index::CAMERA_MAIN] = XMFLOAT3(0.0f, 20.0f, -13.0f);
	m_target[Index::CAMERA_MAIN] = XMFLOAT3(0.0f, -0.5f, 0.0f);

	m_position[Index::CAMERA_SHADOW] = XMFLOAT3(-15, 20, -15);
	m_target[Index::CAMERA_SHADOW] = XMFLOAT3(0.0f, -0.5f, 0.0f);
}

void Camera::Update() {
	UpdateMainCamera();
	UpdateShadowCamera();
}

void Camera::Draw() {
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		m_viewMatrix[i] = XMMatrixLookAtLH({ m_position[i].x,m_position[i].y,m_position[i].z}, {m_target[i].x,m_target[i].y,m_target[i].z}, {0,1,0});
		m_projectionMatrix[i] = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 50000);
	}
}

void Camera::Create() {
	if (!instance) {
		instance = new Camera();

	}
}
void Camera::Destroy() {
	if (instance) {
		delete instance;
	}
}


void Camera::UpdateMainCamera() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (player) {
		XMFLOAT3 playerPos= player->GetPosition();
		XMFLOAT3 cameraPos= playerPos;
		cameraPos.y += 2;
		cameraPos.z += -5;
		m_position[Index::CAMERA_MAIN] = cameraPos;

		XMFLOAT3 target = playerPos;
		target.z += 2;
		m_target[Index::CAMERA_MAIN] = target;
	}

}
void Camera::UpdateShadowCamera() {
	m_position[Index::CAMERA_SHADOW] = XMFLOAT3(-15, 20, -15);
	m_target[Index::CAMERA_SHADOW] = XMFLOAT3(0.0f, -0.5f, 0.0f);
}