#include"CubeParticle.h"
#include"Cube.h"

CubeParticle::CubeParticle() {
	m_position = XMFLOAT3(0, 0, 0);
	m_scale = XMFLOAT3(0.3f, 0.3f, 0.3f);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_moveDir.x = 0;
	m_moveDir.y = 0;
	m_moveDir.z = 0;

	m_destroyInterval = 20;
	m_destroyIntervalCount = 0;

	m_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_cube = new Cube();
	m_cube->SetColor(XMFLOAT4(1, 1, 1, 1));
}
void CubeParticle::Update() {
	m_position.x += m_moveDir.x;
	m_position.y += m_moveDir.y;
	m_position.z += m_moveDir.z;

	if (m_destroyIntervalCount > m_destroyInterval) {
		SetDestroy();
	}
	m_destroyIntervalCount++;
}
void CubeParticle::Draw() {
	m_cube->Draw(m_position, m_scale, m_rotation);
}
void CubeParticle::Finalize() {
	m_cube->Finalize();
	delete m_cube;
}


void CubeParticle::SetTransform(XMFLOAT3 position, XMFLOAT3 rotation) {
	m_position = position;
	m_rotation = rotation;
}
void CubeParticle::SetParametor(XMFLOAT3 moveDir, int detroyInterval, XMFLOAT4 color) {
	m_moveDir = moveDir;
	m_destroyInterval = detroyInterval;
	m_color = color;
	m_cube->SetColor(color);
}