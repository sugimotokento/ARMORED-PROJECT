#include"DebugBattleScene.h"
#include"CameraManager.h"
#include"Player.h"
#include"Ocean.h"
#include"SkyDome.h"


DebugBattleScene::DebugBattleScene() {

}
void DebugBattleScene::Initialize() {
	AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
	AddGameObject<Player>(Layer::GEOMETRY);

	//AddGameObject<TestObj>(Layer::ALPHA);

	CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_PLAYER);
}
void DebugBattleScene::Update() {
	Scene::Update();

}
void DebugBattleScene::Draw(Layer layer) {
	Scene::Draw(layer);
}

void DebugBattleScene::Finalize() {
	Scene::Finalize();
}


