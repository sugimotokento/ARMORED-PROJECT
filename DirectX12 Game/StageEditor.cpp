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
	//�ǉ������I�u�W�F�N�g��Update
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Update();
	}

	//�ҏW���̃I�u�W�F�N�g��Update
	if (m_editObject.get()) {
		m_editObject.get()->Update();
	}
}
void StageEditor::Draw() {
	//�ǉ������I�u�W�F�N�g��Draw
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Draw();
	}

	//�ҏW���̃I�u�W�F�N�g��Draw
	if (m_editObject.get()) {
		m_editObject.get()->Draw();
	}
}
void StageEditor::Finalize() {
	//�ǉ������I�u�W�F�N�g��Finalize
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Finalize();
	}

	//�ҏW���̃I�u�W�F�N�g��Finalize
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


	//�G�f�B�b�g���[�h�ɂ���
	{
		ImGui::Checkbox("isEditMode", &m_isEditMode);

		//�G�f�B�b�g���[�h����DebugCamera  Off����������Ƃ��Ɛݒ肵�Ă����J����
		if (m_isEditMode) {
			CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_DEBUG);
		}
		else {
			CameraManager::GetInstance()->SetMainCamera(defaultCameraID);
		}
	}


	if (m_isEditMode) {
		//�I�u�W�F�N�g�𐶐�����
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
				//�����Ő���
				m_editObject.reset(new FieldObject());
				m_editObject.get()->SetModelID(static_cast<FieldObject::Index::FieldModelID>(modelId + 1 + FieldObject::Index::MODEL_ID_START));
				m_editObject.get()->Initialize();
				m_editObject.get()->CreateWorldMTX(m_editObject.get()->GetScale(), m_editObject.get()->GetPosition(), m_editObject.get()->GetRotation());
			}
		}


		//�I�u�W�F�N�g�̒ǉ���K�p����	
		{
			if (m_editObject.get()) {
				if (ImGui::Button("Add")) {
					//�ݒ�ς݃I�u�W�F�N�g���X�g�ɒǉ�����
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