#pragma once
#include"Main.h"
#include<vector>
#include<memory>

class GameObject {
private:
	bool m_isDestroy = false;

protected:
	XMFLOAT3 m_position=XMFLOAT3(0,0,0);
	XMFLOAT3 m_scale=XMFLOAT3(1,1,1);
	XMFLOAT3 m_rotation = XMFLOAT3(0, 0, 0);
	XMFLOAT4X4 m_worldMTX=XMFLOAT4X4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	GameObject* m_parent=nullptr;
	std::vector <std::unique_ptr<GameObject>> m_child;

	bool GetIsDestroy() { return m_isDestroy; }
public:
	GameObject() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

	void SetPosition(XMFLOAT3 position) { m_position = position; }
	void SetScale(XMFLOAT3 scale) { m_scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { m_rotation = rotation; }
	void SetParent(GameObject* parent) { m_parent = parent; }
	void CreateWorldMTX(XMMATRIX& transMTX, XMMATRIX& scaleMTX, XMMATRIX& rotMTX) {
		XMMATRIX world;
		if (m_parent != nullptr) {
			XMFLOAT4X4 parentWorldTemp = m_parent->GetWorldMTX();
			XMMATRIX parentWorld = XMLoadFloat4x4(&parentWorldTemp);
			world = (scaleMTX * rotMTX * transMTX) * parentWorld;
		}
		else {
			world = scaleMTX * rotMTX * transMTX;
		}
		XMStoreFloat4x4(&m_worldMTX, world);
	}

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetWorldPosition() { return XMFLOAT3(m_worldMTX._41, m_worldMTX._42, m_worldMTX._43); }
	XMFLOAT3 GetScale() { return m_scale; }
	XMFLOAT3 GetRotation() { return m_rotation; }
	XMFLOAT4X4 GetWorldMTX() { return m_worldMTX; }
	GameObject* GetParent() { return m_parent; }
	GameObject* GetChild(int i) { return m_child[i].get(); }

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

	template <typename T>
	T* AddChild() {
		m_child.push_back(std::make_unique<T>());
		m_child[m_child.size() - 1].get()->Initialize();
		m_child[m_child.size() - 1].get()->SetParent(this);
		return (T*)m_child[m_child.size() - 1].get();

	}
};