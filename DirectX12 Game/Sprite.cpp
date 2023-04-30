#include "main.h"
#include "renderer.h"
#include "input.h"
#include "Sprite.h"
#include"texture.h"

Sprite::Sprite(std::wstring textureName) {
	ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();

	D3D12_HEAP_PROPERTIES heapProperties{};
	D3D12_RESOURCE_DESC resourceDesc{};

	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;

	//頂点バッファの作成
	resourceDesc.Width = sizeof(Vertex3DBuffer) * 4;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));

	//定数バッファの作成
	resourceDesc.Width = 256;//定数バッファは256byteアライン
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_constantBuffer));

	//頂点データの書き込み
	Vertex3DBuffer* buffer{};
	m_vertexBuffer->Map(0, nullptr, (void**)&buffer);
	buffer[0].position = { 0.5f,0.5f,0.0f };
	buffer[1].position = { -0.5f,0.5f,0.0f };
	buffer[2].position = { 0.5f,-0.5f,0.0f };
	buffer[3].position = { -0.5f,-0.5f,0.0f };
	buffer[0].normal = { 0.0f,1.0f,0.0f };
	buffer[1].normal = { 0.0f,1.0f,0.0f };
	buffer[2].normal = { 0.0f,1.0f,0.0f };
	buffer[3].normal = { 0.0f,1.0f,0.0f };
	buffer[0].texCoord = { 1.0f,1.0f };
	buffer[1].texCoord = { 0.0f,1.0f };
	buffer[2].texCoord = { 1.0f,0.0f };
	buffer[3].texCoord = { 0.0f,0.0f };
	buffer[0].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[1].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[2].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[3].diffuse = { 1.0f,1.0f,1.0f,1.0f };


	m_vertexBuffer->Unmap(0, nullptr);



	m_texture = new Texture();
	m_texture->LoadTexture(textureName);
}


void Sprite::Draw() {
	//マトリクス設定
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH,
		SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX pos = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX world = size * rot * pos;


	//定数バッファ設定
	Constant2DBuffer* constant;
	m_constantBuffer->Map(0, nullptr, (void**)&constant);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->world = matrix;


	constant->radialRatio = m_radialRatio;
	constant->radialStartAngle = m_radialStartAngle;
	constant->isRadialInvers = m_isRadialInvers;

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//頂点バッファ設定
	D3D12_VERTEX_BUFFER_VIEW vertexView{};
	vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(Vertex3DBuffer);
	vertexView.SizeInBytes = sizeof(Vertex3DBuffer) * 4;
	Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexView);

	//////テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { m_texture->GetDescriptorHeap().Get()};

	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(_countof(dh), dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1, m_texture->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	
	//トポロジ設定
	Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//描画
	Renderer::GetInstance()->GetCommandList().Get()->DrawInstanced(4, 1, 0, 0);
}
void Sprite::Finalize() {

	m_texture->Finalize();
	delete m_texture;
}



void Sprite::SetPosition(XMFLOAT3 position) {
	m_position = position;
}
void Sprite::SetRotation(XMFLOAT3 rotation) {
	m_rotation = rotation;
}
void Sprite::SetScale(XMFLOAT3 scale) {
	m_scale = scale;
}


void Sprite::SetColor(XMFLOAT4 color) {
	//頂点データの書き込み
	Vertex3DBuffer* buffer{};
	m_vertexBuffer->Map(0, nullptr, (void**)&buffer);
	buffer[0].position = { 0.5f,0.5f,0.0f };
	buffer[1].position = { -0.5f,0.5f,0.0f };
	buffer[2].position = { 0.5f,-0.5f,0.0f };
	buffer[3].position = { -0.5f,-0.5f,0.0f };
	buffer[0].normal = { 0.0f,1.0f,0.0f };
	buffer[1].normal = { 0.0f,1.0f,0.0f };
	buffer[2].normal = { 0.0f,1.0f,0.0f };
	buffer[3].normal = { 0.0f,1.0f,0.0f };
	buffer[0].texCoord = { 0.0f,0.0f };
	buffer[1].texCoord = { 1.0f,0.0f };
	buffer[2].texCoord = { 0.0f,1.0f };
	buffer[3].texCoord = { 1.0f,1.0f };
	buffer[0].diffuse = { color };
	buffer[1].diffuse = { color };
	buffer[2].diffuse = { color };
	buffer[3].diffuse = { color };


	m_vertexBuffer->Unmap(0, nullptr);
}