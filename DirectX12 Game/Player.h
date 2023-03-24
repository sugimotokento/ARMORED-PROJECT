#pragma once
#include"Main.h"
#include"GameObject.h"
#include<list>
#include"Arm.h"


class Player :public GameObject {
public:
	const float MOVE_SPEED = 0.15f;
	const int MAX_HP = 5;

private:
	std::unique_ptr<Arm> m_arm[2];
	ComPtr<ID3D12Resource> m_constantBuffer;


	void Move();
	void Shot();
	void Rotation();
	void FieldCollision();

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
public:
	Player();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;


};