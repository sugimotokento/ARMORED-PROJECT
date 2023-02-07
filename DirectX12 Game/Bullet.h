#pragma once
#include"GameObject.h"

class Cube;
class Bullet : public GameObject {
private:
	Cube* m_cube;
	XMFLOAT3 m_moveDir;
	float m_speed;

public:
	Bullet();
	void Initialize() final override {}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetParametor(XMFLOAT3 position, XMFLOAT3 moveDir, float speed=1);
};