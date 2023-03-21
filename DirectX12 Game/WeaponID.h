#pragma once
#include"AssaultRifle.h"
#include"Shotgun.h"
#include<memory>

#define WEAPON_ID(name, className)
#define WEAPON_ID_TABLE\
		WEAPON_ID(ASSAULT_RIFLE, new AssaultRifle)\
		WEAPON_ID(SHOTGUN, new Shotgun)\


enum WeaponID {
#undef WEAPON_ID
#define WEAPON_ID(name, className)WEAPON_ID_##name,
	WEAPON_ID_TABLE
	WEAPON_ID_MAX
};

Weapon* CreateWeapon(WeaponID id) {
#undef WEAPON_ID
#define WEAPON_ID(name, className)\
		case WEAPON_ID_##name:\
			return  className();

	switch (id){
		WEAPON_ID_TABLE
	}

	return nullptr;
}
