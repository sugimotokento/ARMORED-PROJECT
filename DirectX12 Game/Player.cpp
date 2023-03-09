#include"Player.h"
#include"Input.h"
#include"Cube.h"
#include"NormalGun.h"
#include"SceneManager.h"
#include"Scene.h"
 #include"Collision.h"
#include"PlayerUI.h"
#include"DestroyEffect.h"
#include"Call.h"
#include"Camera.h"
#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG



Player::Player() {
	m_position = XMFLOAT3(0, 0.5f, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_hp = MAX_HP;
	m_isDie = false;

	AddGun<NormalGun>();

	m_cube = new Cube();
	m_cube->SetColor(XMFLOAT4(0.9f, 0.5f, 0.5f, 1.0f));

#ifdef _DEBUG
	std::function<bool()> f = std::bind(&Player::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
#endif // _DEBUG
}
void Player::Update() {
	if (m_isDie)return;
	m_position = XMFLOAT3(m_position.x, 0.5f, m_position.z);
	Move();
	Rotation();
	Shot();
	FieldCollision();

}
void Player::Draw() {
	////マトリクス設定
	XMMATRIX lightView = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_SHADOW);

	XMMATRIX view = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_MAIN);
	XMMATRIX projection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_MAIN);

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;
	XMStoreFloat4x4(&m_worldMTX, world);
}
void Player::Finalize() {
	m_cube->Finalize();
	delete m_cube;

	for (Gun* gun : m_guns) {
		delete gun;

	}
	m_guns.clear();
}



void Player::Move() {
	XMVECTOR moveDir;
	moveDir.m128_f32[0] = 0;
	moveDir.m128_f32[1] = 0;
	moveDir.m128_f32[2] = 0;
	if (Input::GetInstance()->GetKeyPress('W')) {
		moveDir.m128_f32[2] += 1.0f;
	}
	if (Input::GetInstance()->GetKeyPress('S')) {
		moveDir.m128_f32[2] -= 1.0f;
	}
	if (Input::GetInstance()->GetKeyPress('D')) {
		moveDir.m128_f32[0] += 1.0f;
	}
	if (Input::GetInstance()->GetKeyPress('A')) {
		moveDir.m128_f32[0] -= 1.0f;
	}

	//normalizeして速さを均一にする
	moveDir = XMVector3Normalize(moveDir);

	m_position.x += moveDir.m128_f32[0] * MOVE_SPEED;
	m_position.z += moveDir.m128_f32[2] * MOVE_SPEED;
}
void Player::Rotation() {
	//入力した方向に向かせる


	if (Input::GetInstance()->GetKeyPress(VK_UP)) {
	}
	if (Input::GetInstance()->GetKeyPress(VK_DOWN)) {
	}
	if (Input::GetInstance()->GetKeyPress(VK_RIGHT)) {
	//	m_rotation.y += 0.02f;
	}
	if (Input::GetInstance()->GetKeyPress(VK_LEFT)) {
		//m_rotation.y -= 0.02f;
	}


}
 
void Player::Shot() {
	/*for (Gun* gun : m_guns) {
		gun->Update();

	}*/
}
void Player::FieldCollision() {
	//Field* field = SceneManager::GetInstance()->GetScene()->GetGameObject<Field>();
	//for (int i = 0; i < 2; i++) {
	//	for (int j = 0; j < Field::Size::WIDTH / 2; j++) {
	//		if (Collision::Box(m_cube, field->GetCubeX(i, j))) {
	//			XMFLOAT3 pushVec = Collision::GetBoxPushVec(m_cube, field->GetCubeX(i, j));
	//			m_position.x += pushVec.x;
	//			m_position.z += pushVec.z;
	//		}
	//	}
	//}
	//for (int i = 0; i < 2; i++) {
	//	for (int j = 0; j < Field::Size::WIDTH / 2 - 2; j++) {
	//		if (Collision::Box(m_cube, field->GetCubeY(i, j))) {
	//			XMFLOAT3 pushVec = Collision::GetBoxPushVec(m_cube, field->GetCubeY(i, j));
	//			m_position.x += pushVec.x;
	//			m_position.z += pushVec.z;
	//		}
	//	}
	//}
}


void Player::Damage(int damage) {
	if (m_isDie)return;

	m_hp -= damage;
	if (m_hp <= 0) {
		SceneManager::GetInstance()->GetScene()->AddGameObject<DestroyEffect>()->AddParticle(m_position, XMFLOAT4(0, 0, 1, 1));
		m_isDie = true;
	}
}
int Player::GetHP() {
	return m_hp;
}
bool Player::GetIsDie() {
	return m_isDie;
}


#ifdef _DEBUG
bool Player::ImguiDebug() {
	ImGui::Begin("Player");
	ImGui::Text("x:%lf, y:%lf, z:%lf", m_position.x, m_position.y, m_position.z);
	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG

