#ifdef _DEBUG
#pragma once
#include"GameObject.h"
#include<memory>
#include"FieldObject.h"


class StageEditor :public GameObject {
private:
	struct EditParam {
		float objectMoveSpeed = 0.5f;
		float objectRotationSpeed = 0.1f;
		float objectChangeScaleSpeed = 0.99f;
	};

private:
	std::unique_ptr<FieldObject> m_editObject;
	std::vector<std::unique_ptr<FieldObject>> m_objectList;
	XMFLOAT3 m_spownPosition = XMFLOAT3(0, 0, 0);
	bool m_isEditMode = false;
	bool m_isEditObjectMode = false;


	//�I�u�W�F�N�g���X�g����폜����p
	int m_removeIndex = -1;
	bool m_isRemove = false;


	//-----------------------------------
	//�R���g���[���[���͂ŃI�u�W�F�N�g��ҏW����@�\
	//----------------------------------
	void EditObject();
	void EditPosition();
	void EditScale();
	void EditRotation();

	//---------------------
	//ImGui����
	//---------------------
	bool ImguiStageEditor(bool isVisible);	//ImguiRenderer�ɒǉ�����֐�
	void ImguiEditWindow();					//�I�u�W�F�N�g�̐����A�ǉ��A�ҏW�̃I���I�t
	void ImguiObjectListWindow();			//�ݒ�ς݃I�u�W�F�N�g�̕\��

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	bool GetIsEditMode() { return m_isEditMode; }
	bool GetIsEditObjectMode() { return m_isEditObjectMode; }
};

#endif
