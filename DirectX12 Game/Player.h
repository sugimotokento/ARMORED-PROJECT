#pragma once
#include"Main.h"
#include"GameObject.h"
#include<list>
#include"Arm.h"

class Weapon;
class Afterburner;
class Player :public GameObject {
public:
	struct Index {
		enum Side {
			LEFT,
			RIGHT,
			MAX,
		};
	};

	enum{BOOST_SPEED_MAX=2};
	const float MOVE_SPEED = 0.15f;
	const int MAX_HP = 5;

private:
	std::unique_ptr<Arm> m_arm[Index::Side::MAX];
	Weapon* m_lightWeapon[Index::Side::MAX];
	Afterburner* m_afterburner;
	ComPtr<ID3D12Resource> m_constantBuffer;


	void Move();
	void Shot();
	void Rotation();

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