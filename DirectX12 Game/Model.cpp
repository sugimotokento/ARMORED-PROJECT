#include"Model.h"


void Model::LoadMesh(const char* fileName) {

	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	const aiScene* scene = importer.ReadFile(fileName, flag);
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	ComPtr<ID3D12Device> device = Renderer::GetInstance()->GetDevice();
	D3D12_HEAP_PROPERTIES heapProperties{};
	D3D12_RESOURCE_DESC   resourceDesc{};

	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;

	m_meshNum = scene->mNumMeshes;
	for (unsigned int i = 0; i < m_meshNum; i++) {
		aiMesh* mesh = scene->mMeshes[i];


		m_modelResource.push_back(ModelResource());

		//頂点バッファの生成
		{
			m_modelResource[i].m_vertexNum = mesh->mNumVertices;
			Vertex3DBuffer* tempVertex = new Vertex3DBuffer[m_modelResource[i].m_vertexNum];

			for (unsigned int v = 0; v < m_modelResource[i].m_vertexNum; v++) {
				auto uv = (mesh->HasTextureCoords(0)) ? &(mesh->mTextureCoords[0][v]) : &zero3D;

				tempVertex[v].position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				tempVertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				tempVertex[v].diffuse = XMFLOAT4(0.98f, 0.98f, 0.98f, 1.0f);
				tempVertex[v].texCoord = XMFLOAT2(uv->x, 1-uv->y);
			}


			//頂点バッファの作成
			resourceDesc.Width = sizeof(Vertex3DBuffer) * m_modelResource[i].m_vertexNum;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_modelResource[i].m_vertexBuffer));

			Vertex3DBuffer* vertex;
			m_modelResource[i].m_vertexBuffer->Map(0, nullptr, (void**)&vertex);
			memcpy(vertex, tempVertex, sizeof(Vertex3DBuffer) * m_modelResource[i].m_vertexNum);
			m_modelResource[i].m_vertexBuffer->Unmap(0, nullptr);

			delete[] tempVertex;
		}


		//インデックスバッファの生成
		{
			m_modelResource[i].m_indexNum = mesh->mNumFaces;
			unsigned int* tempIndex = new unsigned int[m_modelResource[i].m_indexNum * 3];

			for (unsigned int f = 0; f < m_modelResource[i].m_indexNum; f++) {
				const aiFace* face = &mesh->mFaces[f];

				tempIndex[f * 3 + 0] = face->mIndices[0];
				tempIndex[f * 3 + 1] = face->mIndices[1];
				tempIndex[f * 3 + 2] = face->mIndices[2];
			}

			//インデックスバッファの作成
			resourceDesc.Width = sizeof(unsigned int) * m_modelResource[i].m_indexNum*3;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_modelResource[i].m_IndexBuffer));

			unsigned int* index;
			m_modelResource[i].m_IndexBuffer->Map(0, nullptr, (void**)&index);

			memcpy(index, tempIndex, sizeof(unsigned int) * m_modelResource[i].m_indexNum*3);

			m_modelResource[i].m_IndexBuffer->Unmap(0, nullptr);

			delete[] tempIndex;
		}
	}




	////テクスチャのファイルパスを取得
	//for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
	//	aiString path;

	//	if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

	//		if (path.data[0] == '*') {
	//			m_texFileNames.push_back(path.C_Str());
	//		}
	//	}
	//}

}

void Model::Draw() {



	for (unsigned int i = 0; i < m_meshNum; i++) {
		//頂点バッファ設定
		D3D12_VERTEX_BUFFER_VIEW vertexView{};
		vertexView.BufferLocation = m_modelResource[i].m_vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(Vertex3DBuffer);
		vertexView.SizeInBytes = sizeof(Vertex3DBuffer) * m_modelResource[i].m_vertexNum;
		Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexView);


		//インデックスバッファ設定
		D3D12_INDEX_BUFFER_VIEW indexView{};
		indexView.BufferLocation = m_modelResource[i].m_IndexBuffer->GetGPUVirtualAddress();
		indexView.SizeInBytes = sizeof(unsigned int) * m_modelResource[i].m_vertexNum*3;
		indexView.Format = DXGI_FORMAT_R32_UINT;
		Renderer::GetInstance()->GetCommandList().Get()->IASetIndexBuffer(&indexView);



	

		//トポロジ設定
		Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//描画
		Renderer::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(m_modelResource[i].m_indexNum*3, 1, 0, 0, 0);
	}
}
