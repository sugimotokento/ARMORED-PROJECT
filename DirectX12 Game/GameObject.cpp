#include"GameObject.h"

void GameObject::Initialize() {

}

void GameObject::Update() {
	for (int i = 0; i < m_child.size(); i++) {
		m_child[i].get()->Update();
	}
}

void GameObject::Draw() {
	for (int i = 0; i < m_child.size(); i++) {
		m_child[i].get()->Draw();
	}
}

void GameObject::Finalize() {
	for (int i = 0; i < m_child.size(); i++) {
		m_child[i].get()->Finalize();
	}
}