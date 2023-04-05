#include"WeaponViewer.h"
#include"ImguiRenderer.h"
#include"Input.h"
#include"WeaponID.h"

void WeaponViewer::Initialize() {
	m_weapon = new AssaultRifle();
	m_weapon->Initialize();
	m_weapon->SetPosition({ 0,1,0 });

#ifdef _DEBUG
	std::function<bool(bool isVisible)> f = std::bind(&WeaponViewer::ImguiDebug, this, std::placeholders::_1);
	ImguiRenderer::GetInstance()->AddFunction(f);
#endif // _DEBUG
}
void WeaponViewer::Update() {
	m_weapon->Update();
}
void WeaponViewer::Draw() {
	m_weapon->Draw();
}
void WeaponViewer::Finalize() {
	m_weapon->Finalize();
	delete m_weapon;
}


#ifdef _DEBUG
bool WeaponViewer::ImguiDebug(bool isVisible) {
	if (isVisible) {
		ImGui::Begin("WeaponViewer");

		//­Λ
		{
			if (ImGui::Button("Shot") || Input::GetInstance()->GetKeyPress('W')) {
				m_weapon->Shot();
			}
		}

		//νΜIπ
		{
			static int weaponID = 0;
			const char* weaponName[]{
			#undef WEAPON_ID
			#define WEAPON_ID(name, className)#name,
				WEAPON_ID_TABLE
			};

			ImGui::Combo("SelectWeapon", &weaponID, weaponName, WEAPON_ID_MAX);

			//νΨθΦ¦
			if (ImGui::Button("ReloadWeapon")) {
				m_weapon->SetDestroy();
				m_weapon->Destroy();//±ΜΕdelete΅Δι
				m_weapon = CreateWeapon(static_cast<WeaponID>(weaponID));
				m_weapon->Initialize();
				m_weapon->SetPosition({ 0,1,0 });
			}
		}

		ImGui::End();
	}
	return GetIsDestroy();
}
#endif