#pragma once
#include"GameObject.h"

class Cube;
class CubeParticle :public GameObject {
private:
	Cube* m_cube;
	XMFLOAT4 m_color;
	XMFLOAT3 m_moveDir;
	int m_destroyInterval;
	int m_destroyIntervalCount;
public:
	CubeParticle();
	void Initialize() final override {}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetTransform(XMFLOAT3 position = { 0.0f,0.0f,0.0f }, XMFLOAT3 rotation = { 0.0f, 0.0f,0.0f });
	void SetParametor(XMFLOAT3 moveDir = { 0.0f,0.0f,0.0f }, int detroyInterval = 20, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
};