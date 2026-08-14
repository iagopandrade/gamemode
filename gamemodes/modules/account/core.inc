#if defined _INC_account_core
	#endinput
#endif
#define _INC_account_core

#include <pp-hooks>

static enum e_ACCOUNT_DATA
{
    E_ACCOUNT_ID,
    E_ACCOUNT_ADMIN
};

static s_AccountData[MAX_PLAYERS][e_ACCOUNT_DATA];
new s_AccountPasswordHash[MAX_PLAYERS][HASH_LEN]; // *

stock GetAccountPasswordHash(playerid)
{
    return s_AccountPasswordHash[playerid];
}

stock GetAccountId(playerid)
{
    return s_AccountData[playerid][E_ACCOUNT_ID];
}

stock SetAccountId(playerid, id)
{
    s_AccountData[playerid][E_ACCOUNT_ID] = id;
    return 1;
}

stock GetAccountAdminLevel(playerid)
{
    return s_AccountData[playerid][E_ACCOUNT_ADMIN];
}

stock SetAccountAdminLevel(playerid, level)
{
    s_AccountData[playerid][E_ACCOUNT_ADMIN] = level;
    return 1;
}

stock HashPlayerPassword(playerid, const password[], hashOut[HASH_LEN])
{
    new playerName[MAX_PLAYER_NAME];
    new salted[64 + MAX_PLAYER_NAME];

    GetPlayerName(playerid, playerName, sizeof(playerName));
    format(salted, sizeof(salted), "%s%s", password, playerName);

    WP_Hash(hashOut, HASH_LEN, salted);
    return 1;
}

stock HashPlayerIp(playerid, hashOut[HASH_LEN])
{
    new playerIp[16];

    GetPlayerIp(playerid, playerIp, sizeof(playerIp));
    WP_Hash(hashOut, HASH_LEN, playerIp);
    return 1;
}

stock ValidateAccountPassword(playerid, const password[])
{
    new hash[HASH_LEN];

    HashPlayerPassword(playerid, password, hash);

    if (!strcmp(hash, s_AccountPasswordHash[playerid])) {
        OnAccountLoginSuccess(playerid);
    } else {
        OnAccountLoginFailed(playerid);
    }
    return 1;
}

stock OnAccountLoginSuccess(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    new ipHash[HASH_LEN];

    SetPlayerLogged(playerid, true);
    SetPlayerLoginAttempts(playerid, 0);

    HashPlayerIp(playerid, ipHash);

    LoginBlockReset(playerid);
    AccountUpdateLoginSuccess(GetAccountId(playerid), ipHash);

    SelectCharacterForAccount(playerid);

    SendWelcomeMessage(playerid);
    return 1;
}

stock OnAccountLoginFailed(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    new message[128];

    LoginBlockRegisterFailure(playerid);

    new remainingAttempts = GetLoginBlockRemainingAttempts(playerid);

    if (remainingAttempts <= 0) {
        KickBlockedIp(playerid, LOGIN_BLOCK_TIME);
        return 1;
    }

    format(message, sizeof(message),
        "Usuario ou senha invalidos.\nVoce tem mais %d tentativa(s) antes do bloqueio.",
        remainingAttempts
    );

    ShowLoginDialog(playerid, message);
    return 1;
}

stock KickBlockedIp(playerid, remainingSeconds)
{
    new message[128];

    format(message, sizeof(message),
        "Seu IP esta temporariamente bloqueado para novas tentativas de login.\nTente novamente em %d minuto(s).",
        (remainingSeconds + 59) / 60
    );
    SendClientMessage(playerid, COLOR_LIGHTRED, message);

    SetTimerEx("DelayedKick", 500, false, "d", playerid);
    return 1;
}

forward DelayedKick(playerid);
public DelayedKick(playerid)
{
    Kick(playerid);
    return 1;
}

stock CreateAccount(playerid)
{
    new ipHash[HASH_LEN];

    HashPlayerIp(playerid, ipHash);

    AccountCreate(playerid, s_AccountPasswordHash[playerid], ipHash);
    return 1;
}

stock SaveAccount(playerid)
{
    AccountSave(playerid);
    return 1;
}

stock SendWelcomeMessage(playerid)
{
    SendClientMessage(playerid, COLOR_SOFT_YELLOW, "__________________________________________________________");
    SendClientMessage(playerid, COLOR_EMERALD,
        EMBED_EMERALD "B" EMBED_NEPHRITE "P" EMBED_EMERALD "C " EMBED_NEPHRITE "P" EMBED_EMERALD "l" EMBED_NEPHRITE "a" EMBED_EMERALD "y" EMBED_NEPHRITE "e" EMBED_EMERALD "r" EMBED_NEPHRITE "'" EMBED_EMERALD "s " EMBED_SUNFLOWER "C" EMBED_ORANGE "o" EMBED_SUNFLOWER "m" EMBED_ORANGE "m" EMBED_SUNFLOWER "u" EMBED_ORANGE "n" EMBED_SUNFLOWER "i" EMBED_ORANGE "t" EMBED_SUNFLOWER "y"
    );
    SendClientMessage(playerid, COLOR_WHITE,
        EMBED_SPRINGGREEN "| " EMBED_WHITE "Login efetuado com sucesso! Seja bem-vindo ao " EMBED_SUNFLOWER "BPC Player's Community" EMBED_WHITE "."
    );
    SendClientMessage(playerid, COLOR_WHITE, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
    SendClientMessage(playerid, COLOR_WHITE, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
    SendClientMessage(playerid, COLOR_WHITE, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
    SendClientMessage(playerid, COLOR_SOFT_YELLOW, "__________________________________________________________");
    return 1;
}

forward OnAccountLoadResult(playerid);
public OnAccountLoadResult(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    new rows;
    cache_get_row_count(rows);
    
    if (!rows) {
        ShowRegisterDialog(
            playerid,
            "Esta conta ainda nao existe.\nDigite uma senha para cria-la:"
        );
        return 1;
    }

    new adminLevel;

    cache_get_value_name_int(0, "id", s_AccountData[playerid][E_ACCOUNT_ID]);
    cache_get_value_name_int(0, "admin", adminLevel);
    s_AccountData[playerid][E_ACCOUNT_ADMIN] = bool:(adminLevel != 0);

    cache_get_value_name(
        0,
        "password",
        s_AccountPasswordHash[playerid],
        HASH_LEN
    );

    ShowLoginDialog(
        playerid,
        "Bem-vindo de volta!\nDigite sua senha para entrar:"
    );
    return 1;
}

forward OnAccountCreateResult(playerid);
public OnAccountCreateResult(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    SetAccountId(playerid, cache_insert_id());
    SetAccountAdminLevel(playerid, false);

    SetPlayerLogged(playerid, true);
    SetPlayerLoginAttempts(playerid, 0);

    LoginBlockReset(playerid);

    SelectCharacterForAccount(playerid);

    SendWelcomeMessage(playerid);
    return 1;
}

hook OnGameModeExit()
{
    foreach (new i : Player) {
        if (IsPlayerConnected(i) && HasPlayerLogged(i)) {
            SaveAccount(i);
            SaveCharacter(i);
        }
    }

    DisconnectFromDatabase();
}

hook OnPlayerConnect(playerid)
{    
    SetPlayerLogged(playerid, false);
    SetPlayerLoginAttempts(playerid, 0);

    LoginBlockCheck(playerid);
}

forward OnLoginBlockChecked(playerid, bool:blocked, remainingSeconds);
public OnLoginBlockChecked(playerid, bool:blocked, remainingSeconds)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    if (blocked) {
        KickBlockedIp(playerid, remainingSeconds);
        return 1;
    }

    AccountLoad(playerid);
    return 1;
}

hook OnPlayerRequestClass(playerid, classid)
{
    if (IsPlayerNPC(playerid)) {
        return 1;
    }

    TogglePlayerSpectating(playerid, true);
    return 1;
}

hook OnPlayerRequestSpawn(playerid)
{
    return 0;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (HasPlayerLogged(playerid)) {
        SaveAccount(playerid);
        SaveCharacter(playerid);
    }

    SetPlayerLogged(playerid, false);
}
