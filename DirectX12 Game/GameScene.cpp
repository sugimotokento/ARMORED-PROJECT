#include"Main.h"
#include"GameScene.h"
#include"Player.h"
#include"PlayerUI.h"
#include"SceneManager.h"
#include"Ocean.h"
#include"SkyDome.h"

GameScene::GameScene() {

}
void GameScene::Initialize() {
	Scene::Initialize();
	AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
	AddGameObject<Player>(Layer::GEOMETRY);

	//AddGameObject<TestObj>(Layer::ALPHA);

	CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_PLAYER);
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