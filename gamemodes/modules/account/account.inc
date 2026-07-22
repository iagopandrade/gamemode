#define MYSQL_HOST              "127.0.0.1"
#define MYSQL_USER              "root"
#define MYSQL_PASS              ""
#define MYSQL_DB                "players"

#define MAX_LOGIN_ATTEMPTS      3
#define LOGIN_BLOCK_TIME        60     // tempo de bloqueio em segundos
#define MIN_PASSWORD_LEN        4
#define HASH_LEN                129

static enum e_ACCOUNT_DATA
{
    E_ACCOUNT_ID,
    E_ACCOUNT_BLOCKED_UNTIL,
    E_ACCOUNT_ADMIN
};

static s_AccountData[MAX_PLAYERS][e_ACCOUNT_DATA];
new s_AccountPasswordHash[MAX_PLAYERS][HASH_LEN]; // *
static MySQL:g_SQL;

public OnGameModeInit()
{
    ConnectToDatabase();
    return 1;
}

public OnGameModeExit()
{
    foreach (new i : Player) {
        if (IsPlayerConnected(i)) {
            SavePlayerAccount(i);
        }
    }
    mysql_close(g_SQL);
    return 1;
}

hook OnPlayerConnect@2(playerid)
{
    SetPlayerLogged(playerid, false);
    SetPlayerLoginAttempts(playerid, 0);

    LoadPlayerAccount(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if (IsPlayerNPC(playerid)) {
		return 1;
	}
   
    TogglePlayerSpectating(playerid, true);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    // Debug
	printf("Player %d requested spawn", playerid);
    return 0;
}

public OnPlayerSpawn(playerid)
{
    if (HasPlayerLogged(playerid)) {
        GivePlayerMoney(playerid, GetCharacterCash(playerid));
        SetPlayerScore(playerid, GetCharacterScore(playerid));
        TogglePlayerControllable(playerid, true);
    }
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (HasPlayerLogged(playerid)) {
        SavePlayerAccount(playerid);
    }
    return 1;
}

stock ConnectToDatabase()
{
    new MySQLOpt:options = mysql_init_options();

    mysql_set_option(options, AUTO_RECONNECT, true);
    mysql_set_option(options, POOL_SIZE, 2);

    g_SQL = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB, options);

    if (mysql_errno(g_SQL) != 0) {
        print("[MySQL] Nao foi possivel conectar ao banco de dados.");
    } else {
        print("[MySQL] Conexao estabelecida com sucesso.");
    }
    return 1;
}

stock LoadPlayerAccount(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[128];

    GetPlayerName(playerid, playerName, sizeof(playerName));
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `players` WHERE `name` = '%e' LIMIT 1", playerName);

    mysql_tquery(g_SQL, query, "OnAccountDataLoaded", "d", playerid);
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

stock ValidatePlayerPassword(playerid, const password[])
{
    new hash[HASH_LEN];

    HashPlayerPassword(playerid, password, hash);

    if (!strcmp(hash, s_AccountPasswordHash[playerid])) {
        OnLoginSuccess(playerid);
    } else {
        OnLoginFailed(playerid);
    }
    return 1;
}

stock OnLoginSuccess(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    new ipHash[HASH_LEN];
    new query[256];

    SetPlayerLogged(playerid, true);
    SetPlayerLoginAttempts(playerid, 0);

    s_AccountData[playerid][E_ACCOUNT_BLOCKED_UNTIL] = 0;

    HashPlayerIp(playerid, ipHash);

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `login_attempts` = 0, `blocked_until` = 0, `ip` = '%e' WHERE `id` = %d",
        ipHash,
        s_AccountData[playerid][E_ACCOUNT_ID]
    );

    mysql_tquery(g_SQL, query);

    SpawnPlayerFromAccount(playerid);

    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    SendClientMessage(playerid, COLOR_EMERALD,
        EMBED_EMERALD "B" EMBED_NEPHRITE "P" EMBED_EMERALD "C " EMBED_NEPHRITE "P" EMBED_EMERALD "l" EMBED_NEPHRITE "a" EMBED_EMERALD "y" EMBED_NEPHRITE "e" EMBED_EMERALD "r" EMBED_NEPHRITE "'" EMBED_EMERALD "s " EMBED_SUNFLOWER "C" EMBED_ORANGE "o" EMBED_SUNFLOWER "m" EMBED_ORANGE "m" EMBED_SUNFLOWER "u" EMBED_ORANGE "n" EMBED_SUNFLOWER "i" EMBED_ORANGE "t" EMBED_SUNFLOWER "y"
    );
    SendClientMessage(playerid, COLOR_WHITE, 
        EMBED_SPRINGGREEN "| " EMBED_WHITE "Login efetuado com sucesso! Seja bem-vindo ao " EMBED_SUNFLOWER "BPC Player's Community" EMBED_WHITE "."
    );
    SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    return 1;
}

stock OnLoginFailed(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 0;
    }

    new query[160];
    new message[128];

    new attempts = GetPlayerLoginAttempts(playerid);

    SetPlayerLoginAttempts(playerid, ++attempts);

    if (attempts >= MAX_LOGIN_ATTEMPTS) {
        BlockPlayerAccount(playerid);
        return 1;
    }

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `login_attempts` = %d WHERE `id` = %d",
        attempts,
        s_AccountData[playerid][E_ACCOUNT_ID]
    );

    mysql_tquery(g_SQL, query);

    format(message, sizeof(message),
        "Usuario ou senha invalidos.\nVoce tem mais %d tentativa(s) antes do bloqueio.",
        MAX_LOGIN_ATTEMPTS - attempts
    );

    ShowLoginDialog(playerid, message);
    return 1;
}

stock BlockPlayerAccount(playerid)
{
    new blockedUntil = gettime() + LOGIN_BLOCK_TIME;
    new query[160];
    new message[128];
    new attempts = GetPlayerLoginAttempts(playerid);

    s_AccountData[playerid][E_ACCOUNT_BLOCKED_UNTIL] = blockedUntil;

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `login_attempts` = %d, `blocked_until` = %d WHERE `id` = %d",
        attempts, blockedUntil, s_AccountData[playerid][E_ACCOUNT_ID]);
    mysql_tquery(g_SQL, query);

    format(message, sizeof(message), "Muitas tentativas incorretas.\nSua conta foi bloqueada por %d minutos.", (LOGIN_BLOCK_TIME + 59) / 60);
    SendClientMessage(playerid, 0xFF4444FF, message);

    SetTimerEx("DelayedKick", 500, false, "d", playerid);
    return 1;
}

stock KickBlockedPlayer(playerid, remainingSeconds)
{
    new message[128];

    format(message, sizeof(message), "Sua conta esta temporariamente bloqueada.\nTente novamente em %d minuto(s).", (remainingSeconds + 59) / 60);
    SendClientMessage(playerid, 0xFF4444FF, message);

    SetTimerEx("DelayedKick", 500, false, "d", playerid);
    return 1;
}

forward DelayedKick(playerid);
public DelayedKick(playerid)
{
    Kick(playerid);
    return 1;
}

stock CreatePlayerAccount(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new ipHash[HASH_LEN];
    new query[400];

    GetPlayerName(playerid, playerName, sizeof(playerName));
    HashPlayerIp(playerid, ipHash);

    mysql_format(g_SQL, query, sizeof(query),
        "INSERT INTO `players` (`name`, `password`, `ip`, `cash`, `score`) VALUES ('%e', '%e', '%e', 0, 0)",
        playerName, s_AccountPasswordHash[playerid], ipHash);

    mysql_tquery(g_SQL, query, "OnAccountRegistered", "d", playerid);
    return 1;
}

stock SpawnPlayerFromAccount(playerid)
{
    new skin = GetCharacterSkin(playerid);

    if (skin == 0) {
        SetSpawnInfo(
            playerid, NO_TEAM,
            1,
            1723.0403, -1876.3651, 13.5646, 0.0, 
            WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0
        );
    } else {
        new Float:x, Float:y, Float:z, Float:ang;
        GetCharacterPos(playerid, x, y, z, ang);
        new interior = GetCharacterInterior(playerid);
        new world = GetCharacterWorld(playerid);

        SetSpawnInfo(
            playerid,
            NO_TEAM,
            skin,
            x, y, z, ang,
            WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0
        );

        SetPlayerInterior(playerid, interior);
        SetPlayerVirtualWorld(playerid, world);
    }

    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    return 1;
}

stock SavePlayerAccount(playerid)
{
    new Float:posX, Float:posY, Float:posZ, Float:posA;
    new query[320];

    GetPlayerPos(playerid, posX, posY, posZ);
    GetPlayerFacingAngle(playerid, posA);

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `cash` = %d, `score` = %d, `skin` = %d, `interior` = %d, `world` = %d, `pos_x` = '%f', `pos_y` = '%f', `pos_z` = '%f', `pos_a` = '%f' WHERE `id` = %d",
        GetPlayerMoney(playerid),
        GetPlayerScore(playerid),
        GetPlayerSkin(playerid),
        GetPlayerInterior(playerid),
        GetPlayerVirtualWorld(playerid),
        posX, posY, posZ, posA,
        s_AccountData[playerid][E_ACCOUNT_ID]);

    mysql_tquery(g_SQL, query);
    return 1;
}

forward OnAccountDataLoaded(playerid);
public OnAccountDataLoaded(playerid)
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

    new cash;
    new score;
    new level;
    new skin;
    new interior;
    new world;
    new attempts;

    new Float:x;
    new Float:y;
    new Float:z;
    new Float:angle;

    cache_get_value_name_int(0, "id", s_AccountData[playerid][E_ACCOUNT_ID]);
    cache_get_value_name_int(0, "admin", s_AccountData[playerid][E_ACCOUNT_ADMIN]);
    cache_get_value_name_int(0, "blocked_until", s_AccountData[playerid][E_ACCOUNT_BLOCKED_UNTIL]);

    cache_get_value_name_int(0, "cash", cash);
    cache_get_value_name_int(0, "score", score);
    cache_get_value_name_int(0, "level", level);
    cache_get_value_name_int(0, "skin", skin);
    cache_get_value_name_int(0, "interior", interior);
    cache_get_value_name_int(0, "world", world);
    cache_get_value_name_int(0, "login_attempts", attempts);

    cache_get_value_name_float(0, "pos_x", x);
    cache_get_value_name_float(0, "pos_y", y);
    cache_get_value_name_float(0, "pos_z", z);
    cache_get_value_name_float(0, "pos_a", angle);

    cache_get_value_name(
        0,
        "password",
        s_AccountPasswordHash[playerid],
        HASH_LEN
    );

    SetCharacterCash(playerid, cash);
    SetCharacterScore(playerid, score);
    SetCharacterLevel(playerid, level);
    SetCharacterSkin(playerid, skin);
    SetCharacterInterior(playerid, interior);
    SetCharacterWorld(playerid, world);
    SetCharacterPos(playerid, x, y, z, angle);

    SetPlayerLoginAttempts(playerid, attempts);

    if (s_AccountData[playerid][E_ACCOUNT_BLOCKED_UNTIL] > gettime()) {
        KickBlockedPlayer(
            playerid,
            s_AccountData[playerid][E_ACCOUNT_BLOCKED_UNTIL] - gettime()
        );
    } else {
        ShowLoginDialog(
            playerid,
            "Bem-vindo de volta!\nDigite sua senha para entrar:"
        );
    }
    return 1;
}

forward OnAccountRegistered(playerid);
public OnAccountRegistered(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    s_AccountData[playerid][E_ACCOUNT_ID] = cache_insert_id();

    SetPlayerLogged(playerid, true);
    SetPlayerLoginAttempts(playerid, 0);

    SetCharacterCash(playerid, 0);
    SetCharacterScore(playerid, 0);
    SetCharacterLevel(playerid, 0);

    SetCharacterSkin(playerid, 1);
    SetCharacterInterior(playerid, 0);
    SetCharacterWorld(playerid, 0);

    SetCharacterPos(
        playerid,
        1723.0403,
        -1876.3651,
        13.5646,
        0.0
    );

    SpawnPlayerFromAccount(playerid);

    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    SendClientMessage(playerid, 0x2ECC71FF, "{2ECC71}B{27AE60}P{2ECC71}C {27AE60}P{2ECC71}l{27AE60}a{2ECC71}y{27AE60}e{2ECC71}r{27AE60}'{2ECC71}s {F1C40F}C{F39C12}o{F1C40F}m{F39C12}m{F1C40F}u{F39C12}n{F1C40F}i{F39C12}t{F1C40F}y");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00FF7F}| {FFFFFF}Conta registrada com sucesso! Seja bem-vindo ao {F1C40F}BPC Player's Community{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    return 1;
}
