static enum e_PLAYER_DATA
{
    bool:E_PLAYER_LOGGED,
    E_PLAYER_ATTEMPTS
};

static s_PlayerData[MAX_PLAYERS][e_PLAYER_DATA];

stock SetPlayerLogged(playerid, bool:status = false)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }
    s_PlayerData[playerid][E_PLAYER_LOGGED] = status;
    return 1;
}

stock HasPlayerLogged(playerid)
{   
    if (!IsPlayerConnected(playerid)) {
        return -1;
    }
    return s_PlayerData[playerid][E_PLAYER_LOGGED];
}

stock SetPlayerLoginAttempts(playerid, attempts)
{
    s_PlayerData[playerid][E_PLAYER_ATTEMPTS] = attempts;
    return 1;
}

stock GetPlayerLoginAttempts(playerid)
{
    return s_PlayerData[playerid][E_PLAYER_ATTEMPTS];
}
