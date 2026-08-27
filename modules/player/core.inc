#if defined _INC_player_core
	#endinput
#endif
#define _INC_player_core

enum (<<= 1)
{
    CMD_ADMIN = 1, /*  0b00000000000000000000000000000001 */
    CMD_MODER,     /*  0b00000000000000000000000000000010 */
    CMD_JR_MODER,  /*  0b00000000000000000000000000000100 */
    CMD_ALIVE,     /*  0b00000000000000000000000000001000 */
    CMD_LOGGED     /*  0b00000000000000000000000000001000 */
};

static enum e_PLAYER_DATA
{
    bool:E_PLAYER_LOGGED,
    E_PLAYER_ATTEMPTS,
    E_PLAYER_PERMISSIONS
};

static s_PlayerData[MAX_PLAYERS][e_PLAYER_DATA];

#define HasPermission(%0,%1)    ((s_PlayerData[%0][E_PLAYER_PERMISSIONS] & (%1)) != 0)
#define AddPermission(%0,%1)    (s_PlayerData[%0][E_PLAYER_PERMISSIONS] |= (%1))
#define RemovePermission(%0,%1) (s_PlayerData[%0][E_PLAYER_PERMISSIONS] &= ~(%1))
#define TogglePermission(%0,%1) (s_PlayerData[%0][E_PLAYER_PERMISSIONS] ^= (%1))

stock SetPlayerLogged(playerid, bool:status = false)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }
    s_PlayerData[playerid][E_PLAYER_LOGGED] = status;

    AddPermission(playerid, CMD_ADMIN);
    AddPermission(playerid, CMD_MODER);
    AddPermission(playerid, CMD_JR_MODER);
    AddPermission(playerid, CMD_ALIVE);
    AddPermission(playerid, CMD_LOGGED);
    printf("[DEBUG]: Player[%d] Logged Status %d", playerid, status);
    return 1;
}

stock GetCharacterName(playerid, name[], len = sizeof(name))
{
    GetPlayerName(playerid, name, len);

    for (new i = 0; name[i] != '\0'; i++) {
        if (name[i] == '_') {
            name[i] = ' ';
        }
    }
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

static LastCommand[MAX_PLAYERS];

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
    if (!HasPlayerLogged(playerid)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Voce NAO esta logado!",
			5000, 16
		);
        return 0;
    }

    if (GetTickCount() - LastCommand[playerid] < 500) {
		GameTextForPlayer(playerid, "Aguarde um instante.", 3000, 3);
        return 0;
    }

    LastCommand[playerid] = GetTickCount();

    if (PC_CommandExists(cmd)) {
        if (!(flags & s_PlayerData[playerid][E_PLAYER_PERMISSIONS])) {
            /* Debug */
            printf("player %d doesn’t have access to command '%s'", playerid, cmd);
            return 0;
        }
    }
    return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
    if (result == -1) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Esse comando nao existe!",
            5000, 16
        );
        return 0;
    }
    return 1;
}
