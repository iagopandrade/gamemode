static enum e_CHARACTER_DATA
{
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
