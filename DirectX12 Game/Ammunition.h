#pragma once
#include"GameObject.h"
#include"Model.h"
#include"TextureGeometry.h"
#include<memory>

class Ammunition : public GameObject {
public:
	struct SettingItem {
		XMFLOAT3 moveDir = XMFLOAT3(0, 0, 1);
		XMFLOAT3 position = XMFLOAT3(0, 0, 0);
		XMFLOAT3 rotation = XMFLOAT3(0, 0, 0);
		float speed = 1;
		float range = 100;
	};

private:
	std::unique_ptr<TextureGeometry> m_texture;
	std::unique_ptr<Model> m_model;	
	ComPtr<ID3D12Resource> m_constantBuffer;

	XMFLOAT3 m_startPosition;
	XMFLOAT3 m_moveDir;
	float m_speed = 1;
	float m_range = 100;

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetSettingItem(SettingItem item) {
		m_moveDir = item.moveDir;
		m_position = item.position;
		m_rotation = item.rotation;
		m_speed = item.speed;
		m_range = item.range;

		m_startPosition = item.position;
	}

#ifdef _DEBUG
	bool ImguiDebug();
#endif // _DEBUG
};