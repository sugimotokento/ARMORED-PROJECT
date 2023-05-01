#include"JsonManager.h"

JsonManager* JsonManager::m_instance = nullptr;

ptree JsonManager::GetData(Index::JsonID id)
{
	std::string fileName = JsonManager::Index::GetJsonFileName(id);
	ptree data;
	read_json(fileName, data);
	return data;
}

void JsonManager::SetData(Index::JsonID id, ptree data)
{
	std::string fileName = JsonManager::Index::GetJsonFileName(id);
	write_json(fileName, data);
}

void JsonManager::Create() {
	if (!m_instance) {
		m_instance = new JsonManager();
	}
}
void JsonManager::Destroy() {
	delete m_instance;
	m_instance = nullptr;
}
JsonManager* JsonManager::GetInstance() {
	return m_instance;
}