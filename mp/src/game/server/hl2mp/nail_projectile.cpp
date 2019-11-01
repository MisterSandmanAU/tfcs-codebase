#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#include "c_te_effect_dispatch.h"
#else
#include "hl2mp_player.h"
#include "te_effect_dispatch.h"
#include "IEffects.h"
#include "Sprite.h"
#include "SpriteTrail.h"
#include "beam_shared.h"
#include "nail_projectile.h"
#endif

#ifndef CLIENT_DLL

extern ConVar sk_plr_dmg_crossbow;
extern ConVar sk_npc_dmg_crossbow;

void TE_StickyBolt(IRecipientFilter& filter, float delay, Vector vecDirection, const Vector *origin);


LINK_ENTITY_TO_CLASS(projectile_nail, CNailProjectile);

BEGIN_DATADESC(CNailProjectile)
// Function Pointers
DEFINE_FUNCTION(BubbleThink),
DEFINE_FUNCTION(BoltTouch),

// These are recreated on reload, they don't need storage
DEFINE_FIELD(m_pGlowSprite, FIELD_EHANDLE),
//DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),

END_DATADESC()

/*IMPLEMENT_SERVERCLASS_ST(CNailProjectile, DT_NailProjectile)
END_SEND_TABLE()*/

#define BOLT_MODEL	"models/weapons/w_models/w_nail.mdl"


#define	BOLT_SKIN_NORMAL	0
#define BOLT_SKIN_GLOW		1
CNailProjectile *CNailProjectile::BoltCreate(const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner)
{
	// Create a new entity with CNailProjectile private data
	CNailProjectile *pBolt = (CNailProjectile *)CreateEntityByName("projectile_nail");
	UTIL_SetOrigin(pBolt, vecOrigin);
	pBolt->SetAbsAngles(angAngles);
	pBolt->Spawn();
	pBolt->SetOwnerEntity(pentOwner);

	pBolt->m_iDamage = iDamage;

	return pBolt;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CNailProjectile::~CNailProjectile(void)
{
	if (m_pGlowSprite)
	{
		UTIL_Remove(m_pGlowSprite);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNailProjectile::CreateVPhysics(void)
{
	// Create the object in the physics system
	VPhysicsInitNormal(SOLID_BBOX, FSOLID_NOT_STANDABLE, false);

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CNailProjectile::PhysicsSolidMaskForEntity() const
{
	return (BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX) & ~CONTENTS_GRATE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNailProjectile::CreateSprites(void)
{
	// Start up the eye glow
	m_pGlowSprite = CSprite::SpriteCreate("sprites/light_glow02_noz.vmt", GetLocalOrigin(), false);

	if (m_pGlowSprite != NULL)
	{
		m_pGlowSprite->FollowEntity(this);
		m_pGlowSprite->SetTransparency(kRenderGlow, 255, 255, 255, 128, kRenderFxNoDissipation);
		m_pGlowSprite->SetScale(0.2f);
		m_pGlowSprite->TurnOff();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNailProjectile::Spawn(void)
{
	Precache();

	SetModel(BOLT_MODEL);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM);
	UTIL_SetSize(this, -Vector(1, 1, 1), Vector(1, 1, 1));
	SetSolid(SOLID_BBOX);
	SetGravity(0.01f);

	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch(&CNailProjectile::BoltTouch);

	SetThink(&CNailProjectile::BubbleThink);
	SetNextThink(gpGlobals->curtime + 0.1f);

	CreateSprites();

	// Make us glow until we've hit the wall
	m_nSkin = BOLT_SKIN_GLOW;
}


void CNailProjectile::Precache(void)
{
	PrecacheModel(BOLT_MODEL);

	// This is used by C_TEStickyBolt, despte being different from above!!!
	PrecacheModel("models/weapons/w_models/w_nail.mdl");

	PrecacheModel("sprites/light_glow02_noz.vmt");
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CNailProjectile::BoltTouch(CBaseEntity *pOther)
{
	if (!pOther->IsSolid() || pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS))
		return;

	if (pOther->m_takedamage != DAMAGE_NO)
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize(vecNormalizedVel);

		if (GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC())
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), m_iDamage, DMG_NEVERGIB);
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
		}
		else
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), m_iDamage, DMG_BULLET | DMG_NEVERGIB);
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
		}

		ApplyMultiDamage();

		//Adrian: keep going through the glass.
		if (pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS)
			return;

		SetAbsVelocity(Vector(0, 0, 0));

		// play body "thwack" sound
		EmitSound("Weapon_Crossbow.BoltHitBody");

		Vector vForward;

		AngleVectors(GetAbsAngles(), &vForward);
		VectorNormalize(vForward);

		UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + vForward * 128, MASK_OPAQUE, pOther, COLLISION_GROUP_NONE, &tr2);

		if (tr2.fraction != 1.0f)
		{
			//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
			//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			if (tr2.m_pEnt == NULL || (tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE))
			{
				CEffectData	data;

				data.m_vOrigin = tr2.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = tr2.fraction != 1.0f;

				DispatchEffect("NailImpact", data);
			}
		}

		SetTouch(NULL);
		SetThink(NULL);

		UTIL_Remove(this);
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if (pOther->GetMoveType() == MOVETYPE_NONE && !(tr.surface.flags & SURF_SKY))
		{
			EmitSound("Weapon_Crossbow.BoltHitWorld");

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
//			float speed = VectorNormalize(vecDir);

			SetThink(&CNailProjectile::SUB_Remove);
			SetNextThink(gpGlobals->curtime + 2.0f);

			//FIXME: We actually want to stick (with hierarchy) to what we've hit
			SetMoveType(MOVETYPE_NONE);

			Vector vForward;

			AngleVectors(GetAbsAngles(), &vForward);
			VectorNormalize(vForward);

			CEffectData	data;

			data.m_vOrigin = tr.endpos;
			data.m_vNormal = vForward;
			data.m_nEntIndex = 0;

			DispatchEffect("NailImpact", data);

			UTIL_ImpactTrace(&tr, DMG_BULLET);

			AddEffects(EF_NODRAW);
			SetTouch(NULL);
			SetThink(&CNailProjectile::SUB_Remove);
			SetNextThink(gpGlobals->curtime + 2.0f);

			if (m_pGlowSprite != NULL)
			{
				m_pGlowSprite->TurnOn();
				m_pGlowSprite->FadeAndDie(3.0f);
			}

			// Shoot some sparks
			if (UTIL_PointContents(GetAbsOrigin()) != CONTENTS_WATER)
			{
				g_pEffects->Sparks(GetAbsOrigin());
			}
		}
		else
		{
			// Put a mark unless we've hit the sky
			if ((tr.surface.flags & SURF_SKY) == false)
			{
				UTIL_ImpactTrace(&tr, DMG_BULLET);
			}

			UTIL_Remove(this);
		}
	}

	if (g_pGameRules->IsMultiplayer())
	{
		//		SetThink( &CCrossbowBolt::ExplodeThink );
		//		SetNextThink( gpGlobals->curtime + 0.1f );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNailProjectile::BubbleThink(void)
{
	QAngle angNewAngles;

	VectorAngles(GetAbsVelocity(), angNewAngles);
	SetAbsAngles(angNewAngles);

	SetNextThink(gpGlobals->curtime + 0.1f);

	if (GetWaterLevel() == 0)
		return;

	UTIL_BubbleTrail(GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5);
}

#endif