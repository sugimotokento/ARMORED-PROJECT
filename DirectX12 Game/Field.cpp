#include"Field.h"
#include"Input.h"
#include"Cube.h"

Field::Field() {
	m_position = XMFLOAT3(0, -0.05f, 0);
	m_scale = XMFLOAT3(WIDTH, 0.1f, HEIGHT);
	m_rotation = XMFLOAT3(0, 0, 0);

	m_field = new Cube();

	//フィールドの端に並べるキューブ
	for (int i = 0; i < 10; i++) {
		m_cubeX[0][i] = new Cube();

		m_cubeX[1][i] = new Cube();
	}
	for (int i = 0; i < 10 - 2; i++) {
		m_cubeY[0][i] = new Cube();

		m_cubeY[1][i] = new Cube();
	}


	m_field->SetColor(XMFLOAT4(0.7f, 0.7f, 0.7f, 1));
}
void Field::Update() {

}
void Field::Draw() {
	m_field->Draw(m_position, m_scale, m_rotation);

	for (int x = 0; x < 10; x++) {
		XMFLOAT3 scale = XMFLOAT3(1.7f, 1.7f, 1.7f);
		XMFLOAT3 position = XMFLOAT3(-WIDTH / 2 + x * 2 + scale.x / 2, scale.y / 2, HEIGHT / 2 - scale.z / 2);
		m_cubeX[0][x]->SetColor(XMFLOAT4(1, 0, 0, 1));
		m_cubeX[0][x]->Draw(position, scale, XMFLOAT3(0,0,0), XMFLOAT4(0,0,0,0), 0.5f);

		position.z = -HEIGHT / 2 + scale.z / 2;
		m_cubeX[1][x]->SetColor(XMFLOAT4(0, 1, 0, 1));
		m_cubeX[1][x]->Draw(position, scale, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0), 0.5f);
	}

	for (int y = 1; y < 10 - 1; y++) {
		XMFLOAT3 scale = XMFLOAT3(1.7f, 1.7f, 1.7f);
		XMFLOAT3 position = XMFLOAT3(WIDTH / 2 - scale.x / 2, scale.y / 2, -HEIGHT / 2 + y * 2+scale.z/2);
		m_cubeY[0][y-1]->SetColor(XMFLOAT4(0, 1, 1, 1));
		m_cubeY[0][y-1]->Draw(position, scale, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0), 0.5f);

		position.x = -HEIGHT / 2 + scale.x / 2;
		m_cubeY[1][y - 1]->SetColor(XMFLOAT4(0, 0, 1, 1));
		m_cubeY[1][y-1]->Draw(position, scale, XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0), 0.5f);
	}
}
void Field::Finalize() {
	m_field->Finalize();
	delete m_field;


	for (int i = 0; i < 10; i++) {
		m_cubeX[0][i]->Finalize();
		delete m_cubeX[0][i];

		m_cubeX[1][i]->Finalize();
		delete m_cubeX[1][i];
	}
	for (int i = 0; i < 10 - 2; i++) {
		m_cubeY[0][i]->Finalize();
		delete m_cubeY[0][i];

		m_cubeY[1][i]->Finalize();
		delete m_cubeY[1][i];
	}
}


Cube* Field::GetCubeX(int i, int j) {
	return m_cubeX[i][j];
}
Cube* Field::GetCubeY(int i, int j) {
	return m_cubeY[i][j];
}