#include"SkyDome.h"


void SkyDome::Initialize() {
	model=std::make_unique<Model>();
	model.get()->LoadMesh("asset/model/Field/skydome.obj");
	
	texture = std::make_unique<TextureGeometry>();
	texture.get()->LoadTexture(L"asset/Texture/Black.png");

	m_rotation = (XMFLOAT3(0, 0, 0));
	m_scale = (XMFLOAT3(100000, 100000, 100000));
	m_position=(XMFLOAT3(0, 0, 0));


	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

}
void SkyDome::Update() {

}
void SkyDome::Draw() {
	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

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
	constant->cameraPosition = CameraManager::GetInstance()->GetMainPosition();
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { texture.get()->GetDescriptorHeap().Get()};
	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(1, dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1,
		dh[0]->GetGPUDescriptorHandleForHeapStart()
	);

	if (Renderer::GetInstance()->GetNowPipelineStateID() == Renderer::Index::PIPELINE_STATE_ID_GEOMETRY_ALPHA)
		Renderer::GetInstance()->SetPipeline(Renderer::Index::PIPELINE_STATE_ID_SKYDOME);

	model.get()->Draw();

	Renderer::GetInstance()->SetNowBasePipeline();
}
void SkyDome::Finalize() {
	texture.get()->Finalize();


}