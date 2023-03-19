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
#include"Shotgun.h"
DebugWeaponViewerScene::DebugWeaponViewerScene() {

}
void DebugWeaponViewerScene::Initialize() {
	//AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Shotgun>(Layer::GEOMETRY)->SetPosition({ 0,1,0 });
	AddGameObject<Ocean>(Layer::WATER);
}
void DebugWeaponViewerScene::Update() {
	Scene::Update();

	Shotgun* rifle = GetGameObject< Shotgun>();

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