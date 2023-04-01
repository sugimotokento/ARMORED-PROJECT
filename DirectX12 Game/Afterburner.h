#pragma once
#include"GameObject.h"

class  Afterburner :public GameObject {
private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	GameObject* m_parent;
	float m_boosterRatio = 1;

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetParent(GameObject* parent) { m_parent = parent; }
	void SetBoosterRatio(float ratio) { m_boosterRatio = ratio; }
};