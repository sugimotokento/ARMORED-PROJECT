#pragma once
#include"Camera.h"

class DebugCamera :public Camera {
private:
	XMFLOAT3 m_rotation;

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
public:
	DebugCamera();
	void Update()final override;
	void Draw()final override;
};