#include"GameObject.h"
#include"Arm.h"
#include"Player.h"
#include"ImguiRenderer.h"

Arm::Arm() {

}
void Arm::Initialize(Index::ArmSide side, GameObject* parent) {
	m_side = side;
	m_scale = XMFLOAT3(1, 1, 1);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_parent = parent;

	if (m_side == Index::ArmSide::LEFT) {
		m_position = XMFLOAT3(-0.822f, 2.511f, 0.156f);
		//m_position = XMFLOAT3(-0, 0, 0);
		ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L);
	}
	else if (m_side == Index::ArmSide::RIGHT) {
		m_position = XMFLOAT3(0.822f, 2.511f, 0.156f);
		//m_position = XMFLOAT3(0, 0, 0);
		ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R);
	}

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void Arm::Update() {

}
void Arm::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMFLOAT4X4 parentWorldTemp = m_parent->GetWorldMTX();
	XMMATRIX parentWorld = XMLoadFloat4x4(&parentWorldTemp);
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = (size * rot * trans) * parentWorld;

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

	constant->world = matrix;
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	if (m_side == Index::LEFT)
		ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L);
	else
		ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R);
}
void Arm::Finalize() {

}


