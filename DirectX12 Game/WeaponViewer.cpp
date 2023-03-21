#include"WeaponViewer.h"
#include"ImguiRenderer.h"
#include"Input.h"
#include"WeaponID.h"

void WeaponViewer::Initialize() {
	m_weapon = new AssaultRifle();
	m_weapon->Initialize();
	m_weapon->SetPosition({ 0,1,0 });

#ifdef _DEBUG
	std::function<bool()> f = std::bind(&WeaponViewer::ImguiDebug, this);
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
bool WeaponViewer::ImguiDebug() {
	ImGui::Begin("WeaponViewer");

	//”­ŽË
	{
		if (ImGui::Button("Shot") || Input::GetInstance()->GetKeyPress('W')) {
			m_weapon->Shot();
		}
	}
	
	//•Ší‚Ì‘I‘ð
	{
		static int weaponID = 0;
		const char* weaponName[]{
		#undef WEAPON_ID
		#define WEAPON_ID(name, className)#name,
			WEAPON_ID_TABLE
		};

		ImGui::Combo("SelectWeapon", &weaponID, weaponName, WEAPON_ID_MAX);

		//•ŠíØ‚è‘Ö‚¦
		if (ImGui::Button("ReloadWeapon")) {
			m_weapon->SetDestroy();
			m_weapon->Destroy();
			m_weapon = CreateWeapon(static_cast<WeaponID>(weaponID));
			m_weapon->Initialize();
			m_weapon->SetPosition({ 0,1,0 });
		}
	}

	ImGui::End();

	return GetIsDestroy();
}
#endif