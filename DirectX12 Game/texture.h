#pragma once

#include "main.h"
using namespace Microsoft::WRL;

class Texture{
public:
	ComPtr<ID3D12Resource>			m_resource;
	ComPtr<ID3D12DescriptorHeap>	m_descriptorHeap;

	void LoadDDSTexture(const wchar_t* fileName);
	void Finalize();

	ComPtr<ID3D12Resource> GetResource() { return m_resource; };
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap(){ return m_descriptorHeap; }

};