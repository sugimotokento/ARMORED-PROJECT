#include"SkyDome.h"


void SkyDome::Initialize() {
	model=std::make_unique<Model>();
	model.get()->LoadMesh("asset/model/skydome.obj");
	m_rotation = (XMFLOAT3(0, 0, 0));
	m_scale = (XMFLOAT3(1000, 1000, 1000));
	m_position=(XMFLOAT3(0, 0, 0));


	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void SkyDome::Update() {

}
void SkyDome::Draw() {
	////マトリクス設定
	XMMATRIX lightView = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_SHADOW);

	XMMATRIX view = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_MAIN);
	XMMATRIX projection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_MAIN);

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;

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

	constant->isWater = false;

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	model.get()->Draw();
}
void SkyDome::Finalize() {

}