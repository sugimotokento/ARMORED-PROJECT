#pragma once
#include"GameObject.h"

class SceneObjectHierarchyViewer :public GameObject {
private:
	bool ImguiDebug();
	void ImguiViewChild(GameObject* obj);

	std::string GetObjectClassName(GameObject* obj);
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
};