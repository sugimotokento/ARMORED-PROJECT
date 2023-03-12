#include "main.h"
#include "renderer.h"
#include "CameraManager.h"
#include "Player.h"
#include"Scene.h"
#include"SceneManager.h"

CameraManager* CameraManager::instance;

CameraManager::CameraManager() {
	m_position[Index::CAMERA_PLAYER] = XMFLOAT3(0.0f, 20.0f, -13.0f);
	m_target[Index::CAMERA_PLAYER] = XMFLOAT3(0.0f, -0.5f, 0.0f);

	m_position[Index::CAMERA_SHADOW] = XMFLOAT3(-15, 20, -15);
	m_target[Index::CAMERA_SHADOW] = XMFLOAT3(0.0f, -0.5f, 0.0f);
}

void CameraManager::Update() {
	m_mainCamera = m_oldMainCamera;
	UpdatePlayerCamera();
	UpdateShadowCamera();
}

void CameraManager::Draw() {
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		m_viewMatrix[i] = XMMatrixLookAtLH({ m_position[i].x,m_position[i].y,m_position[i].z}, {m_target[i].x,m_target[i].y,m_target[i].z}, {0,1,0});
		m_projectionMatrix[i] = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 500000);
	}
}

void CameraManager::Create() {
	if (!instance) {
		instance = new CameraManager();

	}
}
void CameraManager::Destroy() {
	if (instance) {
		delete instance;
	}
}


void CameraManager::UpdatePlayerCamera() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (player) {
		XMFLOAT3 playerPos= player->GetPosition();
		XMFLOAT3 cameraPos= playerPos;
		cameraPos.y += 2;

		cameraPos.x += -player->GetForward().x * 5;
		cameraPos.y += -player->GetForward().y * 5;
		cameraPos.z += -player->GetForward().z * 5;
		m_position[Index::CAMERA_PLAYER] = cameraPos;

		XMFLOAT3 target = playerPos;
		target.x += player->GetForward().x * 2;
		target.y += player->GetForward().y * 2;
		target.z += player->GetForward().z * 2;
		m_target[Index::CAMERA_PLAYER] = target;
	}

}
void CameraManager::UpdateShadowCamera() {
	m_position[Index::CAMERA_SHADOW] = XMFLOAT3(-15, 20, -15);
	m_target[Index::CAMERA_SHADOW] = XMFLOAT3(0.0f, -0.5f, 0.0f);
}