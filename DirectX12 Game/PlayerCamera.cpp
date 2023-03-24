#include"PlayerCamera.h"
#include"SceneManager.h"
#include"Player.h"

void PlayerCamera::Update() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (player) {
		XMFLOAT3 playerPos = player->GetPosition();
		XMFLOAT3 cameraPos = playerPos;
		cameraPos.y += 4.5f;

		cameraPos.x += -player->GetForward().x * 10;
		cameraPos.y += -player->GetForward().y * 10;
		cameraPos.z += -player->GetForward().z * 10;
		m_position= cameraPos;

		XMFLOAT3 target = playerPos;
		target.x += player->GetForward().x * 12;
		target.y += player->GetForward().y * 12;
		target.z += player->GetForward().z * 12;
		m_target = target;
	}
}
void PlayerCamera::Draw() {
	Player* player = SceneManager::GetInstance()->GetScene()->GetGameObject<Player>(Scene::Layer::GEOMETRY);
	if (!player) return;
	Camera::Draw();
}