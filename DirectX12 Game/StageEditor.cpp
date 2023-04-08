#include"StageEditor.h"
#include"ImguiRenderer.h"
#include"CameraManager.h"
#include"DebugCamera.h"

void StageEditor::Initialize() {
#ifdef _DEBUG
	std::function<bool(bool isVisible)> f = std::bind(&StageEditor::ImguiStageEditor, this, std::placeholders::_1);
	ImguiRenderer::GetInstance()->AddFunction(f, "StageEditor");
#endif // _DEBUG
}
void StageEditor::Update() {
	//追加したオブジェクトのUpdate
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Update();
	}

	//編集中のオブジェクトのUpdate
	if (m_editObject.get()) {
		m_editObject.get()->Update();
	}
}
void StageEditor::Draw() {
	//追加したオブジェクトのDraw
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Draw();
	}

	//編集中のオブジェクトのDraw
	if (m_editObject.get()) {
		m_editObject.get()->Draw();
	}
}
void StageEditor::Finalize() {
	//追加したオブジェクトのFinalize
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Finalize();
	}

	//編集中のオブジェクトのFinalize
	if (m_editObject.get()) {
		m_editObject.get()->Finalize();
	}
}


bool StageEditor::ImguiStageEditor(bool isVisible) {
	if (isVisible) {
		ImguiEditWindow();
		ImguiSetObjectWindow();
	}

	return GetIsDestroy();
}
void StageEditor::ImguiEditWindow() {
	ImGui::Begin("StageEditor");
	static CameraManager::Index::CameraIndex defaultCameraID = CameraManager::GetInstance()->GetMainCameraIndex();


	//エディットモードにする
	{
		ImGui::Checkbox("isEditMode", &m_isEditMode);

		//エディットモード中はDebugCamera  Offだったらもともと設定していたカメラ
		if (m_isEditMode) {
			CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_DEBUG);
		}
		else {
			CameraManager::GetInstance()->SetMainCamera(defaultCameraID);
		}
	}


	if (m_isEditMode) {
		//オブジェクトを生成する
		static int modelId = -1;
		{
			const char* modelName[]{
				#undef MODEL_FIELD_ID
		#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)#name,
					MODEL_FIELD_ID_TABLE
			};

			int indexNum = FieldObject::Index::MODEL_ID_MAX - FieldObject::Index::MODEL_ID_START;
			ImGui::Combo("SelectWeapon", &modelId, modelName, indexNum - 1);
			if (ImGui::Button("SponModel")) {
				//ここで生成
				m_editObject.reset(new FieldObject());
				m_editObject.get()->SetModelID(static_cast<FieldObject::Index::FieldModelID>(modelId + 1 + FieldObject::Index::MODEL_ID_START));
				m_editObject.get()->Initialize();
				m_editObject.get()->CreateWorldMTX(m_editObject.get()->GetScale(), m_editObject.get()->GetPosition(), m_editObject.get()->GetRotation());
			}
		}


		//オブジェクトの追加を適用する	
		{
			if (m_editObject.get()) {
				if (ImGui::Button("Add")) {
					//設定済みオブジェクトリストに追加する
					m_objectList.push_back(std::move(m_editObject));
					m_editObject.reset(nullptr);
					modelId = -1;
				}
			}
		}
	}
	ImGui::End();

}


void StageEditor::ImguiSetObjectWindow() {
	ImGui::Begin("ObjectList");
	std::string objectIdName[]{
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)#name,
			MODEL_FIELD_ID_TABLE
	};
	
	for (int i = 0; i < m_objectList.size(); i++) {
		int index = m_objectList[i].get()->GetModelID()-FieldObject::Index::MODEL_ID_START;
		std::string name = std::to_string(i) + ":" + objectIdName[index - 1];
		ImGui::Text(name.c_str());
	}

	ImGui::End();
}