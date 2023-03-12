#include"Main.h"
#include"GameScene.h"
#include"Player.h"
#include"PlayerUI.h"
#include"SceneManager.h"
#include"TestObj.h"
#include"Ocean.h"
#include"SkyDome.h"

GameScene::GameScene() {

}
void GameScene::Initialize() {
	AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
	AddGameObject<Player>(Layer::GEOMETRY)->SetPosition(XMFLOAT3(0, 0, -5));

	//AddGameObject<TestObj>(Layer::ALPHA);
}
void GameScene::Update() {
	Scene::Update();
	ChangeScene();
}
void GameScene::Draw(Layer layer) {
	Scene::Draw(layer);
}

void GameScene::Finalize() {
	Scene::Finalize();
}


void GameScene::ChangeScene() {

}