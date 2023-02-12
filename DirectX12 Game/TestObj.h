#pragma once
#include "GameObject.h"

class Cube;
class TestObj :public GameObject {
private:
	Cube* cube;
	Cube* cube2;
	Cube* cube3;

public:
	TestObj();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};