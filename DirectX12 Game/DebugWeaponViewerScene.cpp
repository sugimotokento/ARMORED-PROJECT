#ifdef _DEBUG
#include"SceneManager.h"
#include"GameScene.h"
#include"DebugWeaponViewerScene.h"
#include"ImguiRenderer.h"
#include"Input.h"

#include"Ocean.h"
#include"SkyDome.h"
#include"Ammunition.h"

DebugWeaponViewerScene::DebugWeaponViewerScene() {

}
void DebugWeaponViewerScene::Initialize() {
	AddGameObject<SkyDome>(Layer::GEOMETRY);
	AddGameObject<Ocean>(Layer::WATER);
}
void DebugWeaponViewerScene::Update() {
	Scene::Update();

	if (Input::GetInstance()->GetKeyTrigger('W')) {
		Ammunition::SettingItem settingItem = {
			XMFLOAT3(0, 0, 1), 
			XMFLOAT3(0, 1, 0), 
			XMFLOAT3(0, 0, 0),
			5,
			100
		};

		AddGameObject<Ammunition>()->SetSettingItem(settingItem);
	}
}
void DebugWeaponViewerScene::Draw(Layer layer) {
	Scene::Draw(layer);
}
void DebugWeaponViewerScene::Finalize() {
	Scene::Finalize();
}



#endif