#pragma once
#include"Camera.h"

class DebugCamera :public Camera {
public:
	void Update()final override;
	void Draw()final override;
};