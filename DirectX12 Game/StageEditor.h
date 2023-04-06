#ifdef _DEBUG
#pragma once
#include"GameObject.h"
#include<memory>

//csvファイルに建物とかのStageに設置するオブジェクトを情報を書き込んだり読み込んだり
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
