#include"DebugBattleScene.h"
#include"CameraManager.h"
#include"Player.h"
#include"PlayerUI.h"
#include"Ocean.h"
#include"SkyDome.h"
#include"DebugBattleMenu.h"
#include"Field.h"
#include"StageEditor.h"

DebugBattleScene::DebugBattleScene() {

}
void DebugBattleScene::Initialize() {
	Scene::Initialize();

#ifdef  _DEBUG
	AddGameObject<DebugBattleMenu>(Layer::GEOMETRY);
	AddGameObject<StageEditor>(Layer::GEOMETRY);
#endif //  _DEBUG

	AddGameObject<Field>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
	AddGameObject<SkyDome>(Layer::GEOMETRY);

	AddGameObject<Player>(Layer::GEOMETRY);
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


