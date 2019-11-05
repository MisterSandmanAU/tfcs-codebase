//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"
#include "sdk_weapon_melee.h"

#if defined( CLIENT_DLL )

#define CWeaponMedkit C_WeaponMedkit
#include "c_sdk_player.h"

#else

#include "sdk_player.h"
#include "ilagcompensationmanager.h"

#endif


class CWeaponMedkit : public CWeaponSDKMelee
{
public:
	DECLARE_CLASS(CWeaponMedkit, CWeaponSDKMelee);
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponMedkit();

	void Swing();

	virtual void PrimaryAttack();

	virtual void SecondaryAttack();


	virtual SDKWeaponID GetWeaponID(void) const		{ return SDK_WEAPON_MEDKIT; }
	virtual float	GetRange(void)					{ return	64.0f; }	//Tony; let the crowbar swing further.
	virtual bool CanWeaponBeDropped() const				{ return false; }

private:

	CWeaponMedkit(const CWeaponMedkit &);
	bool HitPlayer(CBaseEntity *pTarget);

};

IMPLEMENT_NETWORKCLASS_ALIASED(WeaponMedkit, DT_WeaponMedkit)

BEGIN_NETWORK_TABLE(CWeaponMedkit, DT_WeaponMedkit)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponMedkit)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_medkit, CWeaponMedkit);
PRECACHE_WEAPON_REGISTER(weapon_medkit);


extern ConVar mp_ignorefriendlyjustheal;
CWeaponMedkit::CWeaponMedkit()
{
}

//------------------------------------------------------------------------------
// Purpose :
// Input   :
// Output  :
//------------------------------------------------------------------------------
void CWeaponMedkit::PrimaryAttack()
{
	Swing();

}

void CWeaponMedkit::SecondaryAttack()
{

}

bool CWeaponMedkit::HitPlayer(CBaseEntity *pTarget)
{
	CSDKPlayer *pOwner = ToSDKPlayer(GetOwnerEntity());

	if (!pOwner)
		return false;

	CSDKPlayer *pSDKPlayer = ToSDKPlayer(pTarget);
	if (!pSDKPlayer)
		return false;

	if (pSDKPlayer->InSameTeam(pOwner))
	{
		return true;
	}
	return false;
}

void CWeaponMedkit::Swing()
{
	trace_t traceHit;

	// Try a ray
	CSDKPlayer *pOwner = GetPlayerOwner();
	if (!pOwner)
		return;

	Vector swingStart = pOwner->Weapon_ShootPosition();
	Vector forward;

	pOwner->EyeVectors(&forward, NULL, NULL);

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit);

#ifndef CLIENT_DLL
	lagcompensation->StartLagCompensation(pOwner, pOwner->GetCurrentCommand());
	
	
#endif
	bool bHitSomething = DoSwingTrace(traceHit);
#if !defined( CLIENT_DLL )
	lagcompensation->FinishLagCompensation(pOwner);
#endif

	if (bHitSomething)
	{
		if (HitPlayer(traceHit.m_pEnt) || mp_ignorefriendlyjustheal.GetBool())
		{
#ifdef GAME_DLL
			CSDKPlayer *pPatient = ToSDKPlayer(traceHit.m_pEnt);

			

			float flDamage = GetSDKWpnData().m_iDamage;

			DevMsg("We have found a subject!\n And we are going to heal them for: %d \n", flDamage);

			pPatient->TakeHealth(GetSDKWpnData().m_iDamage, DMG_GENERIC);
			/*int iHealth = pPatient->GetHealth();

			int iMaxHealth = pPatient->GetMaxHealth();

			if (iHealth < iMaxHealth)
			{
				pPatient->TakeHealth(flDamage, DMG_GENERIC);
			}*/
#endif
#if defined( CLIENT_DLL )
			UTIL_ImpactTrace(&traceHit, DMG_CLUB);
#endif
		}
		else
		{
#ifdef GAME_DLL
			CSDKPlayer *pEnemy = ToSDKPlayer(traceHit.m_pEnt);
			float flDamage = GetSDKWpnData().m_iDamage;

			
			CTakeDamageInfo info(pOwner, pOwner, this, flDamage, DMG_POISON, 8);

			pEnemy->OnTakeDamage(info);
#endif
#if defined( CLIENT_DLL )
			UTIL_ImpactTrace(&traceHit, DMG_CLUB);
#endif
		}
	}
}

//Tony; todo; add ACT_MP_PRONE* activities, so we have them.
acttable_t CWeaponMedkit::m_acttable[] =
{
	{ ACT_MP_STAND_IDLE, ACT_DOD_STAND_AIM_SPADE, false },
	{ ACT_MP_CROUCH_IDLE, ACT_DOD_CROUCH_AIM_SPADE, false },
	{ ACT_MP_PRONE_IDLE, ACT_DOD_PRONE_AIM_SPADE, false },

	{ ACT_MP_RUN, ACT_DOD_RUN_AIM_SPADE, false },
	{ ACT_MP_WALK, ACT_DOD_WALK_AIM_SPADE, false },
	{ ACT_MP_CROUCHWALK, ACT_DOD_CROUCHWALK_AIM_SPADE, false },
	{ ACT_MP_PRONE_CRAWL, ACT_DOD_PRONEWALK_AIM_SPADE, false },
	{ ACT_SPRINT, ACT_DOD_SPRINT_AIM_SPADE, false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE, ACT_DOD_PRIMARYATTACK_SPADE, false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE, ACT_DOD_PRIMARYATTACK_SPADE, false },
	{ ACT_MP_ATTACK_PRONE_PRIMARYFIRE, ACT_DOD_PRIMARYATTACK_PRONE_SPADE, false },
};

IMPLEMENT_ACTTABLE(CWeaponMedkit);

