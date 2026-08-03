#if defined _INC_animations_commands
	#endinput
#endif
#define _INC_animations_commands

flags:animacoes(CMD_ALIVE)
CMD:animacoes(playerid, params[])
{
	SendClientMessage(playerid, GENERAL_COLOR, "_________________________________________________");
	SendClientMessage(playerid, GENERAL_COLOR, "/pararanimacao, /render, /drink, /falar, /faca, /crack");
	SendClientMessage(playerid, GENERAL_COLOR, "_________________________________________________");
	return 1;
}

flags:pararanimacao(CMD_ALIVE)
CMD:pararanimacao(playerid, params[])
{
	if (ClearAnimations(playerid)) {
		GameTextForPlayer(playerid, "~r~Voce interrompeu a animacao!", 3000, 3);
	}
	return 1;
}
alias:pararanimacao("pa", "stopanim")

flags:handsup(CMD_ALIVE)
CMD:handsup(playerid, params[])
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
    return 1;
}
alias:handsup("render")

flags:drink(CMD_ALIVE)
CMD:drink(playerid, params[])
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
    return 1;
}

flags:falar(CMD_ALIVE)
CMD:falar(playerid, params[])
{
    ApplyAnimation(
		playerid,
		"PED",
		"IDLE_CHAT",
		4.1,
		false,
		true,
		true,
		false,
		0,
		SYNC_ALL
	);
	return 1;
}

flags:faca(CMD_ALIVE)
CMD:faca(playerid, params[])
{
    new type;
	if (sscanf(params, "d", type)) {
		GameTextForPlayer(playerid, "~r~Uso: /faca <1-8>", 3000, 3);
		return 1;
	}

	if (type < 1 || type > 8) {
	    GameTextForPlayer(playerid, "~r~Opcao Invalida!", 3000, 3);
		return 1;
	}

	switch (type) {
		case 1: { ApplyAnimation(playerid, "KNIFE", "knife_1", 4.1, false, true, true, false, 0, SYNC_ALL); }
		case 2: { ApplyAnimation(playerid, "KNIFE", "knife_2", 4.1, false, true, true, false, 0, SYNC_ALL); }
		case 3: { ApplyAnimation(playerid, "KNIFE", "knife_3", 4.1, false, true, true, false, 0, SYNC_ALL); }
		case 4: { ApplyAnimation(playerid, "KNIFE", "knife_4", 4.1, false, true, true, false, 0, SYNC_ALL); }
		case 5: { ApplyAnimation(playerid, "KNIFE", "WEAPON_knifeidle", 4.1, true, false, false, false, 0, SYNC_ALL); }
		case 6: { ApplyAnimation(playerid, "KNIFE", "KILL_Knife_Player", 4.1, false, false, false, false, 0, SYNC_ALL); }
		case 7: { ApplyAnimation(playerid, "KNIFE", "KILL_Knife_Ped_Damage", 4.1, false, false, false, false, 0, SYNC_ALL); }
		case 8: { ApplyAnimation(playerid, "KNIFE", "KILL_Knife_Ped_Die", 4.1, false, false, false, false, 0, SYNC_ALL); }
	}
	return 1;
}

flags:crack(CMD_ALIVE)
CMD:crack(playerid, params[])
{
    new type;
	if (sscanf(params, "d", type)) {
		GameTextForPlayer(playerid, "~r~Uso: /crack <1-10>", 3000, 3);
		return 1;
	}

	if (type < 1 || type > 10){
	    GameTextForPlayer(playerid, "~r~Opcao Invalida!", 3000, 3);
		return 1;
	}

	switch (type) {
		case 1: { ApplyAnimation(playerid, "CRACK", "Bbalbat_Idle_01", 6.67, false, true, true, false, 0, SYNC_ALL); }
		case 2: { ApplyAnimation(playerid, "CRACK", "Bbalbat_Idle_02", 10.00, false, true, true, false, 0, SYNC_ALL); }
		case 3: { ApplyAnimation(playerid, "CRACK", "crckdeth1", 2.17, false, true, true, false, 0, SYNC_ALL); }
		case 4: { ApplyAnimation(playerid, "CRACK", "crckdeth2", 3.00, false, true, true, false, 0, SYNC_ALL); }
		case 5: { ApplyAnimation(playerid, "CRACK", "crckdeth3", 2.17, true, false, false, false, 0, SYNC_ALL); }
		case 6: { ApplyAnimation(playerid, "CRACK", "crckdeth4", 1.67, false, false, false, false, 0, SYNC_ALL); }
		case 7: { ApplyAnimation(playerid, "CRACK", "crckidle1", 3.50, false, false, false, false, 0, SYNC_ALL); }
		case 8: { ApplyAnimation(playerid, "CRACK", "crckidle2", 3.33, false, false, false, false, 0, SYNC_ALL); }
		case 9: { ApplyAnimation(playerid, "CRACK", "crckidle3", 3.33, false, false, false, false, 0, SYNC_ALL); }
		case 10: { ApplyAnimation(playerid, "CRACK", "crckidle4", 2.67, false, false, false, false, 0, SYNC_ALL); }
	}
	return 1;
}
