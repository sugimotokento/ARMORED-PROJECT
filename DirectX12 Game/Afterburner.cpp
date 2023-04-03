#include"Afterburner.h"
#include"ModelLoader.h"
#include"XMMath.h"
#include <cstdlib>

void Afterburner::Initialize() {
	//�萔�o�b�t�@�̍쐬
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_EFFECT_AFTERBURNER);

	m_scale = XMFLOAT3(1, 1, 1);
}
void Afterburner::Update() {
	XMFLOAT3 maxScale = XMFLOAT3(0.24f, 0.24f, 0.24f);
	XMFLOAT3 minScale = XMFLOAT3(0.01f, 0.01f, 0.01f);

	//�����_���ŃX�P�[���������ς���
	int randomInt = rand() % 10000;//%10�ɂ����10�p�^�[���̒l�ɂ����Ȃ�Ȃ��̂�10000(�����̓e�L�g�[)
	float randomFloat = ((float)randomInt) / 100000;//0�`0.1�̒l�ɂ���

	m_scale = (maxScale * m_boosterRatio + minScale * (1 - m_boosterRatio))*(1- randomFloat);
}
void Afterburner::Draw() {
	////�}�g���N�X�ݒ�
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMFLOAT4X4 parentWorldTemp = m_parent->GetWorldMTX();
	XMMATRIX parentWorld = XMLoadFloat4x4(&parentWorldTemp);
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	CreateWorldMTX(trans, rot, size);
	XMMATRIX world = XMLoadFloat4x4(&m_worldMTX);


	XMStoreFloat4x4(&m_worldMTX, world);

	//�萔�o�b�t�@�ݒ�
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

	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_EFFECT_AFTERBURNER);
}
void Afterburner::Finalize() {

}