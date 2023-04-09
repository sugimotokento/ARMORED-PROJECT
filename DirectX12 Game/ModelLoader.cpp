#include"ModelLoader.h"
#include"Renderer.h"

ModelLoader* ModelLoader::m_instance = nullptr;

void ModelLoader::LoadRequest(Index::ModelID id) {

	//ロード済みモデルだったら終了
	if (CheckLoadedModel(id))return;

	//メッシュとテクスチャをロードする
	m_loadModel[id].model = std::make_unique<Model>();
	m_loadModel[id].texture = std::make_unique<TextureGeometry>();
	m_loadModel[id].model.get()->LoadMesh(Index::GetModelFileName(id).c_str());
	m_loadModel[id].texture.get()->LoadTexture(
		Index::GetTextureFileName(id, Index::TEXTURE_ALBEDO),
		Index::GetTextureFileName(id, Index::TEXTURE_NORMAL),
		Index::GetTextureFileName(id, Index::TEXTURE_OCCLUSION),
		Index::GetTextureFileName(id, Index::TEXTURE_METAL),
		Index::GetTextureFileName(id, Index::TEXTURE_EMMISION)
	);
	m_loadModel[id].id = id;
}
void ModelLoader::UnloadRequest(Index::ModelID id) {
	if (CheckLoadedModel(id)) {
		m_loadModel[id].model.get()->Finalize();
		m_loadModel[id].texture.get()->Finalize();
		m_loadModel.erase(id);
	}
}
void ModelLoader::Draw(Index::ModelID id, D3D_PRIMITIVE_TOPOLOGY primitiveTopology) {
	if (CheckLoadedModel(id)) {
		//テクスチャ設定
		ID3D12DescriptorHeap* dh[] = {m_loadModel[id].texture.get()->GetDescriptorHeap().Get()};
		Renderer::GetInstance()->GetCommandList().Get()->SetDescriptorHeaps(1, dh);
		Renderer::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(1,
			dh[0]->GetGPUDescriptorHandleForHeapStart()
		);

		m_loadModel[id].model.get()->Draw(primitiveTopology);
	}
}
void ModelLoader::Finalize() {
	for (int i = 0; i < Index::ModelID::MODEL_MAX; i++) {
		Index::ModelID id = static_cast<Index::ModelID>(i);
		if (CheckLoadedModel(id)) {
			m_loadModel[id].model.get()->Finalize();
			m_loadModel[id].texture.get()->Finalize();
		}
	}
}


//ロード済み:true
//ロードしてない:false
bool ModelLoader::CheckLoadedModel(Index::ModelID id) {
	if (m_loadModel.find(id) == m_loadModel.end()) {
		return false;
	}
	else {
		return true;
	}
}


void ModelLoader::Create() {
	if (!m_instance) {
		m_instance = new ModelLoader();
	}
}
void ModelLoader::Destroy() {
	delete m_instance;
	m_instance = nullptr;
}
ModelLoader* ModelLoader::GetInstance() {
	return m_instance;
}