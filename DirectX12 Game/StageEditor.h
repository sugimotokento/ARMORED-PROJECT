#ifdef _DEBUG
#pragma once
#include"GameObject.h"
#include<memory>

//csv�t�@�C���Ɍ����Ƃ���Stage�ɐݒu����I�u�W�F�N�g�������������񂾂�ǂݍ��񂾂�
class StageEditor :public GameObject {
private:
	std::unique_ptr<GameObject> m_loadObject;

	bool ImguiEditWindow(bool isVisible);

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

};

#endif
