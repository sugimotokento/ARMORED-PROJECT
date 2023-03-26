#include "main.h"
#include "renderer.h"
#include "CameraManager.h"
#include "Player.h"
#include"Scene.h"
#include"SceneManager.h"

CameraManager* CameraManager::instance;

CameraManager::CameraManager() {
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		m_camera[i] = Index::CreateCamera(static_cast<Index::CameraIndex>(i));
	}
}

void CameraManager::Initialize() {

}

void CameraManager::Update() {
	m_mainCamera = m_oldMainCamera;
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		m_camera[i]->Update();
	}
}

void CameraManager::Draw() {
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		m_camera[i]->Draw();
	}
}

void CameraManager::Finalize() {
	for (int i = 0; i < Index::CAMERA_MAX; i++) {
		delete m_camera[i];
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
