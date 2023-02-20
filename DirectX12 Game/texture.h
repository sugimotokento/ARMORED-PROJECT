#pragma once

#include "main.h"

class Texture{
public:
	ComPtr<ID3D12Resource>			m_resource;
	ComPtr<ID3D12DescriptorHeap>	m_descriptorHeap;

	std::wstring FileExtension(std::wstring path);
	void LoadDDSTexture(std::wstring fileName);
	void Finalize();

	ComPtr<ID3D12Resource> GetResource() { return m_resource; };
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap(){ return m_descriptorHeap; }

};