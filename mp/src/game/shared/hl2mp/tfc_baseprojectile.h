#include "cbase.h"
#include <iostream>
#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#include "c_te_effect_dispatch.h"
#else
#include "hl2mp_player.h"
#include "te_effect_dispatch.h"
#include "Sprite.h"
#endif
using namespace std;

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CTFCBaseprojectile : public CBaseCombatCharacter
{
	DECLARE_CLASS(CTFCBaseprojectile, CBaseCombatCharacter);

public:
	CTFCBaseprojectile() {};
	~CTFCBaseprojectile();
	
	Class_T Classify(void) { return CLASS_NONE; }
	

public:
	void Spawn(void);
	void Precache(void);
	void SetWorldModel(const char*  Model);
	void BubbleThink(void);
	void BoltTouch(CBaseEntity *pOther);
	bool CreateVPhysics(void);
	unsigned int PhysicsSolidMaskForEntity() const;
	static  CTFCBaseprojectile *ProjectileCreate(const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner = NULL);

	const char* WorldModel = "models/crossbow_bolt.mdl";

protected:

	bool	CreateSprites(void);

	CHandle<CSprite>		m_pGlowSprite;
	//CHandle<CSpriteTrail>	m_pGlowTrail;

	int		m_iDamage;

	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};