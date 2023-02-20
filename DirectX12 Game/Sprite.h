#pragma once

#include "main.h"

using namespace Microsoft::WRL;

class Texture;
class Sprite
{
private:
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource> m_constantBuffer;
	Texture* m_texture;

	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
public:
	Sprite() = delete;
	Sprite(std::wstring textureName);

	void Draw();
	void Finalize();

	void SetPosition(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void SetColor(XMFLOAT4 color);
};