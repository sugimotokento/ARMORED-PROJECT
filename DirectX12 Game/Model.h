#pragma once
#include"Renderer.h"
#include<vector>
#include<list>
#include<string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include"assimp/Importer.hpp"
#pragma comment (lib,"assimp-vc142-mtd.lib")

struct ModelResource{
	ComPtr<ID3D12Resource> m_vertexBuffer;
	ComPtr<ID3D12Resource>	m_IndexBuffer;

	unsigned int	m_vertexNum;
	unsigned int	m_indexNum;
};

class Model {
private:
	std::vector<ModelResource> m_modelResource;


	XMFLOAT3 m_position;
	XMFLOAT3 m_scale=XMFLOAT3(1,1,1);
	XMFLOAT3 m_rotation;

	unsigned int m_meshNum = 0;
public:
	void LoadMesh(const char* fileName);
	void Draw(D3D_PRIMITIVE_TOPOLOGY primitiveTopology= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Finalize();

	XMFLOAT3 GetPosition() { return m_position; }
	XMFLOAT3 GetScale() { return m_scale; }
	XMFLOAT3 GetRotation() { return m_rotation; }

	void SetPosition(XMFLOAT3 position) { m_position = position; }
	void SetScale(XMFLOAT3 scale) { m_scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { m_rotation = rotation; }

};