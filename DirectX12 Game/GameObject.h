#pragma once
#include"Main.h"

class GameObject {
private:
	bool m_isDestroy = false;

protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;
	XMFLOAT4X4 m_worldMTX;

	bool GetIsDestroy() { return m_isDestroy; }
public:
	GameObject() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

	void SetPosition(XMFLOAT3 position) { m_position = position; }
	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetScale() { return m_scale; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT4X4 GetWorldMTX() { return m_worldMTX; }

	XMFLOAT3 GetRight() {
		XMFLOAT3 out;
		out.x = m_worldMTX._11;
		out.y = m_worldMTX._12;
		out.z = m_worldMTX._13;

		XMVECTOR dir;
		dir=XMLoadFloat3(&out);
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

	void SetDestroy() { m_isDestroy = true; }
	bool Destroy() { 
		if (m_isDestroy) {
			Finalize();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}
};