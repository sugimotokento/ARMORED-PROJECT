#include"FieldObject.h"
#include"Renderer.h"

void FieldObject::Initialize() {
	ModelLoader::GetInstance()->LoadRequest(static_cast<ModelLoader::Index::ModelID>(m_modelID));
	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);
}
void FieldObject::Update() {

}
void FieldObject::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();


	XMMATRIX world;
	CreateWorldMTX(m_scale, m_position, m_rotation);
	world = XMLoadFloat4x4(&m_worldMTX);

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

	if(m_isDrawLineMode)
		ModelLoader::GetInstance()->Draw(static_cast<ModelLoader::Index::ModelID>(m_modelID), D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else
		ModelLoader::GetInstance()->Draw(static_cast<ModelLoader::Index::ModelID>(m_modelID));
}
void FieldObject::Finalize() {

}

void FieldObject::SetModelID(Index::FieldModelID id) {
	m_modelID = id; 
	ModelLoader::GetInstance()->LoadRequest(static_cast<ModelLoader::Index::ModelID>(m_modelID));

}