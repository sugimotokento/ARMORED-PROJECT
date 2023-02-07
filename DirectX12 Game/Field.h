#pragma once
#include"GameObject.h"

class Cube;
class Field :public GameObject {
public:
	enum Size {
		WIDTH = 20,
		HEIGHT = 20,
	};
private:
	Cube* m_field;
	Cube* m_cubeX[2][WIDTH/2];
	Cube* m_cubeY[2][HEIGHT /2-2];
public:
	Field();
	void Initialize() final override{}
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	Cube* GetCubeX(int i, int j);
	Cube* GetCubeY(int i, int j);
};