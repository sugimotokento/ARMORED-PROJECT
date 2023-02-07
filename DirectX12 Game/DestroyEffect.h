#pragma once
#include"GameObject.h"

class DestroyEffect : public GameObject {
public:
	DestroyEffect();
	void Initialize() final override {}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void AddParticle(XMFLOAT3 position = { 0,0,0 }, XMFLOAT4 color = { 1,1,1,1 });
};