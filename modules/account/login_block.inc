#if defined _INC_account_login_block
	#endinput
#endif
#define _INC_account_login_block

/*
#define MAX_LOGIN_ATTEMPTS      3
#define LOGIN_BLOCK_TIME        60     // tempo de bloqueio em segundos
*/

static enum e_LOGIN_BLOCK_DATA
{
    E_LOGIN_BLOCK_ATTEMPTS,
    E_LOGIN_BLOCK_UNTIL
};

static s_LoginBlockData[MAX_PLAYERS][e_LOGIN_BLOCK_DATA];

stock LoginBlockCheck(playerid)
{
    new ipHash[HASH_LEN];
    new query[512];

    HashPlayerIp(playerid, ipHash);
    
    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "SELECT `attempts`, `blocked_until` FROM `login_blocks` WHERE `ip_hash` = '%e' LIMIT 1",
        ipHash
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnLoginBlockLoadResult", "d", playerid);
    return 1;
}

forward OnLoginBlockLoadResult(playerid);
public OnLoginBlockLoadResult(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    new rows;
    cache_get_row_count(rows);

    new attempts = 0;
    new blockedUntil = 0;

    if (rows) {
        cache_get_value_name_int(0, "attempts", attempts);
        cache_get_value_name_int(0, "blocked_until", blockedUntil);
    }

    s_LoginBlockData[playerid][E_LOGIN_BLOCK_ATTEMPTS] = attempts;
    s_LoginBlockData[playerid][E_LOGIN_BLOCK_UNTIL] = blockedUntil;

    new bool:blocked = bool:(blockedUntil > gettime());
    new remaining = blocked ? (blockedUntil - gettime()) : 0;

    OnLoginBlockChecked(playerid, blocked, remaining);
    return 1;
}

stock LoginBlockRegisterFailure(playerid)
{
    new ipHash[HASH_LEN];
    new query[220];
    new attempts = s_LoginBlockData[playerid][E_LOGIN_BLOCK_ATTEMPTS] + 1;

    s_LoginBlockData[playerid][E_LOGIN_BLOCK_ATTEMPTS] = attempts;

    HashPlayerIp(playerid, ipHash);

    if (attempts >= MAX_LOGIN_ATTEMPTS) {
        new blockedUntil = gettime() + LOGIN_BLOCK_TIME;

        s_LoginBlockData[playerid][E_LOGIN_BLOCK_UNTIL] = blockedUntil;

        mysql_format(GetDatabaseHandle(), query, sizeof(query),
            "INSERT INTO `login_blocks` (`ip_hash`, `attempts`, `blocked_until`) VALUES ('%e', %d, %d) ON DUPLICATE KEY UPDATE `attempts` = %d, `blocked_until` = %d",
            ipHash, attempts, blockedUntil, attempts, blockedUntil
        );
    } else {
        mysql_format(GetDatabaseHandle(), query, sizeof(query),
            "INSERT INTO `login_blocks` (`ip_hash`, `attempts`) VALUES ('%e', %d) ON DUPLICATE KEY UPDATE `attempts` = %d",
            ipHash, attempts, attempts
        );
    }

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}

stock LoginBlockReset(playerid)
{
    new ipHash[HASH_LEN];
    new query[160];

    s_LoginBlockData[playerid][E_LOGIN_BLOCK_ATTEMPTS] = 0;
    s_LoginBlockData[playerid][E_LOGIN_BLOCK_UNTIL] = 0;

    HashPlayerIp(playerid, ipHash);

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "UPDATE `login_blocks` SET `attempts` = 0, `blocked_until` = 0 WHERE `ip_hash` = '%e'",
        ipHash
    );

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}

stock GetLoginBlockRemainingAttempts(playerid)
{
    return MAX_LOGIN_ATTEMPTS - s_LoginBlockData[playerid][E_LOGIN_BLOCK_ATTEMPTS];
}
