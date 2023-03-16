#pragma once
#include"Weapon.h"
#include"TextureGeometry.h"
#include"Model.h"
#include"Ammunition.h"
#include<memory>
#include"Main.h"
#include"GameObject.h"
class AssaultRifle : public Weapon {
private:
	std::unique_ptr<Model> m_model;
	std::unique_ptr<TextureGeometry> m_texture;
	ComPtr<ID3D12Resource> m_constantBuffer;
	Ammunition::SettingItem m_bulletSetting;

	XMFLOAT3 m_bulletOffset;

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;
	void Shot()final override;

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
};