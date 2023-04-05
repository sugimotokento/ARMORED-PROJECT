#ifdef _DEBUG
#include"SceneObjectHierarchyViewer.h"
#include"ImguiRenderer.h"
#include"Scene.h"
#include"SceneManager.h"

void SceneObjectHierarchyViewer::Initialize() {
	isImguiEnd = false;
	std::function<bool(bool isVisible)> f = std::bind(&SceneObjectHierarchyViewer::ImguiDebug, this, std::placeholders::_1);
	ImguiRenderer::GetInstance()->AddFunction(f, "ObjectHierarchyViewer");
}
void SceneObjectHierarchyViewer::Update() {

}
void SceneObjectHierarchyViewer::Draw() {

}
void SceneObjectHierarchyViewer::Finalize() {
	isImguiEnd = true;
}


/// <summary>
/// GameObjectのクラス名をString型で取得する
/// </summary>
/// <param name="obj"></param>
/// <returns></returns>
std::string SceneObjectHierarchyViewer::GetObjectClassName(GameObject* obj) {
	const type_info& id = typeid(*obj);
	const char* name = id.name();
	std::string className = name;
	return className;
}

/// <summary>
/// 再帰的に子オブジェクトを表示していく
/// </summary>
/// <param name="obj"></param>
/// <param name="strspace"></param>
void SceneObjectHierarchyViewer::ImguiViewChild(GameObject* obj) {
	std::string name = GetObjectClassName(obj).c_str();
	name.erase(0, 6);
	std::string str = name;
	if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		//子オブジェクトが存在しない場合終了
		if (obj->GetChildCount() <= 0) {
			ImGui::TreePop();
			return;
		}

		//子オブジェクトを表示
		for (int i = 0; i < obj->GetChildCount(); i++) {
			ImguiViewChild(obj->GetChild(i));
		}
		ImGui::TreePop();
	}
}

bool SceneObjectHierarchyViewer::ImguiDebug(bool isVisible) {
	if (isVisible) {
		const type_info& id = typeid(*SceneManager::GetInstance()->GetScene());
		const char* name = id.name();
		std::string className = name;
		className.erase(0, 6);
		className += "ObjectHierarchyViewer";
		ImGui::Begin(className.c_str());

		//SceneのLayer名を取得
		const char* layerName[]{
	#undef LAYER_ID
	#define LAYER_ID(name) #name,
			LAYER_ID_TABLE
		};

		//各オブジェクトの階層を表示していく
		for (int i = 0; i < Scene::Layer::COUNT; i++) {
			if (ImGui::CollapsingHeader(layerName[i], ImGuiTreeNodeFlags_DefaultOpen)) {

				for (GameObject* obj : SceneManager::GetInstance()->GetScene()->GetAllObject(static_cast<Scene::Layer>(i))) {

					//一番上の親オブジェクト名を表示
					std::string name = GetObjectClassName(obj).c_str();
					name.erase(0, 6);
					if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
						//子オブジェクトを表示
						for (int i = 0; i < obj->GetChildCount(); i++) {
							ImguiViewChild(obj->GetChild(i));
						}
						ImGui::TreePop();
					}
					ImGui::Spacing();
				}
			}

		}
		ImGui::End();
	}
	return isImguiEnd;
}


#endif 