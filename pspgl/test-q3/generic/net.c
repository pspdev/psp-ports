/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "../quake3-1.32b/code/game/q_shared.h"
#include "../quake3-1.32b/code/qcommon/qcommon.h"


qboolean Sys_StringToAdr (const char *s, netadr_t *a)
{
	printf("%s (%d): %s\n", __FUNCTION__, __LINE__, s);
	return qfalse;
}


qboolean Sys_IsLANAddress (netadr_t adr)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return qfalse;
}


void Sys_ShowIP (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void Sys_SendPacket (int length, const void *data, netadr_t to)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}

/* Never called by the game logic, just the system event queing */
qboolean Sys_GetPacket (netadr_t *net_from, msg_t *net_message)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return qfalse;
}


void NET_Sleep (int msec)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}

