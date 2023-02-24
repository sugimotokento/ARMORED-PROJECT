#pragma once
#include "GameObject.h"

class Cube;
class Model;
class TestObj :public GameObject {
private:
	Cube* cube;
	Cube* cube2;
	Cube* cube3;
	Model* model;
public:
	TestObj();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};