#include "main.h"
#include "renderer.h"
#include "input.h"
#include "polygonDeferred.h"


PolygonDeferred::PolygonDeferred(){
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

	//���_�o�b�t�@�̍쐬
	resourceDesc.Width = sizeof(Vertex3DBuffer) * 4;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));

	//�萔�o�b�t�@�̍쐬
	resourceDesc.Width = 256;//�萔�o�b�t�@��256byte�A���C��
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_constantBuffer));

	//���_�f�[�^�̏�������
	Vertex3DBuffer* buffer{};
	m_vertexBuffer->Map(0, nullptr, (void**)&buffer);

	buffer[0].position = { 0.0f,0.0f,0.0f };
	buffer[1].position = { (float)SCREEN_WIDTH,0.0f,0.0f };
	buffer[2].position = { 0.0f,(float)SCREEN_HEIGHT,0.0f };
	buffer[3].position = { (float)SCREEN_WIDTH,(float)SCREEN_HEIGHT,0.0f };
	buffer[0].normal = { 0.0f,1.0f,0.0f };
	buffer[1].normal = { 0.0f,1.0f,0.0f };
	buffer[2].normal = { 0.0f,1.0f,0.0f };
	buffer[3].normal = { 0.0f,1.0f,0.0f };
	buffer[0].texCoord = { 0.0f,0.0f };
	buffer[1].texCoord = { 1.0f,0.0f };
	buffer[2].texCoord = { 0.0f,1.0f };
	buffer[3].texCoord = { 1.0f,1.0f };
	buffer[0].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[1].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[2].diffuse = { 1.0f,1.0f,1.0f,1.0f };
	buffer[3].diffuse = { 1.0f,1.0f,1.0f,1.0f };


	m_vertexBuffer->Unmap(0, nullptr);
}


void PolygonDeferred::Draw(ID3D12DescriptorHeap* texture){

	//�}�g���N�X�ݒ�
	XMMATRIX cameraView = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_MAIN);
	XMMATRIX cameraProjection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_MAIN);

	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH,
		SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);


	//�萔�o�b�t�@�ݒ�
	ConstantBuffer* constant;
	m_constantBuffer->Map(0, nullptr, (void**)&constant);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvpLight = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(cameraView * cameraProjection));
	constant->vp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->world = matrix;
	constant->cameraPosition = Camera::GetInstance()->GetPosition(Camera::Index::CAMERA_MAIN);


	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	//���_�o�b�t�@�ݒ�
	D3D12_VERTEX_BUFFER_VIEW vertexView{};
	vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(Vertex3DBuffer);
	vertexView.SizeInBytes = sizeof(Vertex3DBuffer) * 4;
	Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexView);


	//�e�N�X�`���ݒ�
	ID3D12DescriptorHeap* dh[] = { texture };
	Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(_countof(dh), dh);
	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1, texture->GetGPUDescriptorHandleForHeapStart());

	//�g�|���W�ݒ�
	Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�`��
	Renderer::GetInstance()->GetCommandList().Get()->DrawInstanced(4, 1, 0, 0);
}


void PolygonDeferred::Finalize() {
	m_vertexBuffer.Get()->Release();
	m_constantBuffer.Get()->Release();

}