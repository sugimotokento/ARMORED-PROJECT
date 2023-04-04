#pragma once
#ifdef _DEBUG
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"


#include<functional>
#include<vector>
#include<string>

//-------------ImguiRendererに関数を登録する方法------------------------//
// クラスにImguiを記述する関数を書く{bool 関数名()}
//	std::function<bool()> f = std::bind(&クラス名::関数名, this);
//	ImguiRenderer::GetInstance()->AddFunction(f);
//これでImguiRendererに関数を登録してImguiを表示できる
//--------------------------------------------------------------------//
class GameObject;
class ImguiRenderer {
private:

private:
	static ImguiRenderer* m_instance;
	std::vector <std::function<bool()>> m_function;
	std::vector<std::string> m_label;
	std::vector<bool> m_isVisible;

	bool isImguiEnd = false;
	bool ImguiObjectView();
	void ImguiViewChild(GameObject* obj);

	std::string GetObjectClassName(GameObject* obj);
public:
	ImguiRenderer() { m_instance = this; }
	void Initiaize();
	void Finalize();
	void Draw();
	void AddFunction(const std::function<bool()>& func, std::string label = "None", bool isDefaultVisible=false);
	static ImguiRenderer* GetInstance() { return m_instance; }
};

#endif // DEBUG