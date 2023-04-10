#include"GameObject.h"
#include"Arm.h"
#include"Player.h"
#include"ImguiRenderer.h"

Arm::Arm() {

}
void Arm::SetModelID(Index::ArmModelID side) {
	m_side = side;

	if (m_side == Arm::Index::ArmModelID::LEFT) {
		m_position = XMFLOAT3(-0.822f, 2.511f, 0.156f);
		//m_position = XMFLOAT3(-0, 0, 0);
		ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L);
	}
	else if (m_side == Arm::Index::ArmModelID::RIGHT) {
		m_position = XMFLOAT3(0.822f, 2.511f, 0.156f);
		//m_position = XMFLOAT3(0, 0, 0);
		ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R);
	}
}
void Arm::Initialize() {
	
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);


	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void Arm::Update() {
	GameObject::Update();
}
void Arm::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	CreateWorldMTX(m_scale, m_position, m_rotation);
	XMMATRIX world = XMLoadFloat4x4(&m_worldMTX);


	//定数バッファ設定
	Constant3DBuffer* constant;
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

	if (m_side == Index::LEFT)
		ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L);
	else
		ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R);

	GameObject::Draw();
}
void Arm::Finalize() {
	GameObject::Finalize();
}


