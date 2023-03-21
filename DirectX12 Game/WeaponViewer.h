#pragma once
#include"GameObject.h"


class Weapon;
class WeaponViewer :public GameObject {
private:
	Weapon* m_weapon;

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
};