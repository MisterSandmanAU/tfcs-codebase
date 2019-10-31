#ifndef TFC_PROJECTILE_BASE_H
#define TFC_PROJECTILE_BASE_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "baseprojectile.h"

#ifdef CLIENT_DLL
#include "tempent.h"
#endif

//
// projectile base (some generic projectile to derive off)
//
class CTFCProjectileBase : public CBaseProjectile
{
public:
	DECLARE_CLASS(CTFCProjectileBase, CBaseProjectile);
	DECLARE_DATADESC();
	DECLARE_NETWORKCLASS();

	CTFCProjectileBase();
	~CTFCProjectileBase();

	void Precache(void);
	void Spawn(void);
	const char *GetProjectileModelName(void);
	#ifdef GAME_DLL
	virtual void ProjectileTouch(CBaseEntity *pOther);
	#endif
protected:
	virtual void ProjectileThink(void);
	float m_flDamageAmount;
};

#endif //TFC_PROJECTILE_BASE_H