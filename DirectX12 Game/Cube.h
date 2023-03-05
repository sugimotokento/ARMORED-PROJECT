#pragma once

#include "main.h"



class Cube{
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_constantBuffer;
	

public:
	Cube();
	void Draw();
	void SetColor(XMFLOAT4 color);

	void Finalize();

};