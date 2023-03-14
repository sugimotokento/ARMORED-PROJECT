#include"Player.h"
#include"Input.h"
#include"Cube.h"
#include"SceneManager.h"
#include"Scene.h"
 #include"Collision.h"
#include"PlayerUI.h"
#include"Call.h"
#include"CameraManager.h"
#include"XMMath.h"

#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG



Player::Player() {

}
void Player::Initialize() {
	m_position = XMFLOAT3(0, 0.5f, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_hp = MAX_HP;
	m_isDie = false;

	/*for (int i = 0; i < 5; i++) {
		m_texture[i] = std::make_unique<TextureGeometry>();
		m_model[i] = std::make_unique<Model>();
	}*/
	/*m_model[0].get()->LoadMesh("asset/model/Dreadnought/Arm.fbx");
	m_model[1].get()->LoadMesh("asset/model/Dreadnought/Head.fbx");
	m_model[2].get()->LoadMesh("asset/model/Dreadnought/Lower.fbx");
	m_model[3].get()->LoadMesh("asset/model/Dreadnought/Shoulder.fbx");
	m_model[4].get()->LoadMesh("asset/model/Dreadnought/Upper.fbx");

	std::wstring basePath = L"asset/Texture/Dreadnought/";
	m_texture[0].get()->LoadTexture(basePath + L"T_DN_Arm_Albedo.tga", basePath + L"T_DN_Arm_NormalMap.png", basePath + L"T_DN_Arm_Occlusion.png", basePath + L"T_DN_Arm_Metallic.png");
	m_texture[1].get()->LoadTexture(basePath + L"T_DN_Head_Albedo.tga", basePath + L"T_DN_Head_NormalMap.png", basePath + L"T_DN_Head_Occlusion.png", basePath + L"T_DN_Head_Metallic.png");
	m_texture[2].get()->LoadTexture(basePath + L"T_DN_Lower_Albedo.tga", basePath + L"T_DN_Lower_NormalMap.png", basePath + L"T_DN_Lower_Occlusion.png", basePath + L"T_DN_Lower_Metallic.png");
	m_texture[3].get()->LoadTexture(basePath + L"T_DN_Shoulder_Albedo.tga", basePath + L"T_DN_Shoulder_NormalMap.png", basePath + L"T_DN_Shoulder_Occlusion.png", basePath + L"T_DN_Shoulder_Metallic.png");
	m_texture[4].get()->LoadTexture(basePath + L"T_DN_Upper_Albedo.tga", basePath + L"T_DN_Upper_NormalMap.png", basePath + L"T_DN_Upper_Occlusion.png", basePath + L"T_DN_Upper_Metallic.png");*/

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

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
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;
	XMStoreFloat4x4(&m_worldMTX, world);
}
void Player::Finalize() {
	//for (int i = 0; i < 5; i++) {
	//	m_texture[i].get()->Finalize();
	//}
}



void Player::Move() {

	m_position += GetRight() * MOVE_SPEED * XInput::GetInstance()->GetLeftThumb().x;
	m_position += GetForward() * MOVE_SPEED * XInput::GetInstance()->GetLeftThumb().y;
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
	//	SceneManager::GetInstance()->GetScene()->AddGameObject<DestroyEffect>()->AddParticle(m_position, XMFLOAT4(0, 0, 1, 1));
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
	ImGui::Text("GetForward()");
	ImGui::Text("  x:%lf, y:%lf, z:%lf", GetForward().x, GetForward().y, GetForward().z);
	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG

