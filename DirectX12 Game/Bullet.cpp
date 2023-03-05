#include"Bullet.h"
#include"Input.h"
#include"Cube.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Collision.h"

Bullet::Bullet() {
	m_position = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(0.3f, 0.3f, 0.3f);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_moveDir.x = 0;
	m_moveDir.y = 0;
	m_moveDir.z = 0;

	m_speed = 1;

	m_cube = new Cube();
	m_cube->SetColor(XMFLOAT4(1*1.2f, 0.65f* 1.2f, 0.2f* 1.2f, 1.0f));
}
void Bullet::Update() {
	m_position.x += m_moveDir.x * m_speed;
	m_position.z += m_moveDir.z * m_speed;

	if (m_position.z > 10 || m_position.z < -10 ||
		m_position.x > 10 || m_position.x < -10) {
		SetDestroy();
	}

}
void Bullet::Draw() {
	m_cube->Draw();
}
void Bullet::Finalize() {
	m_cube->Finalize();
	delete m_cube;
}


void Bullet::SetParametor(XMFLOAT3 position, XMFLOAT3 moveDir, float speed) {
	m_position = position;
	m_moveDir = moveDir;
	m_speed = speed;
}