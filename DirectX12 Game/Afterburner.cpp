#include"Afterburner.h"
#include"ModelLoader.h"
#include"XMMath.h"
#include <cstdlib>

void Afterburner::Initialize() {
	//定数バッファの作成
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);
	ModelLoader::GetInstance()->LoadRequest(ModelLoader::Index::MODEL_ID_EFFECT_AFTERBURNER);

	m_scale = XMFLOAT3(1, 1, 1);
}
void Afterburner::Update() {
	XMFLOAT3 maxScale = XMFLOAT3(0.24f, 0.24f, 0.24f);
	XMFLOAT3 minScale = XMFLOAT3(0.01f, 0.01f, 0.01f);

	//ランダムでスケールを少し変える
	int randomInt = rand() % 10000;//%10にすると10パターンの値にしかならないので10000(桁数はテキトー)
	float randomFloat = ((float)randomInt) / 100000;//0～0.1の値にする

	m_scale = (maxScale * m_boosterRatio + minScale * (1 - m_boosterRatio))*(1- randomFloat);
}
void Afterburner::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	CreateWorldMTX(m_scale, m_position, m_rotation);
	XMMATRIX world = XMLoadFloat4x4(&m_worldMTX);


	XMStoreFloat4x4(&m_worldMTX, world);

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

	ModelLoader::GetInstance()->Draw(ModelLoader::Index::MODEL_ID_EFFECT_AFTERBURNER);
}
void Afterburner::Finalize() {

}