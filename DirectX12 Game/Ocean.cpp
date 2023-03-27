#include"Ocean.h"
#include"Renderer.h"

#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG


Ocean::Ocean() {
	m_position = XMFLOAT3(0, 0, 10);
	m_scale = XMFLOAT3(300, 1, 300);

#ifdef _DEBUG
	//std::function<bool()> f = std::bind(&Ocean::ImguiDebug, this);
	//ImguiRenderer::GetInstance()->AddFunction(f);
#endif // _DEBUG
}
void Ocean::Initialize() {
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

	const float sizeX = 5000.0f;
	const float sizeZ = 5000.0f;

	//3Dポリゴン用頂点の準備
	{
		//頂点座標の設定
		{
			buffer[0].position = { -sizeX, 0, -sizeZ };
			buffer[1].position = { -sizeX, 0, sizeZ };
			buffer[2].position = { sizeX, 0, -sizeZ };
			buffer[3].position = { -sizeX, 0, sizeZ };
			buffer[4].position = { sizeX, 0, sizeZ };
			buffer[5].position = { sizeX, 0, -sizeZ };

		}
		//法線ベクトルの設定
		{
			for (int i = 0; i < 6; i++) {
				buffer[i].normal = { 0.0f, 1.0f, 0.0f };
			}
		}
		//反射光の設定
		{
			for (int i = 0; i < 6; i++) {
				buffer[i].diffuse = { 0, 0, 1, 1 };
			}
		}
		////テクスチャの設定
		{
			const float TEX_SIZE = 50000;
			buffer[0].texCoord = { TEX_SIZE, TEX_SIZE };
			buffer[1].texCoord = { 0.0f, TEX_SIZE };
			buffer[2].texCoord = { TEX_SIZE, 0.0f };
			buffer[3].texCoord = { 0.0f, TEX_SIZE };
			buffer[4].texCoord = { 0.0f, 0.0f };
			buffer[5].texCoord = { TEX_SIZE, 0.0f };
		}
	}

	m_vertexBuffer->Unmap(0, nullptr);
}
void Ocean::Update() {

}
void Ocean::Draw() {

	////マトリクス設定
	XMMATRIX lightView = CameraManager::GetInstance()->GetViewMatrix(CameraManager::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = CameraManager::GetInstance()->GetProjectionMatrix(CameraManager::Index::CAMERA_SHADOW);

	XMMATRIX view = CameraManager::GetInstance()->GetMainViewMatrix();
	XMMATRIX projection = CameraManager::GetInstance()->GetMainProjectionMatrix();

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;

	static float waterParamW = 0;
	waterParamW += 0.1f;

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
	constant->cameraPosition = CameraManager::GetInstance()->GetMainPosition();

	constant->waterParam.x = m_waterParam.x;
	constant->waterParam.y = m_waterParam.y;
	constant->waterParam.z = m_waterParam.z;
	constant->waterParam.w = waterParamW;

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//頂点バッファ設定
	D3D12_VERTEX_BUFFER_VIEW bufferView{};
	bufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	bufferView.StrideInBytes = sizeof(Vertex3DBuffer);
	bufferView.SizeInBytes = sizeof(Vertex3DBuffer) * 6;
	Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &bufferView);


	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { Renderer::GetInstance()->GetSRVDescriptorHeap().Get() };
	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(_countof(dh), dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
		1,
		Renderer::GetInstance()->GetSRVDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart()
	);




	//トポロジ設定
	Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画
	if(Renderer::GetInstance()->GetNowPipelineStateID()==Renderer::Index::PIPELINE_STATE_ID_GEOMETRY_ALPHA)
		Renderer::GetInstance()->SetPipeline(Renderer::Index::PIPELINE_STATE_ID_WATER);
	Renderer::GetInstance()->GetCommandList().Get()->DrawInstanced(6, 1, 0, 0);
	if (Renderer::GetInstance()->GetNowPipelineStateID() == Renderer::Index::PIPELINE_STATE_ID_GEOMETRY_ALPHA)
		Renderer::GetInstance()->SetNowBasePipeline();


}
void Ocean::Finalize() {


}



#ifdef _DEBUG
bool Ocean::ImguiDebug() {
	ImGui::Begin("Ocean");
	float param[4]{ m_waterParam.x, m_waterParam.y, m_waterParam.z, m_waterParam.w };
	ImGui::SliderFloat4("OceanParam", param, 0, 1);
	m_waterParam.x = param[0];
	m_waterParam.y = param[1];
	m_waterParam.z = param[2];
	m_waterParam.w = param[3];
	ImGui::End();
	return GetIsDestroy();
}
#endif // _DEBUG