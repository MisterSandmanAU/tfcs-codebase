//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#ifndef BOT_BASE_H
#define BOT_BASE_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

void Bot_add_team_2();
void Bot_add_team_3();

// If iTeam or iClass is -1, then a team or class is randomly chosen.
CBasePlayer *BotPutInServer( bool bFrozen, int iTeam );


#endif // BOT_BASE_H

