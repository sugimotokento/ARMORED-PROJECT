#include"TestObj.h"

#include"Scene.h"
#include"Renderer.h"

TestObj::TestObj() {

}
void TestObj::Initialize() {
	cube = new Cube();
	cube2 = new Cube();
	cube3 = new Cube();
}
void TestObj::Update() {

}
void TestObj::Draw() {
	cube->SetColor(XMFLOAT4(0.8f, 0, 0, 0.5f));
	cube->Draw(XMFLOAT3(0,0.5f,0.5f), XMFLOAT3(0.8f, 0.8f, 0.8f));

	cube2->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
	cube2->Draw(XMFLOAT3(0, 0.5f, 0));

	cube3->SetColor(XMFLOAT4(1.2f, 0.2f, 0.2f, 1));
	cube3->Draw(XMFLOAT3(4, 0.5f, 0));
}
void TestObj::Finalize() {
	delete cube;
	delete cube2;
	delete cube3;
}