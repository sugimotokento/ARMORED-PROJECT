#pragma once
#include"Camera.h"

class PlayerCamera :public Camera {
public:
	void Update()final override;
	void Draw()final override;
};