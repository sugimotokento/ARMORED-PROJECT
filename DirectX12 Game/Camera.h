#pragma once
#include"Main.h"

class Camera {
protected:
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_viewMatrix;
protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_target;
	XMFLOAT3 m_up=XMFLOAT3(0,1,0);
public:
	virtual void Update() {};
	virtual void Draw();

	void SetPosition(XMFLOAT3 pos) { m_position = pos; }
	void SetTarget(XMFLOAT3 target) { m_target = target; }

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetTarget() { return m_target; }
	XMMATRIX GetProjectionMatrix() { return m_projectionMatrix; }
	XMMATRIX GetViewMatrix() { return m_viewMatrix; }
};