#ifdef _DEBUG
#include"SceneManager.h"
#include"GameScene.h"
#include"DebugWeaponViewerScene.h"
#include"ImguiRenderer.h"
#include"Input.h"

#include"Ocean.h"
#include"SkyDome.h"


DebugWeaponViewerScene::DebugWeaponViewerScene() {

}
void DebugWeaponViewerScene::Initialize() {
	AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
}
void DebugWeaponViewerScene::Update() {
	Scene::Update();
}
void DebugWeaponViewerScene::Draw(Layer layer) {
	Scene::Draw(layer);
}
void DebugWeaponViewerScene::Finalize() {
	Scene::Finalize();
}



#endif