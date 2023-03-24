#ifdef _DEBUG
#include"SceneManager.h"
#include"GameScene.h"
#include"DebugWeaponViewerScene.h"
#include"DebugMenuScene.h"
#include"DebugBattleScene.h"
#include"ImguiRenderer.h"
#include"Input.h"
#include"CameraManager.h"


DebugMenuScene::DebugMenuScene() {
	
}
void DebugMenuScene::Initialize() {
	std::function<bool()> f = std::bind(&DebugMenuScene::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);

	CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_DEBUG);
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

	//���C���Q�[���J�n
	if (ImGui::Button("GameStart")) {
		SceneManager::GetInstance()->SetScene<GameScene>();
		isEnd = true;
	}

	//�f�o�b�O�p�o�g���V�[��
	if (ImGui::Button("DebugBattle")) {
		SceneManager::GetInstance()->SetScene<DebugBattleScene>();
		isEnd = true;
	}

	//����̊m�F�p�V�[��
	if (ImGui::Button("WeaponViewer")) {
		SceneManager::GetInstance()->SetScene<DebugWeaponViewerScene>();
		isEnd = true;
	}
	ImGui::End();
	return isEnd;
}
#endif