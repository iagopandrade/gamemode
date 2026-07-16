CMD:animacoes(playerid, params[])
{
	SendClientMessage(playerid, -1, "/stopanim, /handup, /drink, /falar, /faca, /crack");
	return 1;
}

CMD:stopanim(playerid, params[])
{
	if (ClearAnimations(playerid)) {
		GameTextForPlayer(playerid, "~r~Voce interrompeu a animacao!", 3000, 3);
	}
	return 1;
}

CMD:handup(playerid, params[])
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
    return 1;
}

CMD:drink(playerid, params[])
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
    return 1;
}

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