#pragma once
#include"GameObject.h"
#include"Building.h"

class Field :public GameObject {
private:
	enum { BUILDING_MAX = 9 };
	std::unique_ptr<Building> m_building[BUILDING_MAX];

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};