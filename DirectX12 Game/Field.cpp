#include"Field.h"
#include"Renderer.h"

void Field::Initialize() {
	int startID = Building::Index::MODEL_ID_BUILDING1;
	m_scale = XMFLOAT3(1, 1, 1);
	for (int i = startID; i < startID + BUILDING_MAX; i++) {
		Building* building;
		building = AddChild<Building>();
		building->SetModelID(static_cast<Building::Index::BuildingModelID>(i));

		
		building->SetScale(XMFLOAT3(0.8f, 0.8f, 0.8f));
		building->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID))*100));

		if (static_cast<Building::Index::BuildingModelID>(i) >= Building::Index::MODEL_ID_BUILDING6) {
			building->SetScale(XMFLOAT3(3, 3, 3));
			building->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID)) * 100));
		}
	}
}
void Field::Update() {
	GameObject::Update();
}
void Field::Draw() {
	GameObject::Draw();
}
void Field::Finalize() {
	GameObject::Finalize();
}