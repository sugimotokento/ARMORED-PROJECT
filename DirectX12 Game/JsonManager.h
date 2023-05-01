#pragma once
#include<memory>
#include<list>
#include<string>
#include<map>
#include<thread>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace boost::property_tree;

class JsonManager {
public:
	struct Index {

		//name			:enumのJSON_ID_から先の名前を設定する
		//fileName		:Jsonファイルの"asset/jsonData/"から先のパスを指定する

		//DataID
#define JSON_DATA_ID(name, fileName)
#define JSON_DATA_ID_TABLE\
		JSON_DATA_ID(DATA_TEST,	"DataTest.json")\


		enum JsonID {
			//DataID
#undef JSON_DATA_ID
#define JSON_DATA_ID(name, fileName)JSON_ID_##name,
			JSON_DATA_ID_TABLE

		};


		static std::string GetJsonFileName(JsonID id) {
			std::string str = "asset/jsonData/";

			//DataID
#undef JSON_DATA_ID
#define JSON_DATA_ID(name, fileName)\
		case JSON_ID_##name:\
			str+=##fileName;\
			return str;\


			switch (id) {
				JSON_DATA_ID_TABLE
			}

			return " ";
		}
	};

private:
	static JsonManager* m_instance;

public:
	static void Create();
	static void Destroy();
	static JsonManager* GetInstance();

	void SetData(Index::JsonID id, ptree data);
	ptree GetData(Index::JsonID id);

	//template <typename T>
	//inline T GetData(Index::JsonID id, std::string key) {
	//	std::string fileName = JsonManager::Index::GetJsonFileName(id);
	//	ptree data;
	//	read_json(fileName, data);
	//	if (boost::optional<T> value = data.get_optional<T>(key)) {
	//		return value.get();
	//	}

	//	T NON;
	//	return NON;
	//}
};