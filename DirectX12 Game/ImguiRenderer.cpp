#ifdef _DEBUG
#include "ImguiRenderer.h"

#include"Renderer.h"
#include"Main.h"

ImguiRenderer *ImguiRenderer::m_instance = nullptr;

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

	ImGui::Begin("ImguiList");
	for (int i = 0; i < static_cast<int>(m_function.size()); i++) {
		bool temp = m_isVisible[i];
		ImGui::Checkbox(m_label[i].c_str(), &temp);
		m_isVisible[i] = temp;
	}
	ImGui::End();

	for (int i = 0; i < static_cast<int>(m_function.size()); i++) {
		if (m_isVisible[i] == true) {
			if (m_function[i]()) {
				m_function.erase(m_function.begin() + i);
				m_label.erase(m_label.begin() + i);
				m_isVisible.erase(m_isVisible.begin() + i);
			}
		}
	}

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

#endif // _DEBUG
