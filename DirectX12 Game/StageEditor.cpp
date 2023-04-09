#ifdef _DEBUG
#include"StageEditor.h"
#include"ImguiRenderer.h"
#include"CameraManager.h"
#include"DebugCamera.h"
#include"Input.h"
#include"XMMath.h"
#include<math.h>

void StageEditor::Initialize() {
	m_editObject.reset(nullptr);
	std::function<bool(bool isVisible)> f = std::bind(&StageEditor::ImguiStageEditor, this, std::placeholders::_1);
	ImguiRenderer::GetInstance()->AddFunction(f, "StageEditor");

}
void StageEditor::Update() {
	//�ǉ������I�u�W�F�N�g��Update
	for (int i = 0; i < m_objectList.size(); i++) {
		m_objectList[i].get()->Update();
	}

	//�ҏW���̃I�u�W�F�N�g��Update
	if (m_editObject.get()) {
		m_editObject.get()->Update();
		EditObject();
	}


	//�T�u�W�F�N�g�폜
	if (m_isRemove == true) {
		if (m_objectList.size() > m_removeIndex) {
			m_objectList[m_removeIndex].get()->Finalize();
			m_objectList.erase(m_objectList.begin() + m_removeIndex);
		}
		m_isRemove = false;
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




//-----------------------------------
//�R���g���[���[���͂ŃI�u�W�F�N�g��ҏW����@�\
//----------------------------------
void StageEditor::EditObject() {
	if (m_editObject.get() == nullptr)return;
	if (m_isEditObjectMode == false)return;

	EditPosition();
	EditScale();
	EditRotation();
}
void StageEditor::EditPosition() {
	DebugCamera* camera = dynamic_cast<DebugCamera*>(CameraManager::GetInstance()->GetCamera(CameraManager::Index::CAMERA_DEBUG));
	XMFLOAT2 inputLeftThumb = XInput::GetInstance()->GetLeftThumb();

	//�ҏW�I�u�W�F�N�g�𓮂���
	XMFLOAT3 forward = camera->GetForward();
	XMFLOAT3 right = camera->GetRight();
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);

	//���s�ړ����������̂�up�ȊO�̂�����0�ɂ���
	forward.y = 0;
	right.y = 0;
	forward = XMMath::Normalize(forward);
	right = XMMath::Normalize(right);

	//�J�������猩�������ړ�
	if (fabsf(inputLeftThumb.y) > 0.01f) {
		XMFLOAT3 pos = m_editObject.get()->GetPosition();
		pos += forward * inputLeftThumb.y * 0.4f;
		m_editObject.get()->SetPosition(pos);
	}

	//�J�������猩�������ړ�
	if (fabsf(inputLeftThumb.x) > 0.01f) {
		XMFLOAT3 pos = m_editObject.get()->GetPosition();
		pos += right * inputLeftThumb.x * 0.4f;
		m_editObject.get()->SetPosition(pos);
	}

	//world���W�̂����ړ�
	if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_DPAD_UP)) {
		XMFLOAT3 pos = m_editObject.get()->GetPosition();
		pos += up * 0.4f;
		m_editObject.get()->SetPosition(pos);
	}
	else if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_DPAD_DOWN)) {
		XMFLOAT3 pos = m_editObject.get()->GetPosition();
		pos += -up * 0.4f;
		m_editObject.get()->SetPosition(pos);
	}
}
void StageEditor::EditScale() {
	if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		XMFLOAT3 scale = m_editObject.get()->GetScale();
		scale *= 1.01f;
		m_editObject.get()->SetScale(scale);
	}
	else if (XInput::GetInstance()->GetPadPress(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		XMFLOAT3 scale = m_editObject.get()->GetScale();
		scale *= 0.99f;
		m_editObject.get()->SetScale(scale);
	}

}
void StageEditor::EditRotation() {
	XMFLOAT2 inputRightThumb = XInput::GetInstance()->GetRightThumb();

	if (fabsf(inputRightThumb.y) > 0.01f) {
		XMFLOAT3 rotation = m_editObject.get()->GetRotation();
		rotation.x += inputRightThumb.y*0.05f;
		m_editObject.get()->SetRotation(rotation);
	}
	if (fabsf(inputRightThumb.x) > 0.01f) {
		XMFLOAT3 rotation = m_editObject.get()->GetRotation();
		rotation.y += inputRightThumb.x*0.05f;
		m_editObject.get()->SetRotation(rotation);
	}

}

//---------------------
//ImGui����
//---------------------
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


	//���[�h�ύX
	{
		ImGui::Checkbox("isEditMode", &m_isEditMode);

		//�G�f�B�b�g���[�h����DebugCamera  Off����������Ƃ��Ɛݒ肵�Ă����J����
		if (m_isEditMode) {
			CameraManager::GetInstance()->SetMainCamera(CameraManager::Index::CAMERA_DEBUG);
		}
		else {
			CameraManager::GetInstance()->SetMainCamera(defaultCameraID);
		}

		if (m_editObject.get()) {
			ImGui::SameLine();
			ImGui::Checkbox("isEditObjectMode", &m_isEditObjectMode);
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
			ImGui::Combo("SelectObject", &modelId, modelName, indexNum - 1);
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
					m_isEditObjectMode = false;
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

	if (ImGui::CollapsingHeader("RemoveObject", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::InputInt("RemoveIndex", &m_removeIndex);//�폜����I�u�W�F�N�g��I������

		//�폜
		if (ImGui::Button("Remove")) {
			m_isRemove = true;
		}
	}

	//�ǉ����ꂽ�I�u�R���g��\��
	if (ImGui::CollapsingHeader("List", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int i = 0; i < m_objectList.size(); i++) {
			int index = m_objectList[i].get()->GetModelID() - FieldObject::Index::MODEL_ID_START;
			std::string name = std::to_string(i) + ":" + objectIdName[index - 1];
			ImVec4 color(1, 1, 1, 1);
			if (i == m_removeIndex)color = ImVec4(1, 0, 0, 1);
			ImGui::TextColored(color, name.c_str());
		}
	}

	ImGui::End();
}

#endif // _DEBUG