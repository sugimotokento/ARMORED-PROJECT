#pragma once
#ifdef _DEBUG
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"


#include<functional>
#include<vector>

class ImguiRenderer {
private:
	static ImguiRenderer* m_instance;
	std::vector <std::function<bool()>> m_function;

public:
	ImguiRenderer() { m_instance = this; }
	void Initiaize();
	void Finalize();
	void Draw();
	void AddFunction(const std::function<bool()>& func);
	static ImguiRenderer* GetInstance() { return m_instance; }
};

#endif // DEBUG