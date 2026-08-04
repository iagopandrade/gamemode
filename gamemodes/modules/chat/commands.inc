#if defined _INC_chat_commands
	#endinput
#endif
#define _INC_chat_commands

flags:limparchat(CMD_LOGGED)
CMD:limparchat(playerid, params[])
{	
	for (new i = 0; i <= 17; i++) {
        SendClientMessage(playerid, -1, " ");
	}
    return 1;
}
alias:limparchat("lc", "clear", "cls")

flags:me(CMD_LOGGED)
CMD:me(playerid, params[])
{
	if (isnull(params)) {
		GameTextForPlayer(playerid,"~r~SINTAXE~n~~w~Use: /me <acao>", 5000, 16);
		return 1;
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[128];
	format(string, sizeof(string), "* %s %s", name, params);
	
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}

flags:do(CMD_LOGGED)
CMD:do(playerid, params[])
{
	if (isnull(params)) {
		GameTextForPlayer(playerid,"~r~SINTAXE~n~~w~Use: /do <descricao>", 5000, 16);
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	format(string, sizeof(string), "* %s (( %s ))", params, name);
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}

flags:b(CMD_LOGGED)
CMD:b(playerid, params[])
{
	if (isnull(params)) {
		GameTextForPlayer(playerid,"~r~SINTAXE~n~~w~Use: /b <mensagem>", 5000, 16);
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	format(string, sizeof(string), "((%s: %s))", name, params);
	SendClientMessageToAll(0xAAAAAAFF, string);
	return 1;
}

flags:pm(CMD_LOGGED)
CMD:pm(playerid, params[])
{
	new id;
	new mensagem[128];
	if (sscanf(params, "us[128]", id, mensagem)) {
		GameTextForPlayer(playerid,"~r~SINTAXE~n~~w~Use: /pm <id> <mensagem>", 5000, 16);
		return 1;
	}

	if (!IsPlayerConnected(id)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Esse jogador nao esta conectado", 
			5000, 16
		);
		return 1;
	}

	new nomeQuemFala[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nomeQuemFala, sizeof(nomeQuemFala));

	SendClientMessage(id, 0xFFFF00FF, "[PM de %s]: %s", nomeQuemFala, mensagem);

	new nomeQuemRecebe[MAX_PLAYER_NAME];
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	SendClientMessage(playerid, 0xFFFF00FF, "[PM para %s]: %s", nomeQuemRecebe, mensagem);
	return 1;
}

flags:sussurrar(CMD_LOGGED)
CMD:sussurrar(playerid, params[])
{
	new id;
	new mensagem[128];
	if (sscanf(params, "us[128]", id, mensagem)) {
		GameTextForPlayer(playerid,"~r~SINTAXE~n~~w~Use: /sussurrar <id> <mensagem>", 5000, 16);
		return 1;
	}

	if (!IsPlayerConnected(id)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Esse jogador nao esta conectado", 
			5000, 16
		);
		return 1;
	}

	if (id == playerid) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Voce nao pode sussurrar para voce mesmo",
			5000, 16
		);
		return 1;
	}

	new Float:x;
	new Float:y;
	new Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (!IsPlayerInRangeOfPoint(id, 3.0, x, y, z)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Esse jogador esta longe demais para sussurrar", 
			5000, 16
		);
		return 1;
	}

	new string[144], nomeQuemFala[MAX_PLAYER_NAME], nomeQuemRecebe[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nomeQuemFala, sizeof(nomeQuemFala));
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	format(string, sizeof(string), "%s sussurra para voce: %s", nomeQuemFala, mensagem);
	SendClientMessage(id, 0xFFFF00FF, string);

	format(string, sizeof(string), "Voce sussurra para %s: %s", nomeQuemRecebe, mensagem);
	SendClientMessage(playerid, 0xFFFF00FF, string);
	return 1;
}
alias:sussurrar("su")
