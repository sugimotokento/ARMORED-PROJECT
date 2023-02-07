#include"Main.h"
#include"Collision.h"
#include"GameObject.h"
#include"Cube.h"
#include<math.h>

bool Collision::Box(GameObject* obj1, GameObject* obj2) {
	XMFLOAT3 dir;
	dir.x = obj2->GetPosition().x - obj1->GetPosition().x;
	dir.y = 0;
	dir.z = obj2->GetPosition().z - obj1->GetPosition().z;

	XMFLOAT3 scale;
	scale.x = obj1->GetScale().x + obj2->GetScale().x;
	scale.y = 0;
	scale.z = obj1->GetScale().z + obj2->GetScale().z;

	if (fabsf(dir.x) < scale.x / 2 &&
		fabsf(dir.z) < scale.z / 2) {
		return true;
	}
	return false;
}
bool Collision::Box(Cube* obj1, Cube* obj2) {
	XMFLOAT3 dir;
	dir.x = obj2->GetPosition().x - obj1->GetPosition().x;
	dir.y = 0;
	dir.z = obj2->GetPosition().z - obj1->GetPosition().z;

	XMFLOAT3 scale;
	scale.x = obj1->GetScale().x + obj2->GetScale().x;
	scale.y = 0;
	scale.z = obj1->GetScale().z + obj2->GetScale().z;

	if (fabsf(dir.x) < scale.x/2 &&
		fabsf(dir.z) < scale.z/2) {
		return true;
	}
	return false;
}

XMFLOAT3 Collision::GetBoxPushVec(Cube* obj1, Cube* obj2) {
	XMFLOAT3 dir;
	dir.x = obj2->GetPosition().x - obj1->GetPosition().x;
	dir.y = 0;
	dir.z = obj2->GetPosition().z - obj1->GetPosition().z;

	XMFLOAT3 scale;
	scale.x = obj1->GetScale().x + obj2->GetScale().x;
	scale.y = 0;
	scale.z = obj1->GetScale().z + obj2->GetScale().z;

	XMFLOAT3 pushVec;
	pushVec.x = scale.x/2 - fabsf(dir.x);
	pushVec.z = scale.z/2- fabsf(dir.z);

	if (pushVec.x < pushVec.z) {
		pushVec.z = 0;
		if (dir.x > 0) {
			pushVec.x *= -1;
		}
	}
	else {
		pushVec.x = 0;
		if (dir.z > 0) {
			pushVec.z *= -1;
		}
	}


	return pushVec;
}
XMFLOAT3 Collision::GetBoxPushVec(GameObject* obj1, GameObject* obj2) {
	XMFLOAT3 dir;
	dir.x = obj2->GetPosition().x - obj1->GetPosition().x;
	dir.y = 0;
	dir.z = obj2->GetPosition().z - obj1->GetPosition().z;

	XMFLOAT3 scale;
	scale.x = obj1->GetScale().x + obj2->GetScale().x;
	scale.y = 0;
	scale.z = obj1->GetScale().z + obj2->GetScale().z;

	XMFLOAT3 pushVec;
	pushVec.x = scale.x / 2 - fabsf(dir.x);
	pushVec.z = scale.z / 2 - fabsf(dir.z);

	if (pushVec.x < pushVec.z) {
		pushVec.z = 0;
		if (dir.x > 0) {
			pushVec.x *= -1;
		}
	}
	else {
		pushVec.x = 0;
		if (dir.z > 0) {
			pushVec.z *= -1;
		}
	}


	return pushVec;
}