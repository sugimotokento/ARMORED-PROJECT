#include"DestroyEffect.h"
#include"CubeParticle.h"
#include"SceneManager.h"
#include"Scene.h"

DestroyEffect::DestroyEffect() {
	m_position = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);
}
void DestroyEffect::Update() {
	SetDestroy();
}
void DestroyEffect::Draw() {

}
void DestroyEffect::Finalize() {

}

void DestroyEffect::AddParticle(XMFLOAT3 position, XMFLOAT4 color) {
	m_position = position;
	for (int i = 0; i < 15; i++) {
		XMFLOAT3 moveDir;
		moveDir.x = (((float)(rand() % 10) - 5) * 0.015f);
		moveDir.y = (((float)(rand() % 4)) * 0.05f);
		moveDir.z = (((float)(rand() % 10) - 5) * 0.015f);

		XMFLOAT3 rotation;
		rotation.x = ((float)(rand() % 314) * 0.01f);
		rotation.y = ((float)(rand() % 314) * 0.01f);
		rotation.z = ((float)(rand() % 314) * 0.01f);

		int destroyInterval = ((int)(rand() % 15)) + 5;

		CubeParticle* particle = SceneManager::GetInstance()->GetScene()->AddGameObject<CubeParticle>();
		particle->SetTransform(m_position, m_rotation);
		particle->SetParametor(moveDir, destroyInterval, color);
	}
}