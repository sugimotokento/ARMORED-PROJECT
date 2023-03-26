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
#include"ModelLoader.h"

#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG



Player::Player() {

}
void Player::Initialize() {
	m_position = XMFLOAT3(0, 1, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_arm[0] = std::make_unique<Arm>();
	m_arm[1] = std::make_unique<Arm>();
	m_arm[0].get()->Initialize(Arm::Index::LEFT, this);
	m_arm[1].get()->Initialize(Arm::Index::RIGHT, this);

	//モデルのロードリクエスト
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HEAD);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_EYE);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_ARM);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_LOWER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_SHOULDER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_UPPER);

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

#ifdef _DEBUG
	std::function<bool()> f = std::bind(&Player::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
#endif // _DEBUG
}

void Player::Update() {
#ifdef _DEBUG//デバッグカメラだったらReturn
	if (CameraManager::GetInstance()->GetMainCameraIndex() == CameraManager::Index::CAMERA_DEBUG)return;
#endif
	Move();
	Rotation();
	Shot();
	FieldCollision();
	m_arm[0].get()->Update();
	m_arm[1].get()->Update();
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

	//定数バッファ設定
	ConstantBuffer* constant;
	m_constantBuffer->Map(0, nullptr, (void**)&constant);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * lightView * lightProjection));
	constant->wvpLight = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(view * projection));
	constant->vp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->cameraPosition = CameraManager::GetInstance()->GetMainPosition();
	constant->world = matrix;
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//モデル描画
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HEAD);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_ARM);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_LOWER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_SHOULDER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_UPPER);
	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_EYE);

	m_arm[0].get()->Draw();
	m_arm[1].get()->Draw();

}
void Player::Finalize() {
 
}



void Player::Move() {

	if (fabsf(XInput::GetInstance()->GetLeftThumb().x) > 0.01f) {
		m_position += GetRight() * 0.5f * XInput::GetInstance()->GetLeftThumb().x;
	}
	if (fabsf(XInput::GetInstance()->GetLeftThumb().y) > 0.01f) {
		m_position += GetForward() * 0.5f * XInput::GetInstance()->GetLeftThumb().y;
	}
}
void Player::Rotation() {
	if (fabsf(XInput::GetInstance()->GetRightThumb().x) > 0.01f) {
		m_rotation.y += 0.1f * XInput::GetInstance()->GetRightThumb().x;
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


#ifdef _DEBUG
bool Player::ImguiDebug() {
	//ImGui::Begin("Player");
	//ImGui::Text("GetForward()");
	//ImGui::Text("  x:%lf, y:%lf, z:%lf", GetForward().x, GetForward().y, GetForward().z);
	//ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG

