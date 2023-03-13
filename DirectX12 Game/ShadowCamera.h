#pragma once
#pragma once
#include"Camera.h"

class ShadowCamera :public Camera {
public:
	void Update()final override;
	void Draw()final override;
};