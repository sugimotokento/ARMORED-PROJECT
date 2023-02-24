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


		m_moldeResource.push_back(ModelResource());

		//頂点バッファの生成
		{
			m_moldeResource[i].m_vertexNum = mesh->mNumVertices;
			Vertex3DBuffer* tempVertex = new Vertex3DBuffer[m_moldeResource[i].m_vertexNum];

			for (unsigned int v = 0; v < m_moldeResource[i].m_vertexNum; v++) {
				tempVertex[v].position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				tempVertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				tempVertex[v].diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1);
				tempVertex[v].texCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
			}


			//頂点バッファの作成
			resourceDesc.Width = sizeof(Vertex3DBuffer) * m_moldeResource[i].m_vertexNum;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_moldeResource[i].m_vertexBuffer));

			Vertex3DBuffer* vertex;
			m_moldeResource[i].m_vertexBuffer->Map(0, nullptr, (void**)&vertex);
			memcpy(vertex, tempVertex, sizeof(Vertex3DBuffer) * m_moldeResource[i].m_vertexNum);
			m_moldeResource[i].m_vertexBuffer->Unmap(0, nullptr);

			delete[] tempVertex;
		}


		//インデックスバッファの生成
		{
			m_moldeResource[i].m_indexNum = mesh->mNumFaces;
			unsigned int* tempIndex = new unsigned int[m_moldeResource[i].m_indexNum * 3];

			for (unsigned int f = 0; f < m_moldeResource[i].m_indexNum; f++) {
				const aiFace* face = &mesh->mFaces[f];

				tempIndex[f * 3 + 0] = face->mIndices[0];
				tempIndex[f * 3 + 1] = face->mIndices[1];
				tempIndex[f * 3 + 2] = face->mIndices[2];
			}

			//インデックスバッファの作成
			resourceDesc.Width = sizeof(unsigned int) * m_moldeResource[i].m_indexNum*3;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_moldeResource[i].m_IndexBuffer));

			unsigned int* index;
			m_moldeResource[i].m_IndexBuffer->Map(0, nullptr, (void**)&index);

			memcpy(index, tempIndex, sizeof(unsigned int) * m_moldeResource[i].m_indexNum*3);

			m_moldeResource[i].m_IndexBuffer->Unmap(0, nullptr);

			delete[] tempIndex;
		}
	}

	//定数バッファの作成
	resourceDesc.Width = 256;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_constantBuffer));


	m_scale = XMFLOAT3(0.01f, 0.01f, 0.01f);
	m_position = XMFLOAT3(2, 0.01f, 0.01f);
}

void Model::Draw() {
	m_rotation.y += 0.01f;

	////マトリクス設定
	XMMATRIX lightView = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_SHADOW);
	XMMATRIX lightProjection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_SHADOW);

	XMMATRIX view = Camera::GetInstance()->GetViewMatrix(Camera::Index::CAMERA_MAIN);
	XMMATRIX projection = Camera::GetInstance()->GetProjectionMatrix(Camera::Index::CAMERA_MAIN);

	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	XMMATRIX size = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX world = size * rot * trans;

	//定数バッファ設定
	ConstantBuffer* constant;
	m_constantBuffer->Map(0, nullptr, (void**)&constant);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->wvp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * lightView * lightProjection));
	constant->wvpLight = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(view * projection));
	constant->vp = matrix;

	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->world = matrix;

	constant->reflectRate = XMFLOAT4(0,0,0,0);

	constant->isWater = false;

	m_constantBuffer->Unmap(0, nullptr);

	Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(0,
		m_constantBuffer->GetGPUVirtualAddress());

	for (unsigned int i = 0; i < m_meshNum; i++) {
		//頂点バッファ設定
		D3D12_VERTEX_BUFFER_VIEW vertexView{};
		vertexView.BufferLocation = m_moldeResource[i].m_vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(Vertex3DBuffer);
		vertexView.SizeInBytes = sizeof(Vertex3DBuffer) * m_moldeResource[i].m_vertexNum;
		Renderer::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexView);


		//インデックスバッファ設定
		D3D12_INDEX_BUFFER_VIEW indexView{};
		indexView.BufferLocation = m_moldeResource[i].m_IndexBuffer->GetGPUVirtualAddress();
		indexView.SizeInBytes = sizeof(unsigned int) * m_moldeResource[i].m_vertexNum*3;
		indexView.Format = DXGI_FORMAT_R32_UINT;
		Renderer::GetInstance()->GetCommandList().Get()->IASetIndexBuffer(&indexView);



		//テクスチャ設定
		ID3D12DescriptorHeap* dh[] = { Renderer::GetInstance()->GetSRVDescriptorHeap().Get() };
		Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(_countof(dh), dh);
		Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1,
			Renderer::GetInstance()->GetSRVDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart()
		);


		//トポロジ設定
		Renderer::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//描画
		Renderer::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(m_moldeResource[i].m_indexNum*3, 1, 0, 0, 0);
	}
}