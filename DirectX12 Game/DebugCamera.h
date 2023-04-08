#pragma once
#include"Camera.h"

class DebugCamera :public Camera {
private:
	XMFLOAT3 m_rotation;
	XMFLOAT4 m_quaternion;
	XMFLOAT4X4 m_worldMTX;

#ifdef _DEBUG
	bool ImguiDebug(bool isVisible);
#endif // _DEBUG
public:
	DebugCamera();
	void Update()final override;
	void Draw()final override;

	XMFLOAT3 GetRight() {
		XMFLOAT3 out;
		out.x = m_worldMTX._11;
		out.y = m_worldMTX._12;
		out.z = m_worldMTX._13;

		XMVECTOR dir;
		dir = XMLoadFloat3(&out);
		dir = XMVector3Normalize(dir);
		XMStoreFloat3(&out, dir);

		return out;
	}
	XMFLOAT3 GetUp() {
		XMFLOAT3 out;
		out.x = m_worldMTX._21;
		out.y = m_worldMTX._22;
		out.z = m_worldMTX._23;

		XMVECTOR dir;
		dir = XMLoadFloat3(&out);
		dir = XMVector3Normalize(dir);
		XMStoreFloat3(&out, dir);

		return out;
	}
	XMFLOAT3 GetForward() {
		XMFLOAT3 out;
		out.x = m_worldMTX._31;
		out.y = m_worldMTX._32;
		out.z = m_worldMTX._33;

		XMVECTOR dir;
		dir = XMLoadFloat3(&out);
		dir = XMVector3Normalize(dir);
		XMStoreFloat3(&out, dir);

		return out;
	}
};