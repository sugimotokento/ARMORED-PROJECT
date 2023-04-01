#pragma once
#include<list>
#include <functional>
#include<vector>
class GameObject;
class Scene {

public:
	enum Layer {
		GEOMETRY,	//不透明オブジェクトレイヤー
		WATER,		//水面オブジェクトレイヤー
		ALPHA,		//透過オブジェクトレイヤー
		SPRITE,		//2Dオブジェクトレイヤー
		COUNT		
	};

private:
	std::list<GameObject*> m_gameObject[Layer::COUNT];

	
public:
	Scene(){}
	~Scene(){}
	virtual void Initialize() {};
	virtual void Update();
	virtual void Draw(Layer layer);
	virtual void Finalize();


	template <typename T>
	T* AddGameObject(Layer layer=GEOMETRY) {
		T* gameObject = new T();
		m_gameObject[layer].push_back(gameObject);
		gameObject->Initialize();
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
	std::vector<T*> GetGameObjects(Layer layer = GEOMETRY) {
		std::vector<T*> objects;
		for (GameObject* obj : m_gameObject[layer]) {
			if (typeid(*obj) == typeid(T)) {
				objects.push_back((T*)obj);
			}
		}

		return objects;
	}
};
