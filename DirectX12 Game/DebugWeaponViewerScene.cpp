#ifdef _DEBUG
#include"SceneManager.h"
#include"GameScene.h"
#include"DebugWeaponViewerScene.h"
#include"ImguiRenderer.h"
#include"Input.h"

#include"Ocean.h"
#include"SkyDome.h"
#include"Ammunition.h"
#include"AssaultRifle.h"

DebugWeaponViewerScene::DebugWeaponViewerScene() {

}
void DebugWeaponViewerScene::Initialize() {
	//AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<AssaultRifle>(Layer::GEOMETRY)->SetPosition({ 0,1,0 });
	AddGameObject<Ocean>(Layer::WATER);
}
void DebugWeaponViewerScene::Update() {
	Scene::Update();

	AssaultRifle* rifle = GetGameObject< AssaultRifle>();

	if (Input::GetInstance()->GetKeyPress('W')) {
		rifle->Shot();
	}
}
void DebugWeaponViewerScene::Draw(Layer layer) {
	Scene::Draw(layer);
}
void DebugWeaponViewerScene::Finalize() {
	Scene::Finalize();
}



#endif