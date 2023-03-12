#ifdef _DEBUG
#include"SceneManager.h"
#include"GameScene.h"
#include"DebugWeaponViewerScene.h"
#include"DebugMenuScene.h"
#include"ImguiRenderer.h"
#include"Input.h"


DebugMenuScene::DebugMenuScene() {
	std::function<bool()> f = std::bind(&DebugMenuScene::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
}
void DebugMenuScene::Initialize() {
	
}
void DebugMenuScene::Update() {
	Scene::Update();
}
void DebugMenuScene::Draw(Layer layer) {
	Scene::Draw(layer);
}
void DebugMenuScene::Finalize() {
	Scene::Finalize();
}


bool DebugMenuScene::ImguiDebug() {
	static bool isEnd = false;

	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(400, 500));
	ImGui::Begin("DebugMenu");

	//メインゲーム開始
	if (ImGui::Button("GameStart")) {
		SceneManager::GetInstance()->SetScene<GameScene>();
		isEnd = true;
	}


	//武器の確認用シーン
	if (ImGui::Button("WeaponViewer")) {
		SceneManager::GetInstance()->SetScene<DebugWeaponViewerScene>();
		isEnd = true;
	}
	ImGui::End();
	return isEnd;
}
#endif