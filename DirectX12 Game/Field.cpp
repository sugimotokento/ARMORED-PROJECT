#include"Field.h"
#include"Renderer.h"

void Field::Initialize() {
	int startID = Building::Index::MODEL_ID_BUILDING1;
	for (int i = startID; i < startID + BUILDING_MAX; i++) {
		m_building[i - startID] = std::make_unique<Building>();
		m_building[i - startID].get()->SetModelID(static_cast<Building::Index::BuildingModelID>(i));
		m_building[i - startID].get()->Initialize();

		
		m_building[i - startID].get()->SetScale(XMFLOAT3(0.8f, 0.8f, 0.8f));
		m_building[i - startID].get()->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID))*100));

		if (static_cast<Building::Index::BuildingModelID>(i) >= Building::Index::MODEL_ID_BUILDING6) {
			m_building[i - startID].get()->SetScale(XMFLOAT3(3, 3, 3));
			m_building[i - startID].get()->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID)) * 100));
		}
	}

}
void Field::Update() {

}
void Field::Draw() {
	for (int i = 0; i < BUILDING_MAX; i++) {
		m_building[i].get()->Draw();
	}
}
void Field::Finalize() {

}