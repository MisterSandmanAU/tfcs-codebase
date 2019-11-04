//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"

#if defined( CLIENT_DLL )

#define CWeaponAC C_WeaponAC
#include "c_sdk_player.h"

#else

#include "sdk_player.h"

#endif


class CWeaponAC : public CWeaponSDKBase
{
public:
	DECLARE_CLASS(CWeaponAC, CWeaponSDKBase);
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponAC();

	virtual SDKWeaponID GetWeaponID(void) const		{ return SDK_WEAPON_AC; }

private:

	CWeaponAC(const CWeaponAC &);
};

IMPLEMENT_NETWORKCLASS_ALIASED(WeaponAC, DT_WeaponAC)

BEGIN_NETWORK_TABLE(CWeaponAC, DT_WeaponAC)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponAC)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_ac, CWeaponAC);
PRECACHE_WEAPON_REGISTER(weapon_ac);



CWeaponAC::CWeaponAC()
{
}

//Tony; todo; add ACT_MP_PRONE* activities, so we have them.
acttable_t CWeaponAC::m_acttable[] =
{
	{ ref_aim_AC, ref_aim_AC, false },
	{ crouch_idle, crouch_aim_AC, false },
	{ ACT_MP_PRONE_IDLE, ACT_DOD_PRONE_AIM_TOMMY, false },

	{ run2, run2, false },
	{ walk2handed, walk2handed, false },
	{ walk2handed, ACT_DOD_CROUCHWALK_AIM_TOMMY, false },
	{ ACT_MP_PRONE_CRAWL, ACT_DOD_PRONEWALK_IDLE_TOMMY, false },
	{ ACT_SPRINT, ACT_DOD_SPRINT_IDLE_TOMMY, false },

	{ ref_shoot_AC, ref_shoot_AC, false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE, ACT_DOD_PRIMARYATTACK_TOMMY, false },
	{ ACT_MP_ATTACK_PRONE_PRIMARYFIRE, ACT_DOD_PRIMARYATTACK_PRONE_TOMMY, false },
	{ ACT_MP_ATTACK_STAND_SECONDARYFIRE, ACT_DOD_SECONDARYATTACK_TOMMY, false },
	{ ACT_MP_ATTACK_CROUCH_SECONDARYFIRE, ACT_DOD_SECONDARYATTACK_CROUCH_TOMMY, false },
	{ ACT_MP_ATTACK_PRONE_SECONDARYFIRE, ACT_DOD_SECONDARYATTACK_PRONE_TOMMY, false },

	{ ACT_MP_RELOAD_STAND, ACT_DOD_RELOAD_TOMMY, false },
	{ ACT_MP_RELOAD_CROUCH, ACT_DOD_RELOAD_CROUCH_TOMMY, false },
	{ ACT_MP_RELOAD_PRONE, ACT_DOD_RELOAD_PRONE_TOMMY, false },

};

IMPLEMENT_ACTTABLE(CWeaponAC);

