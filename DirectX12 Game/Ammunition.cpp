#include"Ammunition.h"
#include"XMMath.h"

void Ammunition::Initialize() {
	m_model = std::make_unique<Model>();
	m_model.get()->LoadMesh("asset/model/Bullet/ScifiRifle2Projectile.fbx");

	m_texture = std::make_unique<TextureGeometry>();
	std::wstring basePath = L"asset/Texture/Bullet/";
	m_texture.get()->LoadTexture(
		basePath + L"ScifiRifle2ProjectileAlbedo.png",
		L"asset/Texture/White.png",
		L"asset/Texture/White.png",
		L"asset/Texture/Black.png",
		basePath + L"ScifiRifle2ProjectileEmmision.png"
	);

	Renderer::GetInstance()->CreateConstantBuffer(m_constantBuffer);

	m_scale = XMFLOAT3(0.01f, 0.01f, 0.2f);
}
void Ammunition::Update() {
	m_position += m_moveDir * m_speed;


	XMFLOAT3 dist;
	dist = m_startPosition - m_position;
	if (XMMath::Length(dist) > m_range) {
		SetDestroy();
	}
}
void Ammunition::Draw() {
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

	constant->world = matrix;
	constant->reflectRate = XMFLOAT4(0, 0, 0, 0);


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());


	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { m_texture.get()->GetDescriptorHeap().Get() };
	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(1, dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1,
		dh[0]->GetGPUDescriptorHandleForHeapStart()
	);
	m_model.get()->Draw();

}
void Ammunition::Finalize() {
	m_texture.get()->Finalize();
	m_constantBuffer.Get()->Release();
}