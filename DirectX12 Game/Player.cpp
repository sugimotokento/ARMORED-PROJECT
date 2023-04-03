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
#include"Weapon.h"
#include"Shotgun.h"
#include"AssaultRifle.h"
#include"Afterburner.h"
#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG



Player::Player() {

}
void Player::Initialize() {
	m_position = XMFLOAT3(0, 1, 0);
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	//腕を子オブジェクトとして追加
	Arm* arm[Arm::Index::MAX];
	arm[Index::LEFT] = AddChild<Arm>();
	arm[Index::RIGHT] = AddChild<Arm>();
	arm[Index::LEFT]->SetModelID(Arm::Index::ArmModelID::LEFT);
	arm[Index::RIGHT]->SetModelID(Arm::Index::ArmModelID::RIGHT);

	//武器を腕の子オブジェクトとして追加
	m_lightWeapon[Index::LEFT] = arm[Index::LEFT]->AddChild<Shotgun>();
	m_lightWeapon[Index::RIGHT] = arm[Index::RIGHT]->AddChild<AssaultRifle>();
	m_lightWeapon[Index::LEFT]->SetPosition(XMFLOAT3(0, 0, 0.78f));
	m_lightWeapon[Index::RIGHT]->SetPosition(XMFLOAT3(0, 0, 0.78f));


	//エフェクトを子オブジェクトとして追加
	m_afterburner = AddChild<Afterburner>();
	m_afterburner->SetPosition(XMFLOAT3(0, 2.857f, -0.143f));


	//モデルのロードリクエスト
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HEAD);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_EYE);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_ARM);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_LOWER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_SHOULDER);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_UPPER);


	//定数バッファの作成
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
	GameObject::Update();
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
	CreateWorldMTX(trans, rot, size);
	XMMATRIX world = XMLoadFloat4x4(&m_worldMTX);


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

	GameObject::Draw();
}
void Player::Finalize() {
	GameObject::Finalize();
}



void Player::Move() {
	float deceleration =max(0.1f, 1-XInput::GetInstance()->GetLeftTrigger());
	float acceleration = XInput::GetInstance()->GetRightTrigger();
	float boostSpeed = BOOST_SPEED_MAX * acceleration;
	float speed = (0.5f* deceleration + boostSpeed);

	m_afterburner->SetBoosterRatio(acceleration);

	if (fabsf(XInput::GetInstance()->GetLeftThumb().x) > 0.01f) {
		m_position += GetRight() * speed * XInput::GetInstance()->GetLeftThumb().x;
	}
	if (fabsf(XInput::GetInstance()->GetLeftThumb().y) > 0.01f) {
		m_position += GetForward() * speed * XInput::GetInstance()->GetLeftThumb().y;
	}
}
void Player::Rotation() {
	if (fabsf(XInput::GetInstance()->GetRightThumb().x) > 0.01f) {
		m_rotation.y += 0.1f * XInput::GetInstance()->GetRightThumb().x;
	}

}
 
void Player::Shot() {
	if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		m_lightWeapon[Index::LEFT]->Shot();
	}
	if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		m_lightWeapon[Index::RIGHT]->Shot();
	}
}


#ifdef _DEBUG
bool Player::ImguiDebug() {
	//ImGui::Begin("Player");
	//static float pos[3];
	//static float scale = 1;
	//ImGui::SliderFloat3("AfterburnerOffset", pos, -5, 5);
	//m_afterburner->SetPosition(XMFLOAT3(pos[0], pos[1], pos[2]));

	//ImGui::SliderFloat("Scale", &scale, 0, 5);
	//m_afterburner->SetScale(XMFLOAT3(scale, scale, scale));
	//ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG

