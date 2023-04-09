#pragma once
#include"GameObject.h"
#include"ModelLoader.h"
#include<memory>

//�����I�u�W�F�N�g
class FieldObject :public GameObject {
public:
	struct Index {
		enum FieldModelID {
			MODEL_ID_START = ModelLoader::Index::MODEL_ID_FIELD_SKYDOME - 1,
			//�t�B�[���h���f��ID
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_##name,
			MODEL_FIELD_ID_TABLE
			MODEL_ID_MAX
		};
	};

private:
	Index::FieldModelID m_modelID = Index::MODEL_ID_BUILDING1;
	ComPtr<ID3D12Resource> m_constantBuffer;
	bool m_isDrawLineMode = false;
public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;


	void SetModelID(Index::FieldModelID id);
	Index::FieldModelID GetModelID() { return m_modelID; }

	//���f����D3D_PRIMITIVE_TOPOLOGY_LINESTRIP�ŕ\������
	void SetIsDrawLineMode(bool flag) { m_isDrawLineMode = flag; }
};