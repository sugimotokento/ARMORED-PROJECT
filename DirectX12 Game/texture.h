#pragma once

#include "main.h"

class Texture{
private :
	ComPtr<ID3D12Resource>			m_resource;
	ComPtr<ID3D12DescriptorHeap>	m_descriptorHeap;

	//Šg’£Žq‚ðŽæ“¾‚·‚é
	std::wstring FileExtension(std::wstring path);

public:

	void LoadTexture(std::wstring fileName);
	void Finalize();

	ComPtr<ID3D12Resource> GetResource() { return m_resource; };
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap(){ return m_descriptorHeap; }

};