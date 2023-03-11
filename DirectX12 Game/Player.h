#pragma once
#include"Main.h"
#include"GameObject.h"
#include<list>
#include"Model.h"
#include"TextureGeometry.h"

class Player :public GameObject {
public:
	const float MOVE_SPEED = 0.15f;
	const int MAX_HP = 5;

private:
	std::unique_ptr<TextureGeometry> m_texture[5];
	std::unique_ptr<Model> m_model[5];
	ComPtr<ID3D12Resource> m_constantBuffer;

	int m_hp;
	bool m_isDie;

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

	void Damage(int damage=1);
	int GetHP();
	bool GetIsDie();

};