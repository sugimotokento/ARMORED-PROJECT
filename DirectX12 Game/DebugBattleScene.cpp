#include"DebugBattleScene.h"
#include"CameraManager.h"
#include"Player.h"

DebugBattleScene::DebugBattleScene() {

}
void DebugBattleScene::Initialize() {
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


