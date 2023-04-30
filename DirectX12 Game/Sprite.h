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
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	float m_radialRatio = 1;
	float m_radialStartAngle = 0;
	bool m_isRadialInvers = false;
public:
	Sprite() = delete;
	Sprite(std::wstring textureName);

	void Draw();
	void Finalize();

	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	void SetColor(XMFLOAT4 color);
	void SetRadialParam(float ratio, float startAngle = 0, bool isInvers = false) {
		m_radialRatio = ratio;
		m_radialStartAngle = startAngle;
		m_isRadialInvers = isInvers;
	}
};