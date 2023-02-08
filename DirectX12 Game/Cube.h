#pragma once

#include "main.h"

using namespace Microsoft::WRL;

class Cube{
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_constantBuffer;
	
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;
public:
	Cube();
	void Draw(XMFLOAT3 position = XMFLOAT3(0, 0, 0), XMFLOAT3 scale=XMFLOAT3(1,1,1), XMFLOAT3 rotation=XMFLOAT3(0,0,0), float reflectRate=0);
	void SetColor(XMFLOAT4 color);
	void Finalize();


	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetScale() { return m_scale; }
	XMFLOAT3 GetRotation() { return m_rotation; }

};