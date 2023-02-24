
#include "texture.h"
#include "renderer.h"
#include"Renderer.h"
#include<DirectXTex.h>
#include <d3dx12.h>
#pragma comment(lib, "DirectXTex.lib")

// 拡張子を返す
std::wstring Texture::FileExtension(std::wstring path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}

void Texture::LoadDDSTexture(std::wstring fileName)
{
	ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();
	HRESULT hr{};

	TexMetadata meta = {};
	ScratchImage scratch = {};

	auto ext = FileExtension(fileName);
	std::vector<D3D12_SUBRESOURCE_DATA> textureSubresources;

	if (ext == L"tga"){
		LoadFromTGAFile(fileName.c_str(), &meta, scratch);
	}
	else if (ext == L"dds"){
		LoadFromDDSFile(fileName.c_str(), DDS_FLAGS_NONE, &meta, scratch);
	}
	else {
		LoadFromWICFile(fileName.c_str(), WIC_FLAGS_NONE, &meta, scratch);
	}


	const DirectX::Image* img = scratch.GetImage(0, 0, 0);
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(meta.format,
		(UINT)meta.width,
		(UINT)meta.height,
		static_cast<UINT>(meta.arraySize),
		static_cast<UINT>(meta.mipLevels));

	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 1.0f;
	clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	Renderer::GetInstance()->GetDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&m_resource));

	m_resource->WriteToSubresource(0,
		nullptr, //全領域へコピー
		img->pixels, //元データアドレス
		static_cast<UINT>(img->rowPitch), //1ラインサイズ
		static_cast<UINT>(img->slicePitch) //全サイズ
	);
	

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

	resourceViewDesc.Format = desc.Format;
	resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.PlaneSlice = 0;
	resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	resourceViewDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handleSrv = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(m_resource.Get(),
		&resourceViewDesc, handleSrv);


}
void Texture::Finalize() {
	m_descriptorHeap.Get()->Release();
	m_resource.Get()->Release();
}

