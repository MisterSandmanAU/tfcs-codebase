//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2MPTTC__WEAPON_CROWBAR_H
#define HL2MPTFC__WEAPON_CROWBAR_H
#pragma once


#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "weapon_hl2mpbasebasebludgeon.h"


#ifdef CLIENT_DLL
#define CTfc_WeaponCrowbar C_Tfc_WeaponCrowbar
#endif

//-----------------------------------------------------------------------------
// CWeaponCrowbar
//-----------------------------------------------------------------------------

class CTfc_WeaponCrowbar : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS(CTfc_WeaponCrowbar, CBaseHL2MPBludgeonWeapon);

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

	CTfc_WeaponCrowbar();

	float		GetRange(void);
	float		GetFireRate(void);

	void		AddViewKick(void);
	float		GetDamageForActivity(Activity hitActivity);
	void		SecondaryAttack(void)	{ return; }

	void		Drop(const Vector &vecVelocity);


	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);
	void HandleAnimEventMeleeHit(animevent_t *pEvent, CBaseCombatCharacter *pOperator);
	int WeaponMeleeAttack1Condition(float flDot, float flDist);
#endif

	CTfc_WeaponCrowbar(const CTfc_WeaponCrowbar &);

private:

};


#endif // HL2MP_WEAPON_CROWBAR_H

