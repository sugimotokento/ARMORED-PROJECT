#ifdef _DEBUG
#pragma once
#include"GameObject.h"
#include<memory>
#include"FieldObject.h"

//csv�t�@�C���Ɍ����Ƃ���Stage�ɐݒu����I�u�W�F�N�g�������������񂾂�ǂݍ��񂾂�
class StageEditor :public GameObject {
	struct EditData {
		XMFLOAT3 position = XMFLOAT3(0, 0, 0);
		XMFLOAT3 scale = XMFLOAT3(1, 1, 1);
	};
private:
	std::unique_ptr<FieldObject> m_editObject;
	std::vector<std::unique_ptr<FieldObject>> m_objectList;
	EditData editData;
	bool m_isEditMode = false;

	bool ImguiStageEditor(bool isVisible);
	void ImguiEditWindow();
	void ImguiSetObjectWindow();
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	bool GetIsEditMode() { return m_isEditMode; }
};

#endif
