#pragma once
#include"GameObject.h"
#include<list>

class Cube;
class Gun;
class Player :public GameObject {
public:
	const float MOVE_SPEED = 0.15f;
	const int MAX_HP = 5;

private:
	Cube* m_cube;
	std::list<Gun*> m_guns;
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
	void Initialize() final override{}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void Damage(int damage=1);
	int GetHP();
	bool GetIsDie();

	template <typename T>
	void AddGun() {
		T* gun = new T();
		gun->SetGameObject(this);
		m_guns.push_back(gun);
	};
};