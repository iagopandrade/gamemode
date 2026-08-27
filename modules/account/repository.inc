#if defined _INC_account_login_repository
	#endinput
#endif
#define _INC_account_login_repository

stock AccountLoad(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[128];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "SELECT * FROM `accounts` WHERE `name` = '%e' LIMIT 1",
        playerName
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnAccountLoadResult", "d", playerid);
    return 1;
}

stock AccountCreate(playerid, const passwordHash[HASH_LEN], const ipHash[HASH_LEN])
{
    new playerName[MAX_PLAYER_NAME];
    new query[400];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "INSERT INTO `accounts` (`name`, `password`, `last_ip`, `created_at`, `last_login`) VALUES ('%e', '%e', '%e', NOW(), NOW())",
        playerName, passwordHash, ipHash
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnAccountCreateResult", "d", playerid);
    return 1;
}

stock AccountUpdateLoginSuccess(accountId, const ipHash[HASH_LEN])
{
    new query[192];

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "UPDATE `accounts` SET `last_ip` = '%e', `last_login` = NOW() WHERE `id` = %d",
        ipHash, accountId
    );

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}

stock AccountSave(playerid)
{
    new query[160];

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "UPDATE `accounts` SET `admin` = %d WHERE `id` = %d",
        _:GetAccountAdminLevel(playerid),
        GetAccountId(playerid)
    );

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}
