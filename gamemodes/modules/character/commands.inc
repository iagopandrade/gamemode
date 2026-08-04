flags:reset(CMD_LOGGED)
CMD:reset(playerid, params[])
{
    ResetPlayerMoney(playerid);
    return 1;
}

flags:pagar(CMD_LOGGED)
CMD:pagar(playerid, params[])
{
    new targetid;
    new money;
    if (sscanf(params, "ui", targetid, money)) {
        GameTextForPlayer(
            playerid,"~r~SINTAXE~n~~w~Use: /pagar <id> <valor>",
             5000, 16
        );
        return 1;
    }

    if (money <= 0) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~O valor deve ser maior que zero",
            5000, 16
        );
        return 1;
    }

    if (targetid == playerid) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Voce nao pode pagar para voce mesmo",
            5000, 16
        );
        return 1;
    }

    if (!IsPlayerConnected(targetid)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Esse jogador nao esta conectado", 
			5000, 16
		);
		return 1;
	}

    if (GetPlayerMoney(playerid) < money) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Voce nao tem dinheiro suficiente",
            5000, 16
        );
        return 1;
    }

    ApplyAnimation(playerid, "DEALER", "shop_pay", 4.1, false, false, false, false, 0, SYNC_ALL);
    ApplyAnimation(targetid, "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0, SYNC_ALL);
    
    GivePlayerMoney(playerid, -money);
	GivePlayerMoney(targetid, money);
	
    new Float:x;
    new Float:y;
    new Float:z;
	GetPlayerPos(targetid, x, y, z);

    new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

    new nameTargetid[MAX_PLAYER_NAME];
	GetPlayerName(targetid, nameTargetid, sizeof(nameTargetid));

    new string[128];
    format(string, sizeof(string), "* %s entregou uma quantia em dinheiro a %s", name, nameTargetid);

    SetPlayerChatBubble(playerid, string, 0xC2A2DAFF, 15.0, 15000);

    SendClientMessage(playerid, 0xC2A2DAFF, "> %s entregou uma quantia em dinheiro a %s", name, nameTargetid);

	PlayerPlaySound(targetid, 5405, x, y, z);
	return 1;
}
