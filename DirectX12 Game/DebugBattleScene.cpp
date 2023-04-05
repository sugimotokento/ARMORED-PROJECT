#include"DebugBattleScene.h"
#include"CameraManager.h"
#include"Player.h"
#include"Ocean.h"
#include"SkyDome.h"
#include"DebugBattleMenu.h"
#include"Field.h"

DebugBattleScene::DebugBattleScene() {

}
void DebugBattleScene::Initialize() {
	Scene::Initialize();
	AddGameObject<Field>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
	AddGameObject<SkyDome>(Layer::GEOMETRY);

	AddGameObject<Player>(Layer::GEOMETRY);

#ifdef  _DEBUG
	AddGameObject<DebugBattleMenu>(Layer::GEOMETRY);
#endif //  _DEBUG



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


