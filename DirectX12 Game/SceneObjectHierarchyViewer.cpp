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
/// GameObject�̃N���X����String�^�Ŏ擾����
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
/// �ċA�I�Ɏq�I�u�W�F�N�g��\�����Ă���
/// </summary>
/// <param name="obj"></param>
/// <param name="strspace"></param>
void SceneObjectHierarchyViewer::ImguiViewChild(GameObject* obj) {
	std::string name = GetObjectClassName(obj).c_str();
	name.erase(0, 6);
	std::string str = name;
	if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		//�q�I�u�W�F�N�g�����݂��Ȃ��ꍇ�I��
		if (obj->GetChildCount() <= 0) {
			ImGui::TreePop();
			return;
		}

		//�q�I�u�W�F�N�g��\��
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

		//Scene��Layer�����擾
		const char* layerName[]{
	#undef LAYER_ID
	#define LAYER_ID(name) #name,
			LAYER_ID_TABLE
		};

		//�e�I�u�W�F�N�g�̊K�w��\�����Ă���
		for (int i = 0; i < Scene::Layer::COUNT; i++) {
			if (ImGui::CollapsingHeader(layerName[i], ImGuiTreeNodeFlags_DefaultOpen)) {

				for (GameObject* obj : SceneManager::GetInstance()->GetScene()->GetAllObject(static_cast<Scene::Layer>(i))) {

					//��ԏ�̐e�I�u�W�F�N�g����\��
					std::string name = GetObjectClassName(obj).c_str();
					name.erase(0, 6);
					if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
						//�q�I�u�W�F�N�g��\��
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