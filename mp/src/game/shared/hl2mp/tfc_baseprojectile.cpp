#include "cbase.h"
#include "tfc_projectile_base.h"

#ifdef GAME_DLL
#include "hl2mp_player.h"
#endif

#ifdef CLIENT_DLL
#include "c_basetempentity.h"
#include "c_te_legacytempents.h"
#include "c_te_effect_dispatch.h"
#endif

#ifdef GAME_DLL
BEGIN_DATADESC(CTFCProjectileBase)
DEFINE_ENTITYFUNC(ProjectileTouch),
DEFINE_THINKFUNC(ProjectileThink),
END_DATADESC()
#endif

CTFCProjectileBase::CTFCProjectileBase()
{
}

CTFCProjectileBase::~CTFCProjectileBase()
{
}

void CTFCProjectileBase::Precache(void)
{
#ifdef GAME_DLL
	PrecacheModel(GetProjectileModelName());
#endif
	BaseClass::Precache();
}

void CTFCProjectileBase::Spawn()
{
	Precache();
	SetModel(GetProjectileModelName());

	SetSolid(SOLID_BBOX);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM);
	AddEFlags(EFL_NO_WATER_VELOCITY_CHANGE);

	SetGravity(GetGravity());
	m_takedamage = DAMAGE_NO;
	SetCollisionGroup(COLLISION_GROUP_PROJECTILE);
	SetTouch(&CTFCProjectileBase::ProjectileTouch);
	SetThink(&CTFCProjectileBase::ProjectileThink);
	SetNextThink(gpGlobals->curtime);
}