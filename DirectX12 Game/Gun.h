#pragma once

class GameObject;
class Gun {
protected:
	GameObject* m_obj;

public:
	Gun(){}
	~Gun(){}
	virtual void Update() = 0;

	void SetGameObject(GameObject* obj) { m_obj = obj; }
};