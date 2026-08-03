#if defined _INC_admin_commands
	#endinput
#endif
#define _INC_admin_commands

stock static ConnectedPlayerMessage(playerid, targetid)
{
	if (!IsPlayerConnected(targetid)) {
		return SendClientMessage(playerid, GENERAL_COLOR, "(%d) nao esta conectado.", targetid);
	}
	return 0;
}

flags:chatadmin(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:chatadmin(playerid, params[])
{
	if (isnull(params)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /chatadmin <mensagem>");
		return 1;
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	SendClientMessageToAll(GENERAL_COLOR, "%s: %s", name, params);
	return 1;
}
alias:chatadmin("ca")

flags:servico(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:servico(playerid, params[])
{
	static permission[1];
	permission[playerid] = !permission[playerid];
	SendClientMessage(
		playerid, 
		GENERAL_COLOR,
		(permission[playerid] ? "Voce entrou em servico." : "Voce saiu de servico.")
	);
	return 1;
}

flags:ir(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:ir(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /ir <playerid>");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	new Float:x;
	new Float:y;
	new Float:z;
	GetPlayerPos(targetid, x, y, z);

	new interiorid = GetPlayerInterior(targetid);
	new worldid = GetPlayerVirtualWorld(targetid);

	SetPlayerInterior(playerid, interiorid);
	SetPlayerVirtualWorld(playerid, worldid);

	SetPlayerPos(playerid, x + 0.5, y + 0.5, z);

	SendClientMessage(playerid, GENERAL_COLOR, "Voce foi teleportado.");
	return 1;
}

flags:trazer(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:trazer(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /trazer <playerid>.");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	new Float:x;
	new Float:y;
	new Float:z;
	GetPlayerPos(playerid, x, y, z);

	SetPlayerInterior(targetid, GetPlayerInterior(playerid));
	SetPlayerVirtualWorld(targetid, GetPlayerVirtualWorld(playerid));

	SetPlayerPos(targetid, x, y, z);

	SendClientMessage(playerid, GENERAL_COLOR, "Jogador trazido ate voce.");
	return 1;
}

flags:congelar(CMD_ADMIN | CMD_MODER)
CMD:congelar(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /congelar <playerid>.");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	TogglePlayerControllable(targetid, false);

	SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: Jogador congelado.");
	return 1;
}

flags:descongelar(CMD_ADMIN | CMD_MODER)
CMD:descongelar(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: Uso: /descongelar <targetid>");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	TogglePlayerControllable(targetid, true);
	
	SendClientMessage(playerid, GENERAL_COLOR, "Jogador descongelado.");
	return 1;
}

flags:telar(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:telar(playerid, params[])
{
	static Float:x;
	static Float:y;
	static Float:z;
	static interiorid;
	static worldid;

	if (isnull(params)) {
		if (GetPlayerSpectateType(playerid) > 0) {
			TogglePlayerSpectating(playerid, false);
			
			SetPlayerInterior(playerid, interiorid);
			SetPlayerVirtualWorld(playerid, worldid);

			SetPlayerPos(playerid, x, y, z);

			SendClientMessage(playerid, GENERAL_COLOR, "Voce saiu da tv");
			return 1;
		}
	}

	new targetid;
	if (sscanf(params, "u", targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "Uso: /telar <playerid>");
		return 1;
	}

	if (playerid == targetid) {
		SendClientMessage(playerid, GENERAL_COLOR, "Esse ID eh o seu.");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	if (GetPlayerSpectateType(playerid) == 0) {
		interiorid = GetPlayerInterior(playerid);
		worldid = GetPlayerVirtualWorld(playerid);

		GetPlayerPos(playerid, x, y, z);

		SendClientMessage(playerid, GENERAL_COLOR, "Suas coordenadas foram salvas");
	}

	TogglePlayerSpectating(playerid, true);
	PlayerSpectatePlayer(playerid, targetid);
	return 1;
}
alias:telar("tv")

flags:tvoff(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:tvoff(playerid, params[])
{
	if (GetPlayerSpectateType(playerid) == 0) {
		SendClientMessage(playerid, GENERAL_COLOR, "Voce nao esta em tv.");
		return 1;
	}

	TogglePlayerSpectating(playerid, false);
	return 1;
}
/*
CMD:advertir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /advertir [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	Warnings[id]++;
	new string[180];
	format(string, sizeof(string), "[BC:RP] Voce recebeu uma advertencia (%d). Motivo: %s", Warnings[id], motivo);
	SendClientMessage(id, COR_BCRP_INFO, string);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador advertido.");
	return 1;
}
*/
flags:expulsar(CMD_ADMIN | CMD_MODER)
CMD:expulsar(playerid, params[])
{
	new targetid;
	new message[128];
	if (sscanf(params, "us[128]", targetid, message)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: Uso: /expulsar <playerid> <motivo>");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	SendClientMessage(targetid, GENERAL_COLOR, "Voce foi expulso. Motivo: %s", message);

	if (Kick(targetid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "Jogador expulso.");
		return 1;
	}
	SendClientMessage(playerid, GENERAL_COLOR, "vixxxx");
	return 1;
}
alias:expulsar("kick")
/*
CMD:mutar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /mutar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	Muted[id] = true;
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi mutado por um administrador.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador mutado.");
	return 1;
}

CMD:desmutar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /desmutar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	Muted[id] = false;
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi desmutado.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador desmutado.");
	return 1;
}

CMD:prender(playerid, params[])
{
	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /prender [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	Jailed[id] = true;
	SetPlayerPos(id, 264.6432, 77.1918, 1001.0391);
	SetPlayerInterior(id, 6);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi preso por um administrador.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador preso.");
	return 1;
}

CMD:soltar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /soltar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	Jailed[id] = false;
	SetPlayerInterior(id, 0);
	SetPlayerPos(id, 1542.0, -1675.0, 13.5);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi solto.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador solto.");
	return 1;
}

CMD:curar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /curar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	SetPlayerHealth(id, 100.0);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi curado por um administrador.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador curado.");
	return 1;
}

CMD:colete(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /colete [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	SetPlayerArmour(id, 100.0);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce recebeu colete de um administrador.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Colete dado.");
	return 1;
}
*/
flags:setarskin(CMD_ADMIN | CMD_MODER)
CMD:setarskin(playerid, params[])
{
	new targetid, skinid;
	if (sscanf(params, "ui", targetid, skinid)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: Uso: /setarskin <playerid> <skinid>.");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	SetPlayerSkin(targetid, skinid);
	SendClientMessage(playerid, GENERAL_COLOR, "Skin alterada.");
	return 1;
}
alias:setarskin("setskin", "ss")

/*
CMD:banir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /banir [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	new string[180];
	format(string, sizeof(string), "[BC:RP] Voce foi banido. Motivo: %s", motivo);
	SendClientMessage(id, COR_BCRP_INFO, string);
	Ban(id);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador banido.");
	return 1;
}

CMD:desbanir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new ip[16];
	if (sscanf(params, "s[16]", ip)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /desbanir [ip]"); return 1; }

	new string[64];
	format(string, sizeof(string), "unbanip %s", ip);
	SendRconCommand(string);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] IP desbanido.");
	return 1;
}

CMD:clima(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new valor;
	if (sscanf(params, "i", valor)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /clima [valor]"); return 1; }

	SetWeather(valor);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Clima alterado.");
	return 1;
}
*/
flags:dardinheiro(CMD_ADMIN)
CMD:dardinheiro(playerid, params[])
{
	new targetid, money;
	if (sscanf(params, "ui", targetid, money)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /dardinheiro <playerid> <valor>.");
		return 1;
	}

	if (ConnectedPlayerMessage(playerid, targetid)) {
		return 1;
	}

	GivePlayerMoney(targetid, money);
	SendClientMessage(playerid, GENERAL_COLOR, "Dinheiro dado.");
	return 1;
}
alias:dardinheiro("dd")

/*
CMD:definirnivel(playerid, params[])
{
	new id, nivel;
	if (sscanf(params, "ui", id, nivel)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /definirnivel [id] [nivel 0-4]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }
	if (nivel < 0 || nivel > 4) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Nivel invalido. Use de 0 a 4."); return 1; }

	AdminLevel[id] = nivel;
	new string[144];
	format(string, sizeof(string), "[BC:RP] Seu nivel de admin foi definido para %d.", nivel);
	SendClientMessage(id, COR_BCRP_INFO, string);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Nivel definido.");
	return 1;
}

CMD:definirlevel(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_FUNDADOR) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, level;
	if (sscanf(params, "ui", id, level)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /definirlevel [id] [level]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }
	if (level < 1) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] O level minimo e 1."); return 1; }

	Nivel[id] = level;
	SetPlayerScore(id, level);
	TempoJogado[id] = 0;
	SalvarConta(id);

	new string[100];
	format(string, sizeof(string), "[BC:RP] Seu level foi definido para %d.", level);
	SendClientMessage(id, COR_BCRP_INFO, string);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Level definido.");
	return 1;
}

CMD:forcarpayday(playerid, params[])
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (IsPlayerConnected(i) && IsLoggedIn[i])
		{
			ExecutarPayday(i);
		}
	}

	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Payday forcado para todos os jogadores online.");
	return 1;
}
*/

flags:criarveh(CMD_ADMIN)
CMD:criarveh(playerid, params[])
{
	new vehicleModel;
    if (sscanf(params, "k<vehicle>", vehicleModel)) {
		SendClientMessage(playerid, GENERAL_COLOR, "SYNTAX: /criarveh <vehicleModel OR vehicleName>");
		return 1;
	}

	new Float:x;
	new Float:y;
	new Float:z;
	GetPlayerPos(playerid, x, y, z);

	new Float:angle;
	GetPlayerFacingAngle(playerid, angle);
	
	new vehicleid = CreateVehicle(vehicleModel, x, y, z + 5, angle, -1, -1, -1, true);

	PutPlayerInVehicle(playerid, vehicleid, 0);
	return 1;
}
alias:criarveh("cv")

flags:ajudaadmin(CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:ajudaadmin(playerid, params[])
{
	Dialog_Show(playerid, "Comandos Administrativos", 
		DIALOG_STYLE_MSGBOX,
		"Informacao | "EMBED_RED" Comandos Administrativos",
		EMBED_EMERALD"Helper"\
		EMBED_WHITE"\n/aa, /achat, /ir, /trazer, /tv, /tvoff, /servico\n"\
		EMBED_CANARY"Moderador"\
		EMBED_WHITE"\n/kick, /setarskin, /congelar, /descongelar, /criarveh\n"\
		EMBED_RED"Administrador"\
		EMBED_WHITE"\n/ban, /setartempo, /setarclima, /dardinheiro, /definirlevel, /forcarpayday, /acasa\n", 
		"", ""
	);
	return 1;
}
alias:ajudaadmin("aa")
