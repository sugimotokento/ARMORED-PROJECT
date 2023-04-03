#pragma once
#ifdef _DEBUG
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"


#include<functional>
#include<vector>

//-------------ImguiRenderer�Ɋ֐���o�^������@------------------------//
// �N���X��Imgui���L�q����֐�������{bool �֐���()}
//	std::function<bool()> f = std::bind(&�N���X��::�֐���, this);
//	ImguiRenderer::GetInstance()->AddFunction(f);
//�����ImguiRenderer�Ɋ֐���o�^����Imgui��\���ł���
//--------------------------------------------------------------------//
class ImguiRenderer {
private:

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