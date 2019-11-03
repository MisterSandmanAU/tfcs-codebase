//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "sdk_playerclass_info_parse.h"
#include "weapon_sdkbase.h"
#include <KeyValues.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//Tony; due to the nature of the base code.. I must do this !

FilePlayerClassInfo_t* CreatePlayerClassInfo()
{
#if defined ( SDK_USE_PLAYERCLASSES )
	return new CSDKPlayerClassInfo;
#else
	return new FilePlayerClassInfo_t;
#endif
}

#if defined ( SDK_USE_PLAYERCLASSES )

CSDKPlayerClassInfo::CSDKPlayerClassInfo()
{
	m_iTeam				= TEAM_UNASSIGNED;
	
	m_iPrimaryWeapon	= WEAPON_NONE;
	m_iSecondaryWeapon	= WEAPON_NONE;
	m_iMeleeWeapon		= WEAPON_NONE;
	
	m_iNumGrensType1	= 0;
	m_iGrenType1		= WEAPON_NONE;

	m_iNumGrensType2	= 0;
	m_iGrenType2		= WEAPON_NONE;

	m_szLimitCvar[0]	= '\0';
	m_flRunSpeed		= SDK_DEFAULT_PLAYER_RUNSPEED;
	m_flSprintSpeed		= SDK_DEFAULT_PLAYER_SPRINTSPEED;
	m_flProneSpeed		= SDK_DEFAULT_PLAYER_PRONESPEED;

	m_iArmor			= 0;
}

void CSDKPlayerClassInfo::Parse( KeyValues *pKeyValuesData, const char *szWeaponName )
{
	BaseClass::Parse( pKeyValuesData, szWeaponName );

	m_iTeam= pKeyValuesData->GetInt( "team", TEAM_UNASSIGNED );

	// Figure out what team can have this player class
	m_iTeam = TEAM_UNASSIGNED;

//Tony; don't check for teams unless we're using teams. You could do a free for all, but class / character based game if you wanted.
#ifdef SDK_USE_TEAMS
	const char *pTeam = pKeyValuesData->GetString( "team", NULL );
	if ( pTeam )
	{
		if ( Q_stricmp( pTeam, "BLUE" ) == 0 )
		{
			m_iTeam = SDK_TEAM_BLUE;
		}
		else if ( Q_stricmp( pTeam, "RED" ) == 0 )
		{
			m_iTeam = SDK_TEAM_RED;
		}
		else
		{
			Assert( false );
		}
	}
	else
	{
		Assert( false );
	}
#endif

	const char *pszPrimaryWeapon = pKeyValuesData->GetString( "primaryweapon", NULL );
	m_iPrimaryWeapon = AliasToWeaponID( pszPrimaryWeapon );
	Assert( m_iPrimaryWeapon != WEAPON_NONE );	// require player to have a primary weapon

	const char *pszSecondaryWeapon = pKeyValuesData->GetString( "secondaryweapon", NULL );

	if ( pszSecondaryWeapon )
	{
        m_iSecondaryWeapon = AliasToWeaponID( pszSecondaryWeapon );
//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else 
		m_iSecondaryWeapon = WEAPON_NONE;

	const char *pszMeleeWeapon = pKeyValuesData->GetString( "meleeweapon", NULL );
	if ( pszMeleeWeapon )
	{
		m_iMeleeWeapon = AliasToWeaponID( pszMeleeWeapon );
//        Assert( m_iMeleeWeapon != WEAPON_NONE );
	}
	else
		m_iMeleeWeapon = WEAPON_NONE;

	const char *pszWeapon1 = pKeyValuesData->GetString("weapon1", NULL);

	if (pszWeapon1)
	{
		m_iWeapon1 = AliasToWeaponID(pszWeapon1);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon1 = WEAPON_NONE;

	const char *pszWeapon2 = pKeyValuesData->GetString("weapon2", NULL);

	if (pszWeapon2)
	{
		m_iWeapon2 = AliasToWeaponID(pszWeapon2);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon2 = WEAPON_NONE;

	const char *pszWeapon3 = pKeyValuesData->GetString("weapon3", NULL);

	if (pszWeapon3)
	{
		m_iWeapon3 = AliasToWeaponID(pszWeapon3);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon3 = WEAPON_NONE;

	const char *pszWeapon4 = pKeyValuesData->GetString("weapon4", NULL);

	if (pszWeapon4)
	{
		m_iWeapon4 = AliasToWeaponID(pszWeapon4);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon4 = WEAPON_NONE;

	const char *pszWeapon5 = pKeyValuesData->GetString("weapon5", NULL);

	if (pszWeapon5)
	{
		m_iWeapon5 = AliasToWeaponID(pszWeapon5);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon5 = WEAPON_NONE;

	const char *pszWeapon6 = pKeyValuesData->GetString("weapon6", NULL);

	if (pszWeapon6)
	{
		m_iWeapon6 = AliasToWeaponID(pszWeapon6);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon6 = WEAPON_NONE;

	const char *pszWeapon7 = pKeyValuesData->GetString("weapon7", NULL);

	if (pszWeapon7)
	{
		m_iWeapon7 = AliasToWeaponID(pszWeapon7);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon7 = WEAPON_NONE;

	const char *pszWeapon8 = pKeyValuesData->GetString("weapon8", NULL);

	if (pszWeapon8)
	{
		m_iWeapon8 = AliasToWeaponID(pszWeapon8);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon8 = WEAPON_NONE;

	const char *pszWeapon9 = pKeyValuesData->GetString("weapon9", NULL);

	if (pszWeapon9)
	{
		m_iWeapon9 = AliasToWeaponID(pszWeapon9);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon9 = WEAPON_NONE;

	const char *pszWeapon10 = pKeyValuesData->GetString("weapon10", NULL);

	if (pszWeapon10)
	{
		m_iWeapon10 = AliasToWeaponID(pszWeapon10);
		//		Assert( m_iSecondaryWeapon != WEAPON_NONE );
	}
	else
		m_iWeapon10 = WEAPON_NONE;

	m_iNumGrensType1 = pKeyValuesData->GetInt( "numgrens", 0 );
	if ( m_iNumGrensType1 > 0 )
	{
		const char *pszGrenType1 = pKeyValuesData->GetString( "grenadetype", NULL );
		m_iGrenType1 = AliasToWeaponID( pszGrenType1 );
//		Assert( m_iGrenType1 != WEAPON_NONE );
	}

	m_iNumGrensType2 = pKeyValuesData->GetInt( "numgrens2", 0 );
	if ( m_iNumGrensType2 > 0 )
	{
		const char *pszGrenType2 = pKeyValuesData->GetString( "grenadetype2", NULL );
		m_iGrenType2 = AliasToWeaponID( pszGrenType2 );
//		Assert( m_iGrenType2 != WEAPON_NONE );
	}

	Q_strncpy( m_szLimitCvar, pKeyValuesData->GetString( "limitcvar", "!! Missing limit cvar on Player Class" ), sizeof(m_szLimitCvar) );

	Assert( Q_strlen( m_szLimitCvar ) > 0 && "Every class must specify a limitcvar" );

	// HUD player status health images (when the player is hurt)
	Q_strncpy( m_szClassImage, pKeyValuesData->GetString( "classimage", "white" ), sizeof( m_szClassImage ) );
	Q_strncpy( m_szClassImageBG, pKeyValuesData->GetString( "classimagebg", "white" ), sizeof( m_szClassImageBG ) );

	m_flRunSpeed		= pKeyValuesData->GetFloat( "RunSpeed", SDK_DEFAULT_PLAYER_RUNSPEED );
	m_flSprintSpeed		= pKeyValuesData->GetFloat( "SprintSpeed", SDK_DEFAULT_PLAYER_SPRINTSPEED );
	m_flProneSpeed		= pKeyValuesData->GetFloat( "ProneSpeed", SDK_DEFAULT_PLAYER_PRONESPEED );

	m_iArmor			= pKeyValuesData->GetInt( "armor", 0 );

}
#endif // SDK_USE_PLAYERCLASSES