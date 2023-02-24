#pragma once
#include"Renderer.h"
#include<vector>
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
	std::vector<ModelResource> m_moldeResource;
	ComPtr<ID3D12Resource> m_constantBuffer;
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;
	XMFLOAT3 m_rotation;

	unsigned int m_meshNum = 0;
public:
	void LoadMesh(const char* fileName);
	void Draw();
};