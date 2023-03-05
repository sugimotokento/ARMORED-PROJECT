#pragma once

class GameObject;
class Collision {
public:
	static bool Box(GameObject* obj1, GameObject* obj2);
	static XMFLOAT3 GetBoxPushVec(GameObject* obj1, GameObject* obj2);
};