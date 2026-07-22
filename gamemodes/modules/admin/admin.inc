public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if (AdminDuty[playerid]) {
        if (GetPlayerInterior(playerid) != 0) {
            GameTextForPlayer(playerid, "~r~Voce esta dentro de um Interior!", 3000, 3);
            return 1;
        }
        SetPlayerPos(playerid, fX, fY, fZ);
    }
    return 1;
}