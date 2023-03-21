#pragma once
#include"Camera.h"

class DebugCamera :public Camera {
private:
	XMFLOAT3 m_rotation;
	XMFLOAT4 m_quaternion;

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
public:
	DebugCamera();
	void Update()final override;
	void Draw()final override;
};