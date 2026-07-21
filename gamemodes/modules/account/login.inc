#define MYSQL_HOST              "127.0.0.1"
#define MYSQL_USER              "root"
#define MYSQL_PASS              ""
#define MYSQL_DB                "players"

#define MAX_LOGIN_ATTEMPTS      3
#define LOGIN_BLOCK_TIME        60     // tempo de bloqueio em segundos (1 minuto)
#define MIN_PASSWORD_LEN        4
#define HASH_LEN                129

static enum E_PLAYER_DATA
{
    E_PLAYER_DATA_ID,
    bool:E_PLAYER_DATA_LOGGED,
    E_PLAYER_DATA_ATTEMPTS,
    E_PLAYER_DATA_BLOCKED_UNTIL,
    E_PLAYER_DATA_CASH,
    E_PLAYER_DATA_SCORE,
    E_PLAYER_DATA_LEVEL,
    E_PLAYER_DATA_ADMIN,
    E_PLAYER_DATA_SKIN,
    E_PLAYER_DATA_INTERIOR,
    E_PLAYER_DATA_WORLD,
    Float:E_PLAYER_DATA_POS_X,
    Float:E_PLAYER_DATA_POS_Y,
    Float:E_PLAYER_DATA_POS_Z,
    Float:E_PLAYER_DATA_POS_A
}

new g_PlayerData[MAX_PLAYERS][E_PLAYER_DATA];
new g_PlayerPasswordHash[MAX_PLAYERS][HASH_LEN];
new MySQL:g_SQL;

SetPlayerLogin(playerid, bool:login = false)
{
    g_PlayerData[playerid][E_PLAYER_DATA_LOGGED] = login;
    return 1;
}

bool:IsPlayerLogged(playerid)
{
    return g_PlayerData[playerid][E_PLAYER_DATA_LOGGED];
}

public OnGameModeInit()
{
    ConnectToDatabase();
    return 1;
}

public OnGameModeExit()
{
    mysql_close(g_SQL);
    return 1;
}

hook OnPlayerConnect@2(playerid)
{
    g_PlayerData[playerid][E_PLAYER_DATA_LOGGED] = false;
    g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS] = 0;

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

public OnPlayerSpawn(playerid)
{
    if (g_PlayerData[playerid][E_PLAYER_DATA_LOGGED]) {
        GivePlayerMoney(playerid, g_PlayerData[playerid][E_PLAYER_DATA_CASH]);
        SetPlayerScore(playerid, g_PlayerData[playerid][E_PLAYER_DATA_SCORE]);
        TogglePlayerControllable(playerid, true);
    }
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (IsPlayerLogged(playerid)) {
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

stock ShowLoginDialog(playerid, const message[])
{
    Dialog_Show(playerid, "Login", DIALOG_STYLE_PASSWORD, "Login", message, "Entrar", "Sair");
    return 1;
}

stock ShowRegisterDialog(playerid, const message[])
{
    Dialog_Show(playerid, "Registro", DIALOG_STYLE_PASSWORD, "Criar conta", message, "Próximo", "Sair");
    return 1;
}

stock ShowRegisterConfirmDialog(playerid, const message[])
{
    Dialog_Show(playerid, "RegistroConfirmar", DIALOG_STYLE_PASSWORD, "Confirmar senha", message, "Confirmar", "Sair");
    return 1;
}

stock ValidatePlayerPassword(playerid, const password[])
{
    new hash[HASH_LEN];

    HashPlayerPassword(playerid, password, hash);

    if (!strcmp(hash, g_PlayerPasswordHash[playerid]))
    {
        OnLoginSuccess(playerid);
    }
    else
    {
        OnLoginFailed(playerid);
    }
    return 1;
}

stock OnLoginSuccess(playerid)
{
    new ipHash[HASH_LEN];
    new query[256];

    g_PlayerData[playerid][E_PLAYER_DATA_LOGGED] = true;
    g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS] = 0;

    HashPlayerIp(playerid, ipHash);
    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `login_attempts` = 0, `blocked_until` = 0, `ip` = '%e' WHERE `id` = %d",
        ipHash, g_PlayerData[playerid][E_PLAYER_DATA_ID]);
    mysql_tquery(g_SQL, query);

    SetPlayerLogin(playerid, true);
    SpawnPlayerFromAccount(playerid);

    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    SendClientMessage(playerid, 0x2ECC71FF, "{2ECC71}B{27AE60}r{2ECC71}a{27AE60}s{2ECC71}i{27AE60}l {F1C40F}C{F39C12}i{F1C40F}t{F39C12}y {FFFFFF}Roleplay");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00FF7F}| {FFFFFF}Login efetuado com sucesso! Seja bem-vindo ao {F1C40F}Brasil City{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{1ABC9C}| {FFFFFF}Para parar a musica utilize o comando {F1C40F}/pararmusica{FFFFFF}.");
    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    return 1;
}

stock OnLoginFailed(playerid)
{
    new query[160];

    g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS]++;

    if (g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS] >= MAX_LOGIN_ATTEMPTS) {
        BlockPlayerAccount(playerid);
    } else {
        new remaining = MAX_LOGIN_ATTEMPTS - g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS];
        new message[128];

        mysql_format(g_SQL, query, sizeof(query),
            "UPDATE `players` SET `login_attempts` = %d WHERE `id` = %d",
            g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS], g_PlayerData[playerid][E_PLAYER_DATA_ID]);
        mysql_tquery(g_SQL, query);

        format(message, sizeof(message), "Usuário ou senha inválidos.\nVocê tem mais %d tentativa(s) antes do bloqueio.", remaining);
        ShowLoginDialog(playerid, message);
    }
    return 1;
}

stock BlockPlayerAccount(playerid)
{
    new blockedUntil = gettime() + LOGIN_BLOCK_TIME;
    new query[160];
    new message[128];

    g_PlayerData[playerid][E_PLAYER_DATA_BLOCKED_UNTIL] = blockedUntil;

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `players` SET `login_attempts` = %d, `blocked_until` = %d WHERE `id` = %d",
        g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS], blockedUntil, g_PlayerData[playerid][E_PLAYER_DATA_ID]);
    mysql_tquery(g_SQL, query);

    format(message, sizeof(message), "Muitas tentativas incorretas.\nSua conta foi bloqueada por %d minutos.", (LOGIN_BLOCK_TIME + 59) / 60);
    SendClientMessage(playerid, 0xFF4444FF, message);

    SetTimerEx("DelayedKick", 500, false, "d", playerid);
    return 1;
}

stock KickBlockedPlayer(playerid, remainingSeconds)
{
    new message[128];

    format(message, sizeof(message), "Sua conta está temporariamente bloqueada.\nTente novamente em %d minuto(s).", (remainingSeconds + 59) / 60);
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
        playerName, g_PlayerPasswordHash[playerid], ipHash);

    mysql_tquery(g_SQL, query, "OnAccountRegistered", "d", playerid);
    return 1;
}

stock SpawnPlayerFromAccount(playerid)
{
    if (g_PlayerData[playerid][E_PLAYER_DATA_SKIN] == 0) {
        SetSpawnInfo(playerid, NO_TEAM, 0, 1723.0403, -1876.3651, 13.5646, 0.0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
    } else {
        SetSpawnInfo(playerid, NO_TEAM, g_PlayerData[playerid][E_PLAYER_DATA_SKIN],
            g_PlayerData[playerid][E_PLAYER_DATA_POS_X],
            g_PlayerData[playerid][E_PLAYER_DATA_POS_Y],
            g_PlayerData[playerid][E_PLAYER_DATA_POS_Z],
            g_PlayerData[playerid][E_PLAYER_DATA_POS_A], WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);

        SetPlayerInterior(playerid, g_PlayerData[playerid][E_PLAYER_DATA_INTERIOR]);
        SetPlayerVirtualWorld(playerid, g_PlayerData[playerid][E_PLAYER_DATA_WORLD]);
    }

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
        g_PlayerData[playerid][E_PLAYER_DATA_ID]);

    mysql_tquery(g_SQL, query);
    return 1;
}

forward OnAccountDataLoaded(playerid);
public OnAccountDataLoaded(playerid)
{
    new rows, fields;

    cache_get_row_count(rows);
    cache_get_field_count(fields);

    if (rows == 0) {
        ShowRegisterDialog(playerid, "Esta conta ainda não existe.\nDigite uma senha para criá-la:");
    } else {
        cache_get_value_name_int(0, "id", g_PlayerData[playerid][E_PLAYER_DATA_ID]);
        cache_get_value_name_int(0, "cash", g_PlayerData[playerid][E_PLAYER_DATA_CASH]);
        cache_get_value_name_int(0, "score", g_PlayerData[playerid][E_PLAYER_DATA_SCORE]);
        cache_get_value_name_int(0, "level", g_PlayerData[playerid][E_PLAYER_DATA_LEVEL]);
        cache_get_value_name_int(0, "admin", g_PlayerData[playerid][E_PLAYER_DATA_ADMIN]);
        cache_get_value_name_int(0, "skin", g_PlayerData[playerid][E_PLAYER_DATA_SKIN]);
        cache_get_value_name_int(0, "interior", g_PlayerData[playerid][E_PLAYER_DATA_INTERIOR]);
        cache_get_value_name_int(0, "world", g_PlayerData[playerid][E_PLAYER_DATA_WORLD]);
        cache_get_value_name_int(0, "login_attempts", g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS]);
        cache_get_value_name_int(0, "blocked_until", g_PlayerData[playerid][E_PLAYER_DATA_BLOCKED_UNTIL]);

        cache_get_value_name_float(0, "pos_x", g_PlayerData[playerid][E_PLAYER_DATA_POS_X]);
        cache_get_value_name_float(0, "pos_y", g_PlayerData[playerid][E_PLAYER_DATA_POS_Y]);
        cache_get_value_name_float(0, "pos_z", g_PlayerData[playerid][E_PLAYER_DATA_POS_Z]);
        cache_get_value_name_float(0, "pos_a", g_PlayerData[playerid][E_PLAYER_DATA_POS_A]);

        cache_get_value_name(0, "password", g_PlayerPasswordHash[playerid], HASH_LEN);

        if (g_PlayerData[playerid][E_PLAYER_DATA_BLOCKED_UNTIL] > gettime()) {
            KickBlockedPlayer(playerid, g_PlayerData[playerid][E_PLAYER_DATA_BLOCKED_UNTIL] - gettime());
        } else {
            ShowLoginDialog(playerid, "Bem-vindo de volta!\nDigite sua senha para entrar:");
        }
    }
    return 1;
}

forward OnAccountRegistered(playerid);
public OnAccountRegistered(playerid)
{
    g_PlayerData[playerid][E_PLAYER_DATA_ID] = cache_insert_id();
    g_PlayerData[playerid][E_PLAYER_DATA_LOGGED] = true;
    g_PlayerData[playerid][E_PLAYER_DATA_ATTEMPTS] = 0;

    SetPlayerLogin(playerid, true);
    SpawnPlayerFromAccount(playerid);

    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    SendClientMessage(playerid, 0x2ECC71FF, "{2ECC71}B{27AE60}r{2ECC71}a{27AE60}s{2ECC71}i{27AE60}l {F1C40F}C{F39C12}i{F1C40F}t{F39C12}y {FFFFFF}Roleplay");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00FF7F}| {FFFFFF}Conta registrada com sucesso! Seja bem-vindo ao {F1C40F}Brasil City{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
    SendClientMessage(playerid, 0xFFFFFFFF, "{1ABC9C}| {FFFFFF}Para parar a musica utilize o comando {F1C40F}/pararmusica{FFFFFF}.");
    SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
    return 1;
}

Dialog:Login(playerid, response, listitem, inputtext[])
{
    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    if (strlen(inputtext) == 0) {
        ShowLoginDialog(playerid, "Você precisa digitar uma senha.\nDigite sua senha para entrar:");
        return 1;
    }

    ValidatePlayerPassword(playerid, inputtext);
    return 1;
}

Dialog:Registro(playerid, response, listitem, inputtext[])
{
    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    if (strlen(inputtext) < MIN_PASSWORD_LEN) {
        ShowRegisterDialog(playerid, "A senha deve ter pelo menos 4 caracteres.\nDigite uma senha para criar sua conta:");
        return 1;
    }

    HashPlayerPassword(playerid, inputtext, g_PlayerPasswordHash[playerid]);
    ShowRegisterConfirmDialog(playerid, "Digite a mesma senha novamente para confirmar:");
    return 1;
}

Dialog:RegistroConfirmar(playerid, response, listitem, inputtext[])
{
    new hash[HASH_LEN];

    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    HashPlayerPassword(playerid, inputtext, hash);

    if (!strcmp(hash, g_PlayerPasswordHash[playerid])) {
        CreatePlayerAccount(playerid);
    } else {
        ShowRegisterDialog(playerid, "As senhas não coincidem.\nDigite uma senha para criar sua conta:");
    }
    return 1;
}
