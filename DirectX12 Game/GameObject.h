#pragma once
#include"Main.h"

class GameObject {
private:
	bool m_isDestroy = false;

protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;

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