
#include "TextureGeometry.h"
#include "renderer.h"
#include"Renderer.h"
#include<DirectXTex.h>
#include <d3dx12.h>
#pragma comment(lib, "DirectXTex.lib")

// 拡張子を返す
std::wstring TextureGeometry::FileExtension(std::wstring path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}

void TextureGeometry::LoadTexture(std::wstring alpbedPath, std::wstring normalPath, std::wstring occlusionPath, std::wstring metalPath, std::wstring emmisionPath)
{
	ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();
	HRESULT hr{}; 

	std::wstring fileName[Renderer::Index::MODEL_TEX_RESOURCE_NUM] = {
		alpbedPath, 
		normalPath,
		occlusionPath,
		metalPath,
		emmisionPath
	};
	TexMetadata meta[Renderer::Index::MODEL_TEX_RESOURCE_NUM] = {};
	ScratchImage scratch[Renderer::Index::MODEL_TEX_RESOURCE_NUM] = {};
	CD3DX12_RESOURCE_DESC desc[Renderer::Index::MODEL_TEX_RESOURCE_NUM] = {};

	for (int i = 0; i < Renderer::Index::MODEL_TEX_RESOURCE_NUM; i++) {
		auto ext = FileExtension(fileName[i]);
		std::vector<D3D12_SUBRESOURCE_DATA> textureSubresources;

		if (ext == L"tga") {
			LoadFromTGAFile(fileName[i].c_str(), &meta[i], scratch[i]);
		}
		else if (ext == L"dds") {
			LoadFromDDSFile(fileName[i].c_str(), DDS_FLAGS_NONE, &meta[i], scratch[i]);
		}
		else {
			LoadFromWICFile(fileName[i].c_str(), WIC_FLAGS_NONE, &meta[i], scratch[i]);
		}



		const DirectX::Image* img = scratch[i].GetImage(0, 0, 0);
		auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		desc[i] = CD3DX12_RESOURCE_DESC::Tex2D(meta[i].format,
			(UINT)meta[i].width,
			(UINT)meta[i].height,
			static_cast<UINT>(meta[i].arraySize),
			static_cast<UINT>(meta[i].mipLevels));

		D3D12_CLEAR_VALUE clearValue{};
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
		clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		Renderer::GetInstance()->GetDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc[i],
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&m_resource[i]));

		m_resource[i]->WriteToSubresource(0,
			nullptr, //全領域へコピー
			img->pixels, //元データアドレス
			static_cast<UINT>(img->rowPitch), //1ラインサイズ
			static_cast<UINT>(img->slicePitch) //全サイズ
		);
	}

	//デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.NumDescriptors = Renderer::Index::RTV_RESOURCE_INDEX_MAX + Renderer::Index::MODEL_TEX_RESOURCE_NUM;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	hr = device->CreateDescriptorHeap(&descriptorHeapDesc,
		IID_PPV_ARGS(&m_descriptorHeap));
	assert(SUCCEEDED(hr));

	//シェーダリソースビュー作成

	D3D12_CPU_DESCRIPTOR_HANDLE handleSrv{};
	handleSrv = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	unsigned int size = Renderer::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (int i = 0; i <Renderer::Index::RTV_RESOURCE_INDEX_MAX+ Renderer::Index::MODEL_TEX_RESOURCE_NUM; i++) {
		D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};

		if (i < Renderer::Index::RTV_RESOURCE_INDEX_MAX) {
			resourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			resourceViewDesc.Texture2D.MipLevels = 1;
			resourceViewDesc.Texture2D.MostDetailedMip = 0;
			resourceViewDesc.Texture2D.PlaneSlice = 0;
			resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				

			device->CreateShaderResourceView(Renderer::GetInstance()->GetResource((Renderer::Index::RtvResourceIndex)i).Get(),
				&resourceViewDesc, handleSrv);
			handleSrv.ptr += size;
		}
		else {
			resourceViewDesc.Format = desc[i- Renderer::Index::RTV_RESOURCE_INDEX_MAX].Format;
			resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			resourceViewDesc.Texture2D.MipLevels = 1;
			resourceViewDesc.Texture2D.MostDetailedMip = 0;
			resourceViewDesc.Texture2D.PlaneSlice = 0;
			resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			resourceViewDesc.Shader4ComponentMapping =
				D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


			device->CreateShaderResourceView(m_resource[i- Renderer::Index::RTV_RESOURCE_INDEX_MAX].Get(),
				&resourceViewDesc, handleSrv);
			handleSrv.ptr += size;
		}
	}

}
void TextureGeometry::Finalize() {


}

