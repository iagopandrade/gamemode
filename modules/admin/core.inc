#if defined _INC_admin_core
	#endinput
#endif
#define _INC_admin_core

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if (GetPlayerInterior(playerid) != 0) {
        GameTextForPlayer(playerid, "~r~Voce esta dentro de um Interior!", 3000, 3);
        return 1;
    }
    SetPlayerPos(playerid, fX, fY, fZ);
    return 1;
}
