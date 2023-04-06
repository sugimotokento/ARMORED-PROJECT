#pragma once
#include"GameObject.h"
#include"FieldObject.h"

class Field :public GameObject {
private:
	enum { BUILDING_MAX = 9 };
	std::unique_ptr<FieldObject> m_building[BUILDING_MAX];

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};