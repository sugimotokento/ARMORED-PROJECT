#pragma once
#include"GameObject.h"
#include"ModelLoader.h"

class Arm :public GameObject{
public:
	struct Index {
		enum ArmModelID {
			LEFT = ModelLoader::Index::ModelID::MODEL_ID_ROBOT_DREADNOUGHT_HAND_L,
			RIGHT= ModelLoader::Index::ModelID::MODEL_ID_ROBOT_DREADNOUGHT_HAND_R,
			MAX=2
		};
	};
private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	Index::ArmModelID m_side;

	
public:
	Arm();
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetModelID(Index::ArmModelID side);

};