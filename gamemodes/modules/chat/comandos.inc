CMD:me(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, -1, "Uso: /me [acao]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "* %s %s", name, params);
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}
CMD:do(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, -1, "Uso: /do [descricao]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "* %s (( %s ))", params, name);
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}
CMD:b(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /b [mensagem]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "((%s: %s))", name, params);
	SendClientMessageToAll(0xAAAAAAFF, string);
	return 1;
}
CMD:pm(playerid, params[])
{
	new id, mensagem[128];

	if (sscanf(params, "us[128]", id, mensagem))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /pm [id] [mensagem]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao pode mandar PM para si mesmo.");
		return 1;
	}

	new string[144], nomeQuemFala[MAX_PLAYER_NAME], nomeQuemRecebe[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nomeQuemFala, sizeof(nomeQuemFala));
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	format(string, sizeof(string), "[PM de %s]: %s", nomeQuemFala, mensagem);
	SendClientMessage(id, 0xFFFF00FF, string);

	format(string, sizeof(string), "[PM para %s]: %s", nomeQuemRecebe, mensagem);
	SendClientMessage(playerid, 0xFFFF00FF, string);

	return 1;
}
CMD:su(playerid, params[])
{
	new id, mensagem[128];

	if (sscanf(params, "us[128]", id, mensagem))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /su [id] [mensagem]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao pode sussurrar para si mesmo.");
		return 1;
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (!IsPlayerInRangeOfPoint(id, 3.0, x, y, z))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Esse jogador esta longe demais para sussurrar.");
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