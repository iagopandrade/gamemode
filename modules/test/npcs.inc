static PlayerNPC[10];

flags:criarnpc(CMD_LOGGED)
CMD:criarnpc(playerid, params[])
{
    static g_NPCCount;
	new name[24];
    format(name, sizeof name, "Bot_%d", g_NPCCount++);

    new npcid = NPC_Create(name);
    if (NPC_IsValid(npcid))
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);

        NPC_Spawn(npcid);
        NPC_SetPos(npcid, x + 3.0, y, z);
        NPC_SetWeapon(npcid, WEAPON_M4);
        NPC_SetAmmo(npcid, 500);

        PlayerNPC[playerid] = npcid;
        SendClientMessage(playerid, 0x00FF00FF, "NPC %s (ID %d) spawned near you!", name, npcid);
    }
    else
    {
        SendClientMessage(playerid, 0xFF0000FF, "Failed to create NPC!");
    }
    return 1;
}
