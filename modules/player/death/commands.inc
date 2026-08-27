CMD:ak(playerid, params[])
{
    GivePlayerWeapon(playerid, WEAPON_AK47, 999);
}

CMD:reanimar(playerid, params[])
{
    new targetid;

    if (sscanf(params, "u", targetid))
    {
        SendClientMessage(playerid, -1, "Uso: /reanimar [id]");
        return 1;
    }

    if (!IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, "Jogador invalido.");
        return 1;
    }

    if (!Death_IsUnconscious(targetid))
    {
        SendClientMessage(playerid, -1, "Este jogador nao esta desacordado.");
        return 1;
    }

    new Float:x, Float:y, Float:z;

    GetPlayerPos(targetid, x, y, z);

    if (!IsPlayerInRangeOfPoint(playerid, REANIMATE_DISTANCE, x, y, z))
    {
        SendClientMessage(playerid, -1, "Voce precisa estar proximo do jogador.");
        return 1;
    }

    Death_Revive(targetid);

    SendClientMessage(playerid, -1, "Voce reanimou o jogador.");
    SendClientMessage(targetid, -1, "Voce foi reanimado.");

    return 1;
}

static enum
{
    DIALOG_DEATH_INTERACT
};

static s_DeathSelectedPlayer[MAX_PLAYERS];

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
    if (!Death_IsUnconscious(clickedplayerid))
    {
        return 1;
    }

    s_DeathSelectedPlayer[playerid] = clickedplayerid;

    Dialog_Show(playerid, DIALOG_DEATH_INTERACT,
        DIALOG_STYLE_LIST,
        "Interagir",
        "Reviver\nRevistar",
        "Selecionar",
        "Cancelar"
    );

    return 1;
}

Dialog:DIALOG_DEATH_INTERACT(playerid, response, listitem)
{
    if (!response)
    {
        return 1;
    }

    new targetid = s_DeathSelectedPlayer[playerid];

    if (!IsPlayerConnected(targetid))
    {
        return 1;
    }

    switch (listitem)
    {
        case 0:
        {
            Death_StartReanimate(playerid, targetid);
        }

        case 1:
        {
            SendClientMessage(
                playerid,
                -1,
                "Sistema de revista ainda nao implementado."
            );
        }
    }

    return 1;
}

