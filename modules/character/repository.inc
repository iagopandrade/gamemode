#if defined _INC_character_repository
	#endinput
#endif
#define _INC_character_repository

stock CharacterLoadForAccount(playerid, accountId)
{
    new query[128];

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "SELECT * FROM `characters` WHERE `account_id` = %d ORDER BY `id` ASC LIMIT 1",
        accountId
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnCharacterLoadResult", "d", playerid);
    return 1;
}

stock CharacterCreate(playerid, accountId)
{
    new query[400];
    new Float:x;
    new Float:y;
    new Float:z;
    new Float:angle;

    GetCharacterPos(playerid, x, y, z, angle);

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "INSERT INTO `characters` (`account_id`, `cash`, `score`, `level`, `skin`, `interior`, `world`, `pos_x`, `pos_y`, `pos_z`, `pos_a`) VALUES (%d, %d, %d, %d, %d, %d, %d, '%f', '%f', '%f', '%f')",
        accountId,
        GetCharacterCash(playerid),
        GetCharacterScore(playerid),
        GetCharacterLevel(playerid),
        GetCharacterSkin(playerid),
        GetCharacterInterior(playerid),
        GetCharacterWorld(playerid),
        x, y, z, angle
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnCharacterCreateResult", "d", playerid);
    return 1;
}

stock CharacterSave(playerid)
{
    new query[320];
    new Float:x;
    new Float:y;
    new Float:z;
    new Float:angle;

    GetCharacterPos(playerid, x, y, z, angle);

    mysql_format(GetDatabaseHandle(), query, sizeof(query),
        "UPDATE `characters` SET `cash` = %d, `score` = %d, `level` = %d, `skin` = %d, `interior` = %d, `world` = %d, `pos_x` = '%f', `pos_y` = '%f', `pos_z` = '%f', `pos_a` = '%f' WHERE `id` = %d",
        GetCharacterCash(playerid),
        GetCharacterScore(playerid),
        GetCharacterLevel(playerid),
        GetCharacterSkin(playerid),
        GetCharacterInterior(playerid),
        GetCharacterWorld(playerid),
        x, y, z, angle,
        GetCharacterId(playerid)
    );

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}
