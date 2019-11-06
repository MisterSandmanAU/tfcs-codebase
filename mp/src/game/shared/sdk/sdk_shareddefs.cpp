//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include "cbase.h"
#include "weapon_sdkbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// the 1 / 2 / 3 respectively are all identical in our template mod to start, I've made the base ones (pc_class1, pc_class2, pc_class3) and then duplicated them for the teams.
//Tony;  for our template we have two versions.
#if defined ( SDK_USE_PLAYERCLASSES ) && defined ( SDK_USE_TEAMS )
const char *pszTeamBlueClasses[] = 
{
	"scout",
	"soldier",
	"pyro",
	"hwguy",
	"demoman",
	"engineer",
	"medic",
	"sniper",
	"spy",
	"civilian",
	NULL
};

const char *pszTeamRedClasses[] = 
{
	"scout",
	"soldier",
	"pyro",
	"hwguy",
	"demoman",
	"engineer",
	"medic",
	"sniper",
	"spy",
	"civilian",
	NULL
};

const char *pszTeamClasses[] =
{
	"scout",
	"soldier",
	"pyro",
	"hwguy",
	"demoman",
	"engineer",
	"medic",
	"sniper",
	"spy",
	"civilian",
	NULL
};

const char *pszTeamYellowClasses[] =
{
	"scout",
	"soldier",
	"pyro",
	"hwguy",
	"demoman",
	"engineer",
	"medic",
	"sniper",
	"spy",
	"civilian",
	NULL
};

const char *pszTeamGreenClasses[] =
{
	"scout",
	"soldier",
	"pyro",
	"hwguy",
	"demoman",
	"engineer",
	"medic",
	"sniper",
	"spy",
	"civilian",
	NULL
};
ConVar	mp_limit_blue_class1(		"mp_limit_blue_class1", "-1", FCVAR_REPLICATED, "Class limit for Blue class 1" );
ConVar	mp_limit_blue_class2(		"mp_limit_blue_class2", "-1", FCVAR_REPLICATED, "Class limit for Blue class 2" );
ConVar	mp_limit_blue_class3(		"mp_limit_blue_class3", "-1", FCVAR_REPLICATED, "Class limit for Blue class 3" );

ConVar	mp_limit_red_class1(		"mp_limit_red_class1", "-1", FCVAR_REPLICATED, "Class limit for Red class 1" );
ConVar	mp_limit_red_class2(		"mp_limit_red_class2", "-1", FCVAR_REPLICATED, "Class limit for Red class 2" );
ConVar	mp_limit_red_class3(		"mp_limit_red_class3", "-1", FCVAR_REPLICATED, "Class limit for Red class 3" );

//Tony; not using teams, but we are using classes
#elif defined ( SDK_USE_PLAYERCLASSES ) && !defined( SDK_USE_TEAMS )
const char *pszPlayerClasses[] =
{
	"pc_class1",
	"pc_class2",
	"pc_class3",
	NULL
};
ConVar	mp_limit_pc_class1(		"mp_limit_pc_class1", "-1", FCVAR_REPLICATED, "Class limit for class 1" );
ConVar	mp_limit_pc_class2(		"mp_limit_pc_class2", "-1", FCVAR_REPLICATED, "Class limit for class 2" );
ConVar	mp_limit_pc_class3(		"mp_limit_pc_class3", "-1", FCVAR_REPLICATED, "Class limit for class 3" );
#endif

const char *pszTeamNames[] =
{
	"#SDK_Team_Unassigned",
	"#SDK_Team_Spectator",
#if defined ( SDK_USE_TEAMS )
	"#SDK_Team_Blue",
	"#SDK_Team_Red",
	"#SDK_Team_Yellow",
	"#SDK_Team_Green",
#endif
};

//Tony; We need to precache all possible player models that we're going to use
const char *pszPossiblePlayerModels[] =
{
	SDK_PLAYER_MODEL,
	"models/player/blue_player.mdl",
	"models/player/red_player.mdl",
	"models/player/scout.mdl",
	"models/player/soldier.mdl",
	"models/player/pyro.mdl",
	"models/player/hwguy.mdl",
	"models/player/demo.mdl",
	"models/player/engineer.mdl",
	"models/player/medic.mdl",
	"models/player/sniper.mdl",
	"models/player/spy.mdl",
	"models/player/civilian.mdl",
	NULL
};

// ----------------------------------------------------------------------------- //
// Global Weapon Definitions
// ----------------------------------------------------------------------------- //

//--------------------------------------------------------------------------------------------------------
static const char * s_WeaponAliasInfo[] = 
{
	"none",		// WEAPON_NONE
	"mp5",		// SDK_WEAPON_MP5
	"shotgun",	// SDK_WEAPON_SHOTGUN
	"12gauge",	// SDK_WEAPON_SHOTGUN12GAUGE
	"nailgun",	// SDK_WEAPON_NAILGUN
	"grenade",	// SDK_WEAPON_GRENADE
	"pistol",	// SDK_WEAPON_PISTOL
	"crowbar",	// SDK_WEAPON_CROWBAR
	"umbrella",	// SDK_WEAPON_UMBRELLA
	"supernailgun",	// SDK_WEAPON_SUPERNAILGUN
	"knife",	// SDK_WEAPON_KNIFE
	"wrench",	// SDK_WEAPON_WRENCH
	"ac",	// SDK_WEAPON_AC
	"medkit", //SDK_WEAPON_MEDKIT
	NULL,		// WEAPON_NONE
};

//--------------------------------------------------------------------------------------------------------
//
// Given an alias, return the associated weapon ID
//
int AliasToWeaponID( const char *alias )
{
	if (alias)
	{
		for( int i=0; s_WeaponAliasInfo[i] != NULL; ++i )
			if (!Q_stricmp( s_WeaponAliasInfo[i], alias ))
				return i;
	}

	return WEAPON_NONE;
}

//--------------------------------------------------------------------------------------------------------
//
// Given a weapon ID, return its alias
//
const char *WeaponIDToAlias( int id )
{
	if ( (id >= WEAPON_MAX) || (id < 0) )
		return NULL;

	return s_WeaponAliasInfo[id];
}


