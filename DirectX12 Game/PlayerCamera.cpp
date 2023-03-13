#include"PlayerCamera.h"
#include"SceneManager.h"
#include"Player.h"

void PlayerCamera::Update() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (player) {
		XMFLOAT3 playerPos = player->GetPosition();
		XMFLOAT3 cameraPos = playerPos;
		cameraPos.y += 2;

		cameraPos.x += -player->GetForward().x * 5;
		cameraPos.y += -player->GetForward().y * 5;
		cameraPos.z += -player->GetForward().z * 5;
		m_position= cameraPos;

		XMFLOAT3 target = playerPos;
		target.x += player->GetForward().x * 2;
		target.y += player->GetForward().y * 2;
		target.z += player->GetForward().z * 2;
		m_target = target;
	}
}
void PlayerCamera::Draw() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (!player) return;
	Camera::Draw();
}