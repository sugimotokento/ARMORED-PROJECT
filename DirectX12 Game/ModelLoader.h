#pragma once
#include"Model.h"
#include"TextureGeometry.h"
#include<memory>
#include<list>
#include<string>
#include<map>
#include<thread>

class ModelLoader {
public:
	struct Index {

		//ロボットモデルID
#define MODEL_ROBOT_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)
#define MODEL_ROBOT_ID_TABLE\
		MODEL_ROBOT_ID(DREADNOUGHT_HEAD,	   "Dreadnought/Head.fbx",		L"Dreadnought/T_DN_Head_Albedo.tga",        L"Dreadnought/T_DN_Head_NormalMap.png",      L"Dreadnought/T_DN_Head_Occlusion.png",     L"Dreadnought/T_DN_Head_Metallic.png" ,    L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_EYE,		   "Dreadnought/Eye.fbx",		L"Dreadnought/T_DN_Head_Albedo.tga",        L"Dreadnought/T_DN_Head_NormalMap.png",      L"Dreadnought/T_DN_Head_Occlusion.png",     L"Dreadnought/T_DN_Head_Metallic.png" ,    L"Dreadnought/T_DN_Head_Metallic.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_ARM,		   "Dreadnought/Arm.fbx",		L"Dreadnought/T_DN_Arm_Albedo.tga",		    L"Dreadnought/T_DN_Arm_NormalMap.png",       L"Dreadnought/T_DN_Arm_Occlusion.png",      L"Dreadnought/T_DN_Arm_Metallic.png",      L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_HAND_L,	   "Dreadnought/HandR.fbx",		L"Dreadnought/T_DN_Arm_Albedo.tga",		    L"Dreadnought/T_DN_Arm_NormalMap.png",       L"Dreadnought/T_DN_Arm_Occlusion.png",      L"Dreadnought/T_DN_Arm_Metallic.png",      L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_HAND_R,	   "Dreadnought/HandL.fbx",		L"Dreadnought/T_DN_Arm_Albedo.tga",		    L"Dreadnought/T_DN_Arm_NormalMap.png",       L"Dreadnought/T_DN_Arm_Occlusion.png",      L"Dreadnought/T_DN_Arm_Metallic.png",      L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_LOWER,	   "Dreadnought/Lower.fbx",		L"Dreadnought/T_DN_Lower_Albedo.tga",       L"Dreadnought/T_DN_Lower_NormalMap.png",     L"Dreadnought/T_DN_Lower_Occlusion.png",    L"Dreadnought/T_DN_Lower_Metallic.png",    L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_SHOULDER,   "Dreadnought/Shoulder.fbx",  L"Dreadnought/T_DN_Shoulder_Albedo.tga",    L"Dreadnought/T_DN_Shoulder_NormalMap.png",  L"Dreadnought/T_DN_Shoulder_Occlusion.png", L"Dreadnought/T_DN_Shoulder_Metallic.png", L"Black.png")\
		MODEL_ROBOT_ID(DREADNOUGHT_UPPER,      "Dreadnought/Upper.fbx",     L"Dreadnought/T_DN_Upper_Albedo.tga",       L"Dreadnought/T_DN_Upper_NormalMap.png",     L"Dreadnought/T_DN_Upper_Occlusion.png",    L"Dreadnought/T_DN_Upper_Metallic.png",    L"Black.png")\


		
		//武器モデルID
#define MODEL_WEAPON_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)
#define MODEL_WEAPON_ID_TABLE\
		MODEL_WEAPON_ID(ASSAULT_RIFLE, "Weapon/Rifle/ScifiRifle2Static.fbx", L"Weapon/Rifle/ScifiRifle2BlackAlbedo.tga", L"Weapon/Rifle/ScifiRifle2Normal.tga", L"Weapon/Rifle/ScifiRifle2AO.jpg", L"Weapon/Rifle/ScifiRifle2PBRMetalness.jpg", L"Black.png")\
		MODEL_WEAPON_ID(SHOTGUN, "Weapon/Shotgun/Shotgun.fbx", L"Weapon/Shotgun/T_DN_Shotgun_Albedo.tga", L"Weapon/Shotgun/T_DN_Shotgun_NormalMap.png", L"Weapon/Shotgun/T_DN_Shotgun_Occlusion.png", L"Weapon/Shotgun/T_DN_Shotgun_Metallic.tga", L"Weapon/Shotgun/T_DN_Shotgun_Emission.png")\



		//弾モデルID
#define MODEL_BULLET_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)
#define MODEL_BULLET_ID_TABLE\
		MODEL_BULLET_ID(AMMUNITION, "Bullet/ScifiRifle2Projectile.fbx", L"Bullet/ScifiRifle2ProjectileAlbedo.png", L"White.png", L"White.png", L"Black.png", L"Bullet/ScifiRifle2ProjectileEmmision.png")\

		

		//フィールドモデルID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)
#define MODEL_FIELD_ID_TABLE\
		MODEL_FIELD_ID(AMMUNITION, "Field/skydome.obj", L"White.png", L"White.png", L"White.png", L"Black.png", L"Black.png")\





		//使用するテクスチャの種類
		enum TextureID {
			TEXTURE_ALBEDO,
			TEXTURE_NORMAL,
			TEXTURE_OCCLUSION,
			TEXTURE_METAL,
			TEXTURE_EMMISION,
			TEXTURE_MAX
		};

		enum ModelID {
			//ロボットモデルID
#undef MODEL_ROBOT_ID
#define MODEL_ROBOT_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_ROBOT_##name,
			MODEL_ROBOT_ID_TABLE

			//武器モデルID
#undef MODEL_WEAPON_ID
#define MODEL_WEAPON_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_WEAPON_##name,
			MODEL_WEAPON_ID_TABLE

			//弾モデルID
#undef MODEL_BULLET_ID
#define MODEL_BULLET_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_BULLET_##name,
			MODEL_BULLET_ID_TABLE

			//フィールドモデルID
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)MODEL_ID_FIELD_##name,
			MODEL_FIELD_ID_TABLE

			MODEL_MAX
		};


		static std::string GetModelFileName(ModelID id) {
			std::string str = "asset/model/";

			//ロボットモデルID
#undef MODEL_ROBOT_ID
#define MODEL_ROBOT_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_ROBOT_##name:\
			str+=##modelName;\
			return str;\


			//武器モデルID
#undef MODEL_WEAPON_ID
#define MODEL_WEAPON_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_WEAPON_##name:\
			str+=##modelName;\
			return str;\


			//弾モデルID
#undef MODEL_BULLET_ID
#define MODEL_BULLET_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_BULLET_##name:\
			str+=##modelName;\
			return str;\


			//フィールドモデルID
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_FIELD_##name:\
			str+=##modelName;\
			return str;\


			switch (id) {
				MODEL_ROBOT_ID_TABLE
				MODEL_WEAPON_ID_TABLE
				MODEL_BULLET_ID_TABLE
				MODEL_FIELD_ID_TABLE
			}

			return " ";
		}


		static std::wstring GetTextureFileName(ModelID modelID, TextureID texID) {
			std::wstring str[TextureID::TEXTURE_MAX];
			for (int i = 0; i < TextureID::TEXTURE_MAX; i++) {
				str[i] = L"asset/Texture/";
			}

			//ロボットモデルID
#undef MODEL_ROBOT_ID
#define MODEL_ROBOT_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_ROBOT_##name:\
			str[0]+=##albedoTex;\
			str[1]+=##normalTex;\
			str[2]+=##occlusionTex;\
			str[3]+=##metalTex;\
			str[4]+=##emmisionTex;\
			return str[texID];\


			//武器モデルID
#undef MODEL_WEAPON_ID
#define MODEL_WEAPON_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_WEAPON_##name:\
			str[0]+=##albedoTex;\
			str[1]+=##normalTex;\
			str[2]+=##occlusionTex;\
			str[3]+=##metalTex;\
			str[4]+=##emmisionTex;\
			return str[texID];\


			//武器モデルID
#undef MODEL_BULLET_ID
#define MODEL_BULLET_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_BULLET_##name:\
			str[0]+=##albedoTex;\
			str[1]+=##normalTex;\
			str[2]+=##occlusionTex;\
			str[3]+=##metalTex;\
			str[4]+=##emmisionTex;\
			return str[texID];\


			//フィールドモデルID
#undef MODEL_FIELD_ID
#define MODEL_FIELD_ID(name, modelName, albedoTex, normalTex, occlusionTex, metalTex, emmisionTex)\
		case MODEL_ID_FIELD_##name:\
			str[0]+=##albedoTex;\
			str[1]+=##normalTex;\
			str[2]+=##occlusionTex;\
			str[3]+=##metalTex;\
			str[4]+=##emmisionTex;\
			return str[texID];\


			switch (modelID) {
				MODEL_ROBOT_ID_TABLE
				MODEL_WEAPON_ID_TABLE
				MODEL_BULLET_ID_TABLE
				MODEL_FIELD_ID_TABLE
			}

			return L" ";
		}
	};

private:
	struct LoadModel {
		//メッシュデータ
		std::unique_ptr<Model> model;

		//3Dモデル用テクスチャ
		std::unique_ptr<TextureGeometry> texture;

		//ロードしているモデルID
		Index::ModelID id;
	};

	std::map<Index::ModelID, LoadModel> m_loadModel;
	static ModelLoader* m_instance;

	//ロード済み:true
	//ロードしてない:false
	bool CheckLoadedModel(Index::ModelID id);
public:
	void LoadRequest(Index::ModelID id);
	void UnloadRequest(Index::ModelID id);
	void Draw(Index::ModelID id);
	void Finalize();


	static void Create();
	static void Destroy();
	static ModelLoader* GetInstance();
};