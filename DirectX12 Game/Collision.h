#pragma once

class GameObject;
class Cube;
class Collision {
public:
	static bool Box(GameObject* obj1, GameObject* obj2);
	static bool Box(Cube* obj1, Cube* obj2);
	static XMFLOAT3 GetBoxPushVec(GameObject* obj1, GameObject* obj2);
	static XMFLOAT3 GetBoxPushVec(Cube* obj1, Cube* obj2);
};