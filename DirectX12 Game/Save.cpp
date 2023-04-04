#define _CRT_SECURE_NO_WARNINGS
#include"Save.h"
#include<string>

Save* Save::m_instance = nullptr;

void Save::Write() {
	FILE* fp;

	fp = fopen("SaveData.bin", "wb");

	dataSizeFloat = (unsigned int)dataFloat.size();
	dataSizeInt = (unsigned int)dataInt.size();
	dataSizeBool = (unsigned int)dataBool.size();

	fwrite(&dataSizeFloat, sizeof(int), 1, fp);
	fwrite(&dataSizeInt, sizeof(int), 1, fp);
	fwrite(&dataSizeBool, sizeof(int), 1, fp);

	for (int i = 0; i < (int)dataSizeFloat; i++) {
		float value = dataFloat[i].value;

		fwrite(&value, sizeof(int), 1, fp);
		fwrite(&dataFloat[i].name, sizeof(char), 20, fp);
	}


	for (int i = 0; i < (int)dataSizeInt; i++) {
		int value = dataInt[i].value;

		fwrite(&value, sizeof(int), 1, fp);
		fwrite(&dataInt[i].name, sizeof(char), 20, fp);
	}


	for (int i = 0; i < (int)dataSizeBool; i++) {
		bool value = dataBool[i].value;

		fwrite(&value, sizeof(int), 1, fp);
		fwrite(&dataBool[i].name, sizeof(char), 20, fp);
	}


	fclose(fp);
}
void Save::Load() {
	FILE* fp;

	fp = fopen("SaveData.bin", "rb");

	fread(&dataSizeFloat, sizeof(int), 1, fp);
	fread(&dataSizeInt, sizeof(int), 1, fp);
	fread(&dataSizeBool, sizeof(int), 1, fp);

	for (int i = 0; i < (int)dataSizeFloat; i++) {
		float value;
		char n[21];

		dataFloat.push_back(DataFloat{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		fread(&value, sizeof(int), 1, fp);
		fread(&n, sizeof(char), 20, fp);

		dataFloat[i].value = value;

		for (int j = 0; j < 20; j++) {
			if (dataFloat[i].name[j] != NULL) {
				dataFloat[i].name[j] = n[j];
			}
			else {
				break;
			}
		}
	}

	for (int i = 0; i < (int)dataSizeInt; i++) {
		int value;
		char n[21];

		dataInt.push_back(DataInt{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		fread(&value, sizeof(int), 1, fp);
		fread(&n, sizeof(char), 20, fp);

		dataInt[i].value = value;

		for (int j = 0; j < 20; j++) {
			if (dataInt[i].name[j] != NULL) {
				dataInt[i].name[j] = n[j];
			}
			else {
				break;
			}
		}
	}

	for (int i = 0; i < (int)dataSizeBool; i++) {
		bool value;
		char n[21];

		dataBool.push_back(DataBool{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		fread(&value, sizeof(int), 1, fp);
		fread(&n, sizeof(char), 20, fp);

		dataBool[i].value = value;

		for (int j = 0; j < 20; j++) {
			if (dataBool[i].name[j] != NULL) {
				dataBool[i].name[j] = n[j];
			}
			else {
				break;
			}
		}
	}

	fclose(fp);
}

void Save::DeleteAllFloat() {
	dataFloat.clear();
}
void Save::DeleteAllInt() {
	dataInt.clear();
}
void Save::DeleteAllBool() {
	dataBool.clear();
}

void Save::DeleteKeyFloat(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);

	//同じ名前があれば削除
	for (int i = 0; i < (int)dataFloat.size(); i++) {
		if (strcmp(n, dataFloat[i].name) == 0) {
			dataFloat.erase(dataFloat.begin() + i);
			break;
		}
	}
}
void Save::DeleteKeyInt(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);

	//同じ名前があれば削除
	for (int i = 0; i < (int)dataInt.size(); i++) {
		if (strcmp(n, dataInt[i].name) == 0) {
			dataInt.erase(dataInt.begin() + i);
			break;
		}
	}
}
void Save::DeleteKeyBool(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);

	//同じ名前があれば削除
	for (int i = 0; i < (int)dataBool.size(); i++) {
		if (strcmp(n, dataBool[i].name) == 0) {
			dataBool.erase(dataBool.begin() + i);
			break;
		}
	}
}


void Save::SetFloat(std::string name, float value) {
	//すでに同じ名前のデータが存在するか調べる.存在すれば書き換え無ければ追加
	bool isExists = false;
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);

	//同じデーターがあれば書き換え
	for (int i = 0; i < (int)dataFloat.size(); i++) {
		if (strcmp(n, dataFloat[i].name) == 0) {
			isExists = true;
			dataFloat[i].value = value;
			break;
		}
	}


	//データが存在しなければ追加
	if (isExists == false) {
		dataFloat.push_back(DataFloat{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		dataFloat[(int)dataFloat.size() - 1].value = value;
		for (int i = 0; i < 20; i++) {
			if (dataFloat[(int)dataFloat.size() - 1].name[i] != NULL) {
				dataFloat[(int)dataFloat.size() - 1].name[i] = n[i];
			}
			else {
				break;
			}
		}
	}
}
void Save::SetInt(std::string name, int value) {
	//すでに同じ名前のデータが存在するか調べる.存在すれば書き換え無ければ追加
	bool isExists = false;
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);


	//同じデーターがあれば書き換え
	for (int i = 0; i < (int)dataInt.size(); i++) {
		if (strcmp(n, dataInt[i].name) == 0) {
			isExists = true;
			dataInt[i].value = value;
			break;
		}
	}


	if (isExists == false) {
		//同じデータがなかったわ
		dataInt.push_back(DataInt{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		dataInt[(int)dataInt.size() - 1].value = value;
		for (int i = 0; i < 20; i++) {
			if (dataInt[(int)dataInt.size() - 1].name[i] != NULL) {
				dataInt[(int)dataInt.size() - 1].name[i] = n[i];
			}
			else {
				break;
			}
		}
	}
}
void Save::SetBool(std::string name, bool value) {
	//すでに同じ名前のデータが存在するか調べる.存在すれば書き換え無ければ追加
	bool isExists = false;
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);


	//同じデーターがあれば書き換え
	for (int i = 0; i < (int)dataBool.size(); i++) {
		if (strcmp(n, dataBool[i].name) == 0) {
			isExists = true;
			dataBool[i].value = value;
			break;
		}
	}


	if (isExists == false) {
		//同じデータがなかったわ
		dataBool.push_back(DataBool{ 0, "aaaaaaaaaaaaaaaaaaaa" });
		dataBool[(int)dataBool.size() - 1].value = value;
		for (int i = 0; i < 20; i++) {
			if (dataBool[(int)dataBool.size() - 1].name[i] != NULL) {
				dataBool[(int)dataBool.size() - 1].name[i] = n[i];
			}
			else {
				break;
			}
		}
	}
}

float Save::GetFloat(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);

	float value = NULL;
	for (int i = 0; i < (int)dataFloat.size(); i++) {
		if (strcmp(n, dataFloat[i].name) == 0) {
			value = dataFloat[i].value;
		}
	}

	return value;
}
int   Save::GetInt(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);
	int value = NULL;

	for (int i = 0; i < (int)dataInt.size(); i++) {
		if (strcmp(n, dataInt[i].name) == 0) {
			value = dataInt[i].value;
		}
	}

	return value;
}
bool  Save::GetBool(std::string name) {
	char n[21];
	std::char_traits<char>::copy(n, name.c_str(), name.size() + 1);
	float value = NULL;

	for (int i = 0; i < (int)dataBool.size(); i++) {
		if (strcmp(n, dataBool[i].name) == 0) {
			value = dataBool[i].value;
		}
	}

	return value;
}


void Save::Create() {
	if (!m_instance) {
		m_instance = new Save();
	}
}
void Save::Destroy() {
	delete m_instance;
	m_instance = nullptr;
}
Save* Save::GetInstance() {
	return m_instance;
}