#pragma once

#include "main.h"

using namespace Microsoft::WRL;

class PolygonDeferred
{
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_constantBuffer;

public:
	PolygonDeferred();

	void Draw(ID3D12DescriptorHeap* texture);
	void Finalize();
};