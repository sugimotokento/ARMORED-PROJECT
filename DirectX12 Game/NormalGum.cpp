#include"NormalGun.h"
#include"GameObject.h"
#include"Bullet.h"
#include"SceneManager.h"
#include"Scene.h"

NormalGun::NormalGun() {
	m_shotIntervalCount = 0;
}

void NormalGun::Update() {
	if (m_shotIntervalCount > SHOT_INTERVAL) {
		m_shotIntervalCount = 0;

		XMFLOAT3 shotDir;
		shotDir.x = cosf(m_obj->GetRotation().y);
		shotDir.y = 0;
		shotDir.z = sinf(m_obj->GetRotation().y);
		SceneManager::GetInstance()->GetScene()->AddGameObject<Bullet>()->SetParametor(m_obj->GetPosition(), shotDir);
	}

	m_shotIntervalCount++;
}