#pragma once
#include"GameObject.h"
#include"ModelLoader.h"
#include<memory>

class Building :public GameObject {
public:
	struct Index {
		enum BuildingModelID {
			MODEL_ID_START = ModelLoader::Index::MODEL_ID_FIELD_SKYDOME - 1,
			//フィールドモデルID
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_##name,
			MODEL_FIELD_ID_TABLE
			MODEL_ID_MAX
		};
	};

private:
	Index::BuildingModelID m_modelID = Index::MODEL_ID_BUILDING1;
	ComPtr<ID3D12Resource> m_constantBuffer;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	void SetModelID(Index::BuildingModelID id) { m_modelID = id; }
};