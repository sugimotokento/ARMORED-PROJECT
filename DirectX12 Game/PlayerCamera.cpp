#include"PlayerCamera.h"
#include"SceneManager.h"
#include"CameraManager.h"
#include"Player.h"
#include"XMMath.h"
void PlayerCamera::Update() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (player) {
		XMFLOAT3 playerPos = player->GetPosition();
		XMFLOAT3 cameraPos = playerPos;
		cameraPos.y += 4.5f;

		cameraPos += -player->GetForward() * 10;
		m_position= cameraPos;

		XMFLOAT3 target = playerPos;
		target += player->GetForward() * 12;
		m_target = target;
	}
}
void PlayerCamera::Draw() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (!player) return;
	Camera::Draw();
}