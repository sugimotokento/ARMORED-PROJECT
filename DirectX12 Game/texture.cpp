
#include "main.h"
#include "texture.h"
#include "renderer.h"
#include "DDSTextureLoader12.h"




void Texture::LoadDDSTexture(const wchar_t* fileName)
{
	ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();
	HRESULT hr{};

	std::unique_ptr<uint8_t[]>ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> subresourceDate;
	LoadDDSTextureFromFile(device.Get(), fileName, m_resource.GetAddressOf(), ddsData, subresourceDate);

	for (int i = 0; i < subresourceDate.size(); i++) {
		int width = (int)(subresourceDate[i].RowPitch / 4);
		int height = (int)(subresourceDate[i].SlicePitch / subresourceDate[i].RowPitch);
		D3D12_BOX box = { 0,0,0,(UINT)width,(UINT)height,1 };
		m_resource->WriteToSubresource(i, &box, subresourceDate[i].pData,
			(UINT)subresourceDate[i].RowPitch, (UINT)subresourceDate[i].SlicePitch);
	}


	//デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	hr = device->CreateDescriptorHeap(&descriptorHeapDesc,
		IID_PPV_ARGS(&m_descriptorHeap));
	assert(SUCCEEDED(hr));

	//シェーダリソースビュー作成
	D3D12_CPU_DESCRIPTOR_HANDLE handleSrv{};
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};

	resourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.PlaneSlice = 0;
	resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	resourceViewDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handleSrv = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(*m_resource.GetAddressOf(),
		&resourceViewDesc, handleSrv);


}
void Texture::Finalize() {
	if (m_descriptorHeap.Get())
		m_descriptorHeap.Get()->Release();
	if (m_resource.Get())
		m_resource.Get()->Release();
}

