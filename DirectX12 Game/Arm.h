#pragma once
#include"Main.h"
#include"ModelLoader.h"

class GameObject;
class Arm {
public:
	struct Index {
		enum ArmSide {
			LEFT = ModelLoader::Index::ModelID::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L,
			RIGHT= ModelLoader::Index::ModelID::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R,
			MAX=2
		};
	};
private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	Index::ArmSide m_side;
	GameObject* m_parent;
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;
	XMFLOAT4X4 m_worldMTX;

	
public:
	Arm();
	void Initialize(Index::ArmSide side, GameObject* parent);
	void Update();
	void Draw();
	void Finalize();


};