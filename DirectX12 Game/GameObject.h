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
	XMFLOAT4X4 m_worldMTX=XMFLOAT4X4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);//基本的にCreateWorldMTX()から設定する！
	GameObject* m_parent=nullptr;
	std::vector <std::unique_ptr<GameObject>> m_child;

	bool GetIsDestroy() { return m_isDestroy; }
public:
	GameObject() {}
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();


	//--------------------------
	//基本的なパラメーターの設定
	//--------------------------
	void SetPosition(XMFLOAT3 position) { m_position = position; }
	void SetScale(XMFLOAT3 scale)		{ m_scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { m_rotation = rotation; }

	//--------------------------
	//基本的なパラメーターの取得
	//--------------------------
	XMFLOAT3 GetPosition()		{ return m_position; }
	XMFLOAT3 GetScale()			{ return m_scale; }
	XMFLOAT3 GetRotation()		{ return m_rotation; }
	XMFLOAT4X4 GetWorldMTX()	{ return m_worldMTX; }

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
	XMFLOAT3 GetWorldPosition() { return XMFLOAT3(m_worldMTX._41, m_worldMTX._42, m_worldMTX._43); }


	//WorldMatrixを計算する
	void CreateWorldMTX(XMFLOAT3 scale, XMFLOAT3 position, XMFLOAT3 rotation) {
		XMMATRIX world;
		XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
		XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		if (m_parent != nullptr) {
			XMFLOAT4X4 parentWorldTemp = m_parent->GetWorldMTX();
			XMMATRIX parentWorld = XMLoadFloat4x4(&parentWorldTemp);
			world = (size * rot * trans) * parentWorld;
		}
		else {
			world = size * rot * trans;
		}
		XMStoreFloat4x4(&m_worldMTX, world);
	}
	void CreateWorldMTX(XMFLOAT3 scale, XMFLOAT3 position, XMFLOAT4 quaternion) {
		XMVECTOR quat;
		quat = XMLoadFloat4(&quaternion);

		XMMATRIX world;
		XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		XMMATRIX rot = XMMatrixRotationQuaternion(quat);
		XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		if (m_parent != nullptr) {
			XMFLOAT4X4 parentWorldTemp = m_parent->GetWorldMTX();
			XMMATRIX parentWorld = XMLoadFloat4x4(&parentWorldTemp);
			world = (size * rot * trans) * parentWorld;
		}
		else {
			world = size * rot * trans;
		}
		XMStoreFloat4x4(&m_worldMTX, world);
	}

	//-----------------
	//削除関連
	//-----------------
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

	//--------------------
	//親子関係周り
	//--------------------
	//親オブジェクトの設定
	void SetParent(GameObject* parent) { m_parent = parent; }
	//親オブジェクトを取得
	GameObject* GetParent() { return m_parent; }
	//子オブジェクトを取得
	GameObject* GetChild(int i) { return m_child[i].get(); }
	//子オブジェクトの数を取得
	int GetChildCount() { return (int)m_child.size(); }
	//子オブジェクトを追加
	template <typename T>
	T* AddChild() {
		m_child.push_back(std::make_unique<T>());
		m_child[m_child.size() - 1].get()->Initialize();
		m_child[m_child.size() - 1].get()->SetParent(this);
		return (T*)m_child[m_child.size() - 1].get();

	}
};