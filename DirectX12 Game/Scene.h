#pragma once
#include<list>
#include <functional>

class GameObject;
class Scene {

public:
	enum Layer {
		GEOMETRY,
		WATER,
		ALPHA,
		SPRITE,
		COUNT
	};

private:
	std::list<GameObject*> m_gameObject[Layer::COUNT];

	
public:
	Scene(){}
	~Scene(){}
	virtual void Update();
	virtual void Draw(Layer layer);
	virtual void Finalize();


	template <typename T>
	T* AddGameObject(Layer layer=GEOMETRY) {
		T* gameObject = new T();
		gameObject->Initialize();
		m_gameObject[layer].push_back(gameObject);
		return (T*)gameObject;
	}
	template <typename T>
	T* GetGameObject(Layer layer = GEOMETRY) {
		
		for (GameObject* obj : m_gameObject[layer]) {
			if (typeid(*obj) == typeid(T)) {
				return (T*)obj;
			}
		}

		return nullptr;
	}
	template <typename T>
	std::list<T*> GetGameObjects(Layer layer = GEOMETRY) {
		std::list<T*> objects;
		for (GameObject* obj : m_gameObject[layer]) {
			if (typeid(*obj) == typeid(T)) {
				objects.push_back((T*)obj);
			}
		}

		return objects;
	}
};
