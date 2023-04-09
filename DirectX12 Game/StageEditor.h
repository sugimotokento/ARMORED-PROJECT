#ifdef _DEBUG
#pragma once
#include"GameObject.h"
#include<memory>
#include"FieldObject.h"


class StageEditor :public GameObject {
private:
	struct EditParam {
		float objectMoveSpeed = 0.5f;
		float objectRotationSpeed = 0.1f;
		float objectChangeScaleSpeed = 0.99f;
	};

private:
	std::unique_ptr<FieldObject> m_editObject;
	std::vector<std::unique_ptr<FieldObject>> m_objectList;
	bool m_isEditMode = false;
	bool m_isEditObjectMode = false;


	//オブジェクトリストから削除する用
	int m_removeIndex = 0;
	bool m_isRemove = false;


	//-----------------------------------
	//コントローラー入力でオブジェクトを編集する機能
	//----------------------------------
	void EditObject();
	void EditPosition();
	void EditScale();
	void EditRotation();

	//---------------------
	//ImGui周り
	//---------------------
	bool ImguiStageEditor(bool isVisible);	//ImguiRendererに追加する関数
	void ImguiEditWindow();					//オブジェクトの生成、追加、編集のオンオフ
	void ImguiSetObjectWindow();			//設定済みオブジェクトの表示

public:
	void Initialize() final override;
	void Update() final override;
	void Draw() final override;
	void Finalize() final override;

	bool GetIsEditMode() { return m_isEditMode; }
	bool GetIsEditObjectMode() { return m_isEditObjectMode; }
};

#endif
