#ifdef _DEBUG
#include "ImguiRenderer.h"
#include"GameObject.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Renderer.h"
#include"Main.h"
#include"Input.h"

ImguiRenderer* ImguiRenderer::m_instance = nullptr;

void ImguiRenderer::Initiaize() {
	// Setup Dear ImGui context
//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GetWind());

	ImGui_ImplDX12_Init(Renderer::GetInstance()->GetDevice().Get(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM, Renderer::GetInstance()->GetImguiSRVDescriptorHeap().Get(),
		Renderer::GetInstance()->GetImguiSRVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		Renderer::GetInstance()->GetImguiSRVDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void ImguiRenderer::Finalize() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiRenderer::Draw() {
	//ImguiStart
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	//登録したImguiのオン、オフをする
	ImGui::Begin("ImguiList");
	for (int i = 0; i < static_cast<int>(m_function.size()); i++) {
		bool temp = m_isVisible[i];
		ImGui::Checkbox(m_label[i].c_str(), &temp);
		m_isVisible[i] = temp;
	}
	ImGui::End();


	//登録したImguiを表示
	for (int i = 0; i < static_cast<int>(m_function.size()); i++) {
		if (m_isVisible[i] == true) {
			if (m_function[i]()) {
				m_function.erase(m_function.begin() + i);
				m_label.erase(m_label.begin() + i);
				m_isVisible.erase(m_isVisible.begin() + i);
			}
		}
	}
		ImguiObjectView();

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Renderer::GetInstance()->GetGraphicsCommandList().Get());

}

void ImguiRenderer::AddFunction(const std::function<bool()>& func, std::string label, bool isDefaultVisible) {
	m_function.push_back(func);
	m_isVisible.push_back(isDefaultVisible);

	if (label == "None") {
		int num = (int)m_label.size();
		m_label.push_back("None" + std::to_string(num));
	}
	else {
		m_label.push_back(label);
	}
}

std::string ImguiRenderer::GetObjectClassName(GameObject* obj) {
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
void ImguiRenderer::ImguiViewChild(GameObject* obj, std::string& strspace) {
	std::string name = GetObjectClassName(obj).c_str();
	name.erase(0, 6);
	std::string str = name;
	if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

		if (obj->GetChildCount() <= 0) {
			ImGui::TreePop();
			return;
		}

		for (int i = 0; i < obj->GetChildCount(); i++) {
			ImguiViewChild(obj->GetChild(i), strspace);
		}
		ImGui::TreePop();
	}
}

bool ImguiRenderer::ImguiObjectView() {
	ImGui::Begin("ObjectView");

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
						std::string childSpace = "   ";

						ImguiViewChild(obj->GetChild(i), childSpace);
					}
					ImGui::TreePop();
				}
				ImGui::Spacing();
			}
		}
		
	}
	ImGui::End();

	return isImguiEnd;
}

#endif // _DEBUG
