#include"Field.h"
#include"Renderer.h"

void Field::Initialize() {
	int startID = FieldObject::Index::MODEL_ID_BUILDING1;
	m_scale = XMFLOAT3(1, 1, 1);
	for (int i = startID; i < startID + BUILDING_MAX; i++) {
		FieldObject* fieldObject;
		fieldObject = AddChild<FieldObject>();
		fieldObject->SetModelID(static_cast<FieldObject::Index::FieldModelID>(i));

		
		fieldObject->SetScale(XMFLOAT3(0.8f, 0.8f, 0.8f));
		fieldObject->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID))*100));

		if (static_cast<FieldObject::Index::FieldModelID>(i) >= FieldObject::Index::MODEL_ID_BUILDING6) {
			fieldObject->SetScale(XMFLOAT3(3, 3, 3));
			fieldObject->SetPosition(XMFLOAT3(0, -100, ((float)(i - startID)) * 100));
		}
	}
}
void Field::Update() {
	GameObject::Update();
}
void Field::Draw() {
	GameObject::Draw();
	CreateWorldMTX(m_scale, m_position, m_rotation);
}
void Field::Finalize() {
	GameObject::Finalize();
}