#ifdef _DEBUG
#include"DebugBattleMenu.h"
#include"CameraManager.h"
#include"ImguiRenderer.h"

void DebugBattleMenu::Initialize() {
	std::function<bool()> f = std::bind(&DebugBattleMenu::ImguiDebug, this);
	ImguiRenderer::GetInstance()->AddFunction(f);
}

void DebugBattleMenu::Update() {

}

void DebugBattleMenu::Draw() {

}

void DebugBattleMenu::Finalize() {

}

bool DebugBattleMenu::ImguiDebug() {
	ImGui::Begin("DebugBattleMenu");

	if (ImGui::CollapsingHeader("CameraSetting"), ImGuiTreeNodeFlags_DefaultOpen) {

		////////////////////////////////////////
		//�g�p����J������؂�ւ���
		////////////////////////////////////////
		if(ImGui::TreeNodeEx("ChangeCamera", ImGuiTreeNodeFlags_DefaultOpen)) {

			//�ύX�������J������ݒ肷��
			static int cameraID = static_cast<int>(CameraManager::Index::CAMERA_DEBUG);
			const char* cameraName[] = {
			#undef CAMERA_ID
			#define CAMERA_ID(name, className)#name,
				CAMERA_ID_TABLE

			#undef DEBUG_CAMERA_ID
			#define DEBUG_CAMERA_ID(name, className)#name,
				DEBUG_CAMERA_ID_TABLE
			};

			ImGui::Combo("SelectCamera", &cameraID, cameraName, CameraManager::Index::CAMERA_MAX);

			//���Őݒ肵���J�����ɐ؂�ւ���
			if (ImGui::Button("ChangeCamera")) {
				if (cameraID != CameraManager::Index::CAMERA_SHADOW) {
					CameraManager::GetInstance()->SetMainCamera(static_cast<CameraManager::Index::CameraIndex>(cameraID));
				}
			}
			ImGui::TreePop();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		////////////////////////////////////////
		//�J�����̍��W���v���C���[�̈ʒu�ɐݒ肷��
		////////////////////////////////////////

		if (ImGui::TreeNodeEx("SetPosition", ImGuiTreeNodeFlags_DefaultOpen)) {

			//���W���m�F�������J������ݒ肷��
			{
				static int cameraID = static_cast<int>(CameraManager::Index::CAMERA_PLAYER);
				const char* cameraName[] = {
				#undef CAMERA_ID
				#define CAMERA_ID(name, className)#name,
					CAMERA_ID_TABLE

				#undef DEBUG_CAMERA_ID
				#define DEBUG_CAMERA_ID(name, className)#name,
					DEBUG_CAMERA_ID_TABLE
				};

				ImGui::Combo("CheckPositionCamera", &cameraID, cameraName, CameraManager::Index::CAMERA_MAX);

				XMFLOAT3 pos = CameraManager::GetInstance()->GetPosition(static_cast<CameraManager::Index::CameraIndex>(cameraID));
				ImGui::Text("PositionView");
				ImGui::Text("x:%lf, y:%lf, z:%lf", pos.x, pos.y, pos.z);

			}
			ImGui::Spacing();
			//�J�����̍��W��ݒ肷��
			{
				static float pos[3] = { 0,0,0 };
				ImGui::InputFloat3("SetPos", pos);

				if (ImGui::Button("SetPosition")) {
					CameraManager::GetInstance()->SetMainCameraPosition(XMFLOAT3(pos[0], pos[1], pos[2]));
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
	return GetIsDestroy();
}

#endif