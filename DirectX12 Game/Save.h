#pragma once
#include<vector>
#include<string>

struct DataFloat {
	float value;
	char name[21];
};
struct DataInt {
	int value;
	char name[21];
};
struct DataBool {
	bool value;
	char name[21];
};

class Save {
private:
	static Save* m_instance;

	//vector�̃T�C�Y��ۑ����Ƃ��p
	unsigned int dataSizeFloat;
	unsigned int dataSizeBool;
	unsigned int dataSizeInt;


	//�Z�[�u����f�[�^�������ɓ����
	std::vector<DataFloat> dataFloat;
	std::vector<DataInt> dataInt;
	std::vector<DataBool> dataBool;


public:
	void Write(); //�Ō��1�񂾂��Ă�
	void Load();  //�ŏ���1�񂾂��Ă�



	//���O��20�����܂�
	//�Ăяo�����߂̖��O�ƃZ�[�u����l������
	void SetFloat(std::string name, float value);
	void SetInt(std::string name, int value);
	void SetBool(std::string name, bool value);

	//�Z�b�g�֐���name���L�[�ɂ��ČĂяo��
	float GetFloat(std::string name);
	int GetInt(std::string name);
	bool GetBool(std::string name);


	//�f�[�^�̑S�폜
	void DeleteAllFloat();
	void DeleteAllInt();
	void DeleteAllBool();

	//�L�[�ō폜
	void DeleteKeyFloat(std::string name);
	void DeleteKeyInt(std::string name);
	void DeleteKeyBool(std::string name);


	static void Create();
	static void Destroy();
	static Save* GetInstance();
};