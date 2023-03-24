#pragma once
#include "GameObject.h"

class Arm;
class TestObj :public GameObject {
private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	Arm* m_arm[2];

public:
	TestObj();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};