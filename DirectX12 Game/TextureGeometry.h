#pragma once

#include "main.h"

class TextureGeometry {
private:
	ComPtr<ID3D12Resource>			m_resource[5];
	ComPtr<ID3D12DescriptorHeap>	m_descriptorHeap;

	//Šg’£Žq‚ðŽæ“¾‚·‚é
	std::wstring FileExtension(std::wstring path);

public:

	void LoadTexture(std::wstring alpbedPath, 
		std::wstring normalPath = L"asset/Texture/White.png", 
		std::wstring occlusionPath = L"asset/Texture/White.png", 
		std::wstring metalPath = L"asset/Texture/Black.png", 
		std::wstring emmisionPath = L"asset/Texture/Black.png"
	);
	void Finalize();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_descriptorHeap; }

};