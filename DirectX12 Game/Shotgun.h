#pragma once
#include"Weapon.h"
#include"Ammunition.h"
#include<memory>
#include"Main.h"
#include"GameObject.h"
class Shotgun : public Weapon {
private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	Ammunition::SettingItem m_bulletSetting;

	XMFLOAT3 m_bulletOffset;
	float m_spreadRatio = 1;
	const int SHOT_NUM = 15;
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