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
#endif

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CNailProjectile : public CBaseCombatCharacter
{
	DECLARE_CLASS(CNailProjectile, CBaseCombatCharacter);

public:
	CNailProjectile() { };
	~CNailProjectile();

	Class_T Classify(void) { return CLASS_NONE; }

public:
	void Spawn(void);
	void Precache(void);
	void BubbleThink(void);
	void BoltTouch(CBaseEntity *pOther);
	bool CreateVPhysics(void);
	unsigned int PhysicsSolidMaskForEntity() const;
	static CNailProjectile *BoltCreate(const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner = NULL);

protected:

	bool	CreateSprites(void);

	CHandle<CSprite>		m_pGlowSprite;
	//CHandle<CSpriteTrail>	m_pGlowTrail;

	int		m_iDamage;

	DECLARE_DATADESC();
	//DECLARE_SERVERCLASS();
};