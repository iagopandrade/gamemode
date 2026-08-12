#if defined _INC_CharacterCore
	#endinput
#endif
#define _INC_CharacterCore


#define DEFAULT_CHARACTER_SKIN      1
#define DEFAULT_CHARACTER_POS_X     1723.0403
#define DEFAULT_CHARACTER_POS_Y     -1876.3651
#define DEFAULT_CHARACTER_POS_Z     13.5646
#define DEFAULT_CHARACTER_ANGLE     0.0

static enum e_CHARACTER_DATA
{
    E_CHARACTER_ID,
    E_CHARACTER_CASH,
    E_CHARACTER_SCORE,
    E_CHARACTER_LEVEL,
    E_CHARACTER_SKIN,
    E_CHARACTER_INTERIOR,
    E_CHARACTER_WORLD,

    Float:E_CHARACTER_POS_X,
    Float:E_CHARACTER_POS_Y,
    Float:E_CHARACTER_POS_Z,
    Float:E_CHARACTER_POS_A
};

static s_CharacterData[MAX_PLAYERS][e_CHARACTER_DATA];

stock GetCharacterId(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_ID];
}

stock SetCharacterId(playerid, id)
{
    s_CharacterData[playerid][E_CHARACTER_ID] = id;
    return 1;
}

stock SetCharacterCash(playerid, cash)
{
    s_CharacterData[playerid][E_CHARACTER_CASH] = cash;
    return 1;
}

stock GetCharacterCash(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_CASH];
}

stock SetCharacterScore(playerid, score)
{
    s_CharacterData[playerid][E_CHARACTER_SCORE] = score;
    return 1;
}

stock GetCharacterScore(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_SCORE];
}

stock SetCharacterLevel(playerid, level)
{
    s_CharacterData[playerid][E_CHARACTER_LEVEL] = level;
    return 1;
}

stock GetCharacterLevel(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_LEVEL];
}

stock SetCharacterSkin(playerid, skinid)
{
    s_CharacterData[playerid][E_CHARACTER_SKIN] = skinid;
    return 1;
}

stock GetCharacterSkin(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_SKIN];
}

stock SetCharacterInterior(playerid, interiorid)
{
    s_CharacterData[playerid][E_CHARACTER_INTERIOR] = interiorid;
    return 1;
}

stock GetCharacterInterior(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_INTERIOR];
}

stock SetCharacterWorld(playerid, worldid)
{
    s_CharacterData[playerid][E_CHARACTER_WORLD] = worldid;
    return 1;
}

stock GetCharacterWorld(playerid)
{
    return s_CharacterData[playerid][E_CHARACTER_WORLD];
}

stock SetCharacterAngle(playerid, Float:ang)
{
    s_CharacterData[playerid][E_CHARACTER_POS_A] = ang;
    return 1;
}

stock GetCharacterAngle(playerid, &Float:ang)
{
    ang = s_CharacterData[playerid][E_CHARACTER_POS_A];
    return 1;
}

stock SetCharacterPos(playerid, Float:x, Float:y, Float:z, Float:ang)
{
    s_CharacterData[playerid][E_CHARACTER_POS_X] = x;
    s_CharacterData[playerid][E_CHARACTER_POS_Y] = y;
    s_CharacterData[playerid][E_CHARACTER_POS_Z] = z;
    s_CharacterData[playerid][E_CHARACTER_POS_A] = ang;
    return 1;
}

stock GetCharacterPos(playerid, &Float:x, &Float:y, &Float:z, &Float:ang)
{
    x = s_CharacterData[playerid][E_CHARACTER_POS_X];
    y = s_CharacterData[playerid][E_CHARACTER_POS_Y];
    z = s_CharacterData[playerid][E_CHARACTER_POS_Z];
    ang = s_CharacterData[playerid][E_CHARACTER_POS_A];
    return 1;
}

stock SelectCharacterForAccount(playerid)
{
    CharacterLoadForAccount(playerid, GetAccountId(playerid));
    return 1;
}

forward OnCharacterLoadResult(playerid);
public OnCharacterLoadResult(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    new rows;
    cache_get_row_count(rows);

    if (!rows) {
        CreateDefaultCharacter(playerid);
        return 1;
    }

    ApplyLoadedCharacterRow(playerid);
    SpawnPlayerFromCharacter(playerid);
    return 1;
}

stock ApplyLoadedCharacterRow(playerid)
{
    new cash;
    new score;
    new level;
    new skin;
    new interior;
    new world;

    new Float:x;
    new Float:y;
    new Float:z;
    new Float:angle;

    cache_get_value_name_int(0, "id", s_CharacterData[playerid][E_CHARACTER_ID]);
    cache_get_value_name_int(0, "cash", cash);
    cache_get_value_name_int(0, "score", score);
    cache_get_value_name_int(0, "level", level);
    cache_get_value_name_int(0, "skin", skin);
    cache_get_value_name_int(0, "interior", interior);
    cache_get_value_name_int(0, "world", world);

    cache_get_value_name_float(0, "pos_x", x);
    cache_get_value_name_float(0, "pos_y", y);
    cache_get_value_name_float(0, "pos_z", z);
    cache_get_value_name_float(0, "pos_a", angle);

    SetCharacterCash(playerid, cash);
    SetCharacterScore(playerid, score);
    SetCharacterLevel(playerid, level);
    SetCharacterSkin(playerid, skin);
    SetCharacterInterior(playerid, interior);
    SetCharacterWorld(playerid, world);
    SetCharacterPos(playerid, x, y, z, angle);
    return 1;
}

stock CreateDefaultCharacter(playerid)
{
    SetCharacterCash(playerid, 0);
    SetCharacterScore(playerid, 0);
    SetCharacterLevel(playerid, 0);
    SetCharacterSkin(playerid, DEFAULT_CHARACTER_SKIN);
    SetCharacterInterior(playerid, 0);
    SetCharacterWorld(playerid, 0);

    SetCharacterPos(
        playerid,
        DEFAULT_CHARACTER_POS_X,
        DEFAULT_CHARACTER_POS_Y,
        DEFAULT_CHARACTER_POS_Z,
        DEFAULT_CHARACTER_ANGLE
    );

    CharacterCreate(playerid, GetAccountId(playerid));
    return 1;
}

forward OnCharacterCreateResult(playerid);
public OnCharacterCreateResult(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return 1;
    }

    SetCharacterId(playerid, cache_insert_id());
    SpawnPlayerFromCharacter(playerid);
    return 1;
}

stock SpawnPlayerFromCharacter(playerid)
{
    new Float:x;
    new Float:y;
    new Float:z;
    new Float:angle;

    GetCharacterPos(playerid, x, y, z, angle);

    SetSpawnInfo(
        playerid,
        NO_TEAM,
        GetCharacterSkin(playerid),
        x, y, z, angle,
        WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0
    );

    SetPlayerInterior(playerid, GetCharacterInterior(playerid));
    SetPlayerVirtualWorld(playerid, GetCharacterWorld(playerid));

    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    return 1;
}

stock SaveCharacter(playerid)
{
    new Float:posX;
    new Float:posY;
    new Float:posZ;
    new Float:posA;

    GetPlayerPos(playerid, posX, posY, posZ);
    GetPlayerFacingAngle(playerid, posA);

    SetCharacterCash(playerid, GetPlayerMoney(playerid));
    SetCharacterScore(playerid, GetPlayerScore(playerid));
    SetCharacterSkin(playerid, GetPlayerSkin(playerid));
    SetCharacterInterior(playerid, GetPlayerInterior(playerid));
    SetCharacterWorld(playerid, GetPlayerVirtualWorld(playerid));
    SetCharacterPos(playerid, posX, posY, posZ, posA);

    CharacterSave(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (HasPlayerLogged(playerid)) {
        GivePlayerMoney(playerid, GetCharacterCash(playerid));
        SetPlayerScore(playerid, GetCharacterScore(playerid));
        TogglePlayerControllable(playerid, true);
    }

    #if defined CharacterCore_OnPlayerSpawn
        return CharacterCore_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn CharacterCore_OnPlayerSpawn

#if defined CharacterCore_OnPlayerSpawn
    forward CharacterCore_OnPlayerSpawn(playerid);
#endif
