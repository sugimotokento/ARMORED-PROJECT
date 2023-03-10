#pragma once

#include "main.h"

class Texture{
private :
	ComPtr<ID3D12Resource>			m_resource;
	ComPtr<ID3D12DescriptorHeap>	m_descriptorHeap;

	//拡張子を取得する
	std::wstring FileExtension(std::wstring path);

public:

	void LoadTexture(std::wstring fileName);
	void Finalize();

	ComPtr<ID3D12Resource> GetResource() { return m_resource; };
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap(){ return m_descriptorHeap; }

};