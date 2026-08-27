#include <YSI_Coding\y_hooks>

#define DEATH_TIME             (180)
#define REANIMATE_DISTANCE     (3.0)

static enum E_DEATH_DATA
{
    bool:E_DEATH_UNCONSCIOUS,
    bool:E_DEATH_REANIMATING,

    E_DEATH_TIMER,
    E_DEATH_SECONDS,

    E_DEATH_KILLER,
    E_DEATH_WEAPON,

    E_DEATH_REANIMATOR,
    E_DEATH_REANIMATE_TIMER
}

#define REANIMATE_TIME (10000)

static s_DeathData[MAX_PLAYERS][E_DEATH_DATA];

static STREAMER_TAG_3D_TEXT_LABEL:s_DeathLabel[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    s_DeathLabel[playerid] = STREAMER_TAG_3D_TEXT_LABEL:INVALID_STREAMER_ID;
    return 1;
}

Death_StartReanimate(playerid, targetid)
{
    if (!Death_IsUnconscious(targetid))
    {
        return 0;
    }

    if (s_DeathData[targetid][E_DEATH_REANIMATING])
    {
        return 0;
    }

    new Float:x;
    new Float:y;
    new Float:z;

    GetPlayerPos(targetid, x, y, z);

    if (!IsPlayerInRangeOfPoint(playerid, REANIMATE_DISTANCE, x, y, z))
    {
        return 0;
    }

    s_DeathData[targetid][E_DEATH_REANIMATING] = true;
    s_DeathData[targetid][E_DEATH_REANIMATOR] = playerid;

    ApplyAnimation(
        playerid,
        "MEDIC",
        "CPR",
        4.1,
        true,
        false,
        false,
        true,
        false
    );

    s_DeathData[targetid][E_DEATH_REANIMATE_TIMER] =
        SetTimerEx(
            "Death_FinishReanimate",
            REANIMATE_TIME,
            false,
            "ii",
            playerid,
            targetid
        );

    SendClientMessage(
        playerid,
        -1,
        "Voce iniciou a reanimacao."
    );

    return 1;
}

forward Death_FinishReanimate(playerid, targetid);

public Death_FinishReanimate(playerid, targetid)
{
    if (!IsPlayerConnected(playerid))
    {
        return 1;
    }

    if (!IsPlayerConnected(targetid))
    {
        return 1;
    }

    if (!Death_IsUnconscious(targetid))
    {
        return 1;
    }

    new Float:x;
    new Float:y;
    new Float:z;

    GetPlayerPos(targetid, x, y, z);

    if (!IsPlayerInRangeOfPoint(playerid, REANIMATE_DISTANCE, x, y, z))
    {
        ClearAnimations(playerid);

        s_DeathData[targetid][E_DEATH_REANIMATING] = false;

        SendClientMessage(
            playerid,
            -1,
            "Voce se afastou do paciente."
        );

        return 1;
    }

    ClearAnimations(playerid);

    s_DeathData[targetid][E_DEATH_REANIMATING] = false;

    Death_Revive(targetid);

    SendClientMessage(
        playerid,
        -1,
        "Paciente reanimado."
    );

    return 1;
}

bool:Death_IsUnconscious(playerid)
{
    return s_DeathData[playerid][E_DEATH_UNCONSCIOUS];
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    new Float:health;

    GetPlayerHealth(playerid, health);

    if (health - amount <= 10.0)
    {
        SetPlayerHealth(playerid, 10.0);

        Death_SetUnconscious(
            playerid,
            issuerid,
            weaponid
        );

        return 0;
    }

    return 1;
}

Death_SetUnconscious(playerid, killerid, weaponid)
{
    Death_UpdateLabel(playerid);

    if (s_DeathData[playerid][E_DEATH_UNCONSCIOUS])
    {
        return 0;
    }


    s_DeathData[playerid][E_DEATH_UNCONSCIOUS] = true;
    s_DeathData[playerid][E_DEATH_SECONDS] = DEATH_TIME;

    s_DeathData[playerid][E_DEATH_KILLER] = killerid;
    s_DeathData[playerid][E_DEATH_WEAPON] = weaponid;

    TogglePlayerControllable(playerid, false);

    ApplyAnimation(
        playerid,
        "CRACK",
        "crckdeth2",
        4.1,
        true,
        false,
        false,
        true,
        0,
        SYNC_ALL
    );

    s_DeathData[playerid][E_DEATH_TIMER] =
        SetTimerEx(
            "Death_Update",
            1000,
            true,
            "i",
            playerid
        );

    SendClientMessage(
        playerid,
        -1,
        "Voce esta desacordado. Aguarde um medico ou utilize /aceitarmorte."
    );

    return 1;
}

Death_Kill(playerid)
{
    KillTimer(s_DeathData[playerid][E_DEATH_TIMER]);

    TogglePlayerControllable(playerid, true);

    ClearAnimations(playerid);

    Death_Reset(playerid);

    SpawnPlayer(playerid);

    return 1;
}

forward Death_Update(playerid);

public Death_Update(playerid)
{
    if (!s_DeathData[playerid][E_DEATH_UNCONSCIOUS])
    {
        return 1;
    }

    s_DeathData[playerid][E_DEATH_SECONDS]--;

    if (s_DeathData[playerid][E_DEATH_SECONDS] <= 0)
    {
        Death_Kill(playerid);
    }

    return 1;
}

Death_Revive(playerid)
{
    KillTimer(s_DeathData[playerid][E_DEATH_TIMER]);

    TogglePlayerControllable(playerid, true);

    ClearAnimations(playerid);

    SetPlayerHealth(playerid, 25.0);

    Death_Reset(playerid);

    Death_UpdateLabel(playerid);

    SendClientMessage(
        playerid,
        -1,
        "Voce foi reanimado."
    );

    return 1;
}

Death_Reset(playerid)
{
    s_DeathData[playerid][E_DEATH_UNCONSCIOUS] = false;
    s_DeathData[playerid][E_DEATH_REANIMATING] = false;

    s_DeathData[playerid][E_DEATH_TIMER] = 0;
    s_DeathData[playerid][E_DEATH_SECONDS] = 0;

    s_DeathData[playerid][E_DEATH_KILLER] = INVALID_PLAYER_ID;
    s_DeathData[playerid][E_DEATH_WEAPON] = 0;

    return 1;
}

Death_UpdateLabel(playerid)
{
    if (IsValidDynamic3DTextLabel(s_DeathLabel[playerid]))
    {
        DestroyDynamic3DTextLabel(s_DeathLabel[playerid]);
        s_DeathLabel[playerid] = INVALID_STREAMER_ID;
    }

    if (!Death_IsUnconscious(playerid))
    {
        return 1;
    }

    new playerName[MAX_PLAYER_NAME];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    s_DeathLabel[playerid] = CreateDynamic3DTextLabel(
        va_return("* %s esta desacordado.", playerName),
        0xC2A2DAFF,
        0.0,
        0.0,
        0.5,
        10.0,
        playerid
    );

    foreach (new i : Player)
    {
        if (!IsPlayerStreamedIn(playerid, i))
        {
            continue;
        }

        Streamer_Update(i, STREAMER_TYPE_3D_TEXT_LABEL);
    }

    return 1;
}
