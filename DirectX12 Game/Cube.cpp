#include "main.h"
#include "renderer.h"
#include "Cube.h"


Cube::Cube() {
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
	resourceDesc.Width = 256;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_constantBuffer));

	//頂点データの書き込み
	Vertex3DBuffer* buffer{};
	m_vertexBuffer->Map(0, nullptr, (void**)&buffer);

	const float sizeX = 0.5f;
	const float sizeY = 0.5f;
	const float sizeZ = 0.5f;

	//3Dポリゴン用頂点の準備
	{
		//頂点座標の設定
		{
			buffer[0].position = { -sizeX, sizeY, -sizeZ };
			buffer[1].position = { sizeX, sizeY, -sizeZ };
			buffer[2].position = { -sizeX, -sizeY, -sizeZ };
			buffer[3].position = { sizeX, sizeY, -sizeZ };
			buffer[4].position = { sizeX, -sizeY, -sizeZ };
			buffer[5].position = { -sizeX, -sizeY, -sizeZ };
			buffer[6].position = { sizeX, sizeY, sizeZ };
			buffer[7].position = { sizeX, -sizeY, -sizeZ };
			buffer[8].position = { sizeX, sizeY, -sizeZ };
			buffer[9].position = { sizeX, sizeY, sizeZ };
			buffer[10].position = { sizeX, -sizeY, sizeZ };
			buffer[11].position = { sizeX, -sizeY, -sizeZ };
			buffer[12].position = { -sizeX, sizeY, sizeZ };
			buffer[14].position = { -sizeX, -sizeY, -sizeZ };
			buffer[13].position = { -sizeX, sizeY, -sizeZ };
			buffer[15].position = { -sizeX, sizeY, sizeZ };
			buffer[17].position = { -sizeX, -sizeY, sizeZ };
			buffer[16].position = { -sizeX, -sizeY, -sizeZ };
			buffer[18].position = { -sizeX, sizeY, sizeZ };
			buffer[19].position = { -sizeX, -sizeY, sizeZ };
			buffer[20].position = { sizeX, sizeY, sizeZ };
			buffer[21].position = { sizeX, sizeY, sizeZ };
			buffer[22].position = { -sizeX, -sizeY, sizeZ };
			buffer[23].position = { sizeX, -sizeY, sizeZ };
			buffer[24].position = { -sizeX, sizeY, -sizeZ };
			buffer[25].position = { -sizeX, sizeY, sizeZ };
			buffer[26].position = { sizeX, sizeY, -sizeZ };
			buffer[27].position = { -sizeX, sizeY, sizeZ };
			buffer[28].position = { sizeX, sizeY, sizeZ };
			buffer[29].position = { sizeX, sizeY, -sizeZ };
			buffer[30].position = { -sizeX, -sizeY, -sizeZ };
			buffer[32].position = { -sizeX, -sizeY, sizeZ };
			buffer[31].position = { sizeX, -sizeY, -sizeZ };
			buffer[33].position = { -sizeX, -sizeY, sizeZ };
			buffer[34].position = { sizeX, -sizeY, -sizeZ };
			buffer[35].position = { sizeX, -sizeY, sizeZ };
		}
		//法線ベクトルの設定
		{
			for (int i = 0; i < 6; i++) {
				buffer[i].normal = { 0.0f, 0.0f, -1.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 6].normal = { 1.0f, 0.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 12].normal = { -1.0f, 0.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 18].normal = { 0.0f, 0.0f, 1.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 24].normal = { 0.0f, 1.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 30].normal = { 0.0f, -1.0f, 0.0f };
			}

		}
		//反射光の設定
		{
			for (int i = 0; i < 36; i++) {
				buffer[i].diffuse = { 1, 1, 1, 1 };
			}
		}
		////テクスチャの設定
		{
			buffer[0].texCoord = { 0.0f, 0.0f };
			buffer[1].texCoord = { 1.0f, 0.0f };
			buffer[2].texCoord = { 0.0f, 1.0f };
			buffer[3].texCoord = { 1.0f, 0.0f };
			buffer[4].texCoord = { 1.0f, 1.0f };
			buffer[5].texCoord = { 0.0f, 1.0f };
			buffer[6].texCoord = { 1.0f, 0.0f };
			buffer[7].texCoord = { 0.0f, 1.0f };
			buffer[8].texCoord = { 0.0f, 0.0f };
			buffer[9].texCoord = { 1.0f, 0.0f };
			buffer[10].texCoord = { 1.0f, 1.0f };
			buffer[11].texCoord = { 0.0f, 1.0f };
			buffer[12].texCoord = { 0.0f, 0.0f };
			buffer[13].texCoord = { 0.0f, 1.0f };
			buffer[14].texCoord = { 1.0f, 1.0f };
			buffer[15].texCoord = { 0.0f, 0.0f };
			buffer[16].texCoord = { 1.0f, 1.0f };
			buffer[17].texCoord = { 1.0f, 0.0f };
			buffer[18].texCoord = { 0.0f, 0.0f };
			buffer[19].texCoord = { 1.0f, 0.0f };
			buffer[20].texCoord = { 0.0f, 1.0f };
			buffer[21].texCoord = { 0.0f, 1.0f };
			buffer[22].texCoord = { 1.0f, 0.0f };
			buffer[23].texCoord = { 1.0f, 1.0f };
			buffer[24].texCoord = { 1.0f, 1.0f };
			buffer[25].texCoord = { 0.0f, 1.0f };
			buffer[26].texCoord = { 1.0f, 0.0f };
			buffer[27].texCoord = { 0.0f, 1.0f };
			buffer[28].texCoord = { 0.0f, 0.0f };
			buffer[29].texCoord = { 1.0f, 0.0f };
			buffer[30].texCoord = { 1.0f, 1.0f };
			buffer[31].texCoord = { 1.0f, 0.0f };
			buffer[32].texCoord = { 0.0f, 1.0f };
			buffer[33].texCoord = { 0.0f, 1.0f };
			buffer[34].texCoord = { 1.0f, 0.0f };
			buffer[35].texCoord = { 0.0f, 0.0f };
		}
	}

	m_vertexBuffer->Unmap(0, nullptr);

}


void Cube::Draw(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation, float reflectRate) {
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

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

	constant->reflectRate = XMFLOAT4(reflectRate, reflectRate, reflectRate, reflectRate);

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//頂点バッファ設定
	D3D12_VERTEX_BUFFER_VIEW bufferView{};
	bufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	bufferView.StrideInBytes = sizeof(Vertex3DBuffer);
	bufferView.SizeInBytes = sizeof(Vertex3DBuffer) * 36;
	Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &bufferView);


	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { Renderer::GetInstance()->GetSRVDescriptorHeap().Get()};
	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(_countof(dh), dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1, Renderer::GetInstance()->GetSRVDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());


	//トポロジ設定
	Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	Renderer::GetInstance()->GetCommandList().Get()->DrawInstanced(36, 1, 0, 0);
}


void Cube::SetColor(XMFLOAT4 color) {
	//頂点データの書き込み
	Vertex3DBuffer* buffer{};
	m_vertexBuffer->Map(0, nullptr, (void**)&buffer);

	const float sizeX = 0.5f;
	const float sizeY = 0.5f;
	const float sizeZ = 0.5f;

	//3Dポリゴン用頂点の準備
	{
		//頂点座標の設定
		{
			buffer[0].position = { -sizeX, sizeY, -sizeZ };
			buffer[1].position = { sizeX, sizeY, -sizeZ };
			buffer[2].position = { -sizeX, -sizeY, -sizeZ };
			buffer[3].position = { sizeX, sizeY, -sizeZ };
			buffer[4].position = { sizeX, -sizeY, -sizeZ };
			buffer[5].position = { -sizeX, -sizeY, -sizeZ };
			buffer[6].position = { sizeX, sizeY, sizeZ };
			buffer[7].position = { sizeX, -sizeY, -sizeZ };
			buffer[8].position = { sizeX, sizeY, -sizeZ };
			buffer[9].position = { sizeX, sizeY, sizeZ };
			buffer[10].position = { sizeX, -sizeY, sizeZ };
			buffer[11].position = { sizeX, -sizeY, -sizeZ };
			buffer[12].position = { -sizeX, sizeY, sizeZ };
			buffer[14].position = { -sizeX, -sizeY, -sizeZ };
			buffer[13].position = { -sizeX, sizeY, -sizeZ };
			buffer[15].position = { -sizeX, sizeY, sizeZ };
			buffer[17].position = { -sizeX, -sizeY, sizeZ };
			buffer[16].position = { -sizeX, -sizeY, -sizeZ };
			buffer[18].position = { -sizeX, sizeY, sizeZ };
			buffer[19].position = { -sizeX, -sizeY, sizeZ };
			buffer[20].position = { sizeX, sizeY, sizeZ };
			buffer[21].position = { sizeX, sizeY, sizeZ };
			buffer[22].position = { -sizeX, -sizeY, sizeZ };
			buffer[23].position = { sizeX, -sizeY, sizeZ };
			buffer[24].position = { -sizeX, sizeY, -sizeZ };
			buffer[25].position = { -sizeX, sizeY, sizeZ };
			buffer[26].position = { sizeX, sizeY, -sizeZ };
			buffer[27].position = { -sizeX, sizeY, sizeZ };
			buffer[28].position = { sizeX, sizeY, sizeZ };
			buffer[29].position = { sizeX, sizeY, -sizeZ };
			buffer[30].position = { -sizeX, -sizeY, -sizeZ };
			buffer[32].position = { -sizeX, -sizeY, sizeZ };
			buffer[31].position = { sizeX, -sizeY, -sizeZ };
			buffer[33].position = { -sizeX, -sizeY, sizeZ };
			buffer[34].position = { sizeX, -sizeY, -sizeZ };
			buffer[35].position = { sizeX, -sizeY, sizeZ };
		}
		//法線ベクトルの設定
		{
			for (int i = 0; i < 6; i++) {
				buffer[i].normal = { 0.0f, 0.0f, -1.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 6].normal = { 1.0f, 0.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 12].normal = { -1.0f, 0.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 18].normal = { 0.0f, 0.0f, 1.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 24].normal = { 0.0f, 1.0f, 0.0f };
			}

			for (int i = 0; i < 6; i++) {
				buffer[i + 30].normal = { 0.0f, -1.0f, 0.0f };
			}

		}
		//反射光の設定
		{
			for (int i = 0; i < 36; i++) {
				buffer[i].diffuse = { color.x, color.y, color.z, color.w };
			}
		}
		////テクスチャの設定
		{
			buffer[0].texCoord = { 0.0f, 0.0f };
			buffer[1].texCoord = { 1.0f, 0.0f };
			buffer[2].texCoord = { 0.0f, 1.0f };
			buffer[3].texCoord = { 1.0f, 0.0f };
			buffer[4].texCoord = { 1.0f, 1.0f };
			buffer[5].texCoord = { 0.0f, 1.0f };
			buffer[6].texCoord = { 1.0f, 0.0f };
			buffer[7].texCoord = { 0.0f, 1.0f };
			buffer[8].texCoord = { 0.0f, 0.0f };
			buffer[9].texCoord = { 1.0f, 0.0f };
			buffer[10].texCoord = { 1.0f, 1.0f };
			buffer[11].texCoord = { 0.0f, 1.0f };
			buffer[12].texCoord = { 0.0f, 0.0f };
			buffer[13].texCoord = { 0.0f, 1.0f };
			buffer[14].texCoord = { 1.0f, 1.0f };
			buffer[15].texCoord = { 0.0f, 0.0f };
			buffer[16].texCoord = { 1.0f, 1.0f };
			buffer[17].texCoord = { 1.0f, 0.0f };
			buffer[18].texCoord = { 0.0f, 0.0f };
			buffer[19].texCoord = { 1.0f, 0.0f };
			buffer[20].texCoord = { 0.0f, 1.0f };
			buffer[21].texCoord = { 0.0f, 1.0f };
			buffer[22].texCoord = { 1.0f, 0.0f };
			buffer[23].texCoord = { 1.0f, 1.0f };
			buffer[24].texCoord = { 1.0f, 1.0f };
			buffer[25].texCoord = { 0.0f, 1.0f };
			buffer[26].texCoord = { 1.0f, 0.0f };
			buffer[27].texCoord = { 0.0f, 1.0f };
			buffer[28].texCoord = { 0.0f, 0.0f };
			buffer[29].texCoord = { 1.0f, 0.0f };
			buffer[30].texCoord = { 1.0f, 1.0f };
			buffer[31].texCoord = { 1.0f, 0.0f };
			buffer[32].texCoord = { 0.0f, 1.0f };
			buffer[33].texCoord = { 0.0f, 1.0f };
			buffer[34].texCoord = { 1.0f, 0.0f };
			buffer[35].texCoord = { 0.0f, 0.0f };
		}
	}

	m_vertexBuffer->Unmap(0, nullptr);
}

void Cube::Finalize() {
	m_constantBuffer.Get()->Release();
	m_vertexBuffer.Get()->Release();
}