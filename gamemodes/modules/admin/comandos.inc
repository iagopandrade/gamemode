CMD:tpconfig(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid, 1467.2704,-1011.1876,26.8438 + 1.0);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce foi teleportado com sucesso!");
	return 1;
}

CMD:achat(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }
	if (isnull(params)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /achat [mensagem]"); return 1; }

	new string[144], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "[ADMIN CHAT] %s: %s", name, params);

	for (new i = 0; i < MAX_PLAYERS; i++)
		if (IsPlayerConnected(i) && AdminLevel[i] >= LEVEL_HELPER)
			SendClientMessage(i, 0x00FFFFFF, string);
	return 1;
}

CMD:servico(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	AdminDuty[playerid] = !AdminDuty[playerid];
	SendClientMessage(playerid, COR_BCRP_INFO, AdminDuty[playerid] ? ("[BC:RP] Voce entrou em servico.") : ("[BC:RP] Voce saiu de servico."));
	return 1;
}

CMD:irpara(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /irpara [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	new Float:x, Float:y, Float:z;
	GetPlayerPos(id, x, y, z);
	SetPlayerPos(playerid, x, y, z);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce foi teleportado.");
	return 1;
}

CMD:trazer(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /trazer [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPos(id, x, y, z);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador trazido ate voce.");
	return 1;
}

CMD:congelar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /congelar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	TogglePlayerControllable(id, false);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi congelado por um administrador.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador congelado.");
	return 1;
}

CMD:descongelar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /descongelar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	TogglePlayerControllable(id, true);
	SendClientMessage(id, COR_BCRP_INFO, "[BC:RP] Voce foi descongelado.");
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador descongelado.");
	return 1;
}

CMD:espectar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /espectar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	if (!Espectando[playerid])
	{
		GetPlayerPos(playerid, PosAntesEspectar[playerid][0], PosAntesEspectar[playerid][1], PosAntesEspectar[playerid][2]);
		Espectando[playerid] = true;
	}

	TogglePlayerSpectating(playerid, true);
	PlayerSpectatePlayer(playerid, id);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Espectando jogador.");
	return 1;
}

CMD:pararespectar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }
	if (!Espectando[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao esta espectando ninguem."); return 1; }

	TogglePlayerSpectating(playerid, false);
	SetPlayerPos(playerid, PosAntesEspectar[playerid][0], PosAntesEspectar[playerid][1], PosAntesEspectar[playerid][2]);
	Espectando[playerid] = false;
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce parou de espectar.");
	return 1;
}

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
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 2 (GAME ADMIN)
//////////////////////////////////////////////////

CMD:expulsar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /expulsar [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	new string[180];
	format(string, sizeof(string), "[BC:RP] Voce foi expulso. Motivo: %s", motivo);
	SendClientMessage(id, COR_BCRP_INFO, string);
	Kick(id);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Jogador expulso.");
	return 1;
}

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
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

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

CMD:definirskin(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, skinid;
	if (sscanf(params, "ui", id, skinid)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /definirskin [id] [skinid]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	SetPlayerSkin(id, skinid);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Skin alterada.");
	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 3 (ENCARREGADO)
//////////////////////////////////////////////////

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

CMD:hora(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new valor;
	if (sscanf(params, "i", valor)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /hora [valor de 0 a 23]"); return 1; }

	SetWorldTime(valor);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Hora alterada.");
	return 1;
}

CMD:darinheiro(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new id, valor;
	if (sscanf(params, "ui", id, valor)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /darinheiro [id] [valor]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado."); return 1; }

	GivePlayerMoney(id, valor);
	SendClientMessage(playerid, COR_BCRP_DINHEIRO, "[BC:RP] Dinheiro dado.");
	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 4 (FUNDADOR)
//////////////////////////////////////////////////

CMD:definirnivel(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_FUNDADOR) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

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
CMD:acmds(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new string[1024];
	strcat(string, "{00FF00}NIVEL 1 - HELPER\n");
	strcat(string, "{AAAAAA}/achat, /servico, /irpara, /trazer, /congelar, /descongelar, /espectar, /pararespectar, /advertir\n\n");
	strcat(string, "{1E90FF}NIVEL 2 - GAME ADMIN\n");
	strcat(string, "{AAAAAA}(tudo do Helper) + /expulsar, /mutar, /desmutar, /prender, /soltar, /curar, /colete, /definirskin\n\n");
	strcat(string, "{FFA500}NIVEL 3 - ENCARREGADO\n");
	strcat(string, "{AAAAAA}(tudo do Game Admin) + /banir, /desbanir, /clima, /hora, /darinheiro\n\n");
	strcat(string, "{FF0000}NIVEL 4 - FUNDADOR\n");
	strcat(string, "{AAAAAA}(tudo do Encarregado) + /definirnivel, /definirlevel");

	ShowPlayerDialog(playerid, DIALOG_ACMDS, DIALOG_STYLE_MSGBOX, "Painel Administrativo", string, "Fechar", "");
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
	if (AdminLevel[playerid] < LEVEL_FUNDADOR) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

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

CMD:veiculo(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem permissao para usar este comando."); return 1; }

	new modelo;
	if (sscanf(params, "i", modelo)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /veiculo [id do veiculo]"); return 1; }
	if (modelo < 400 || modelo > 611) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] ID de veiculo invalido. Use de 400 a 611."); return 1; }

	new Float:x, Float:y, Float:z, Float:angulo;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angulo);

	new veiculo = CreateVehicle(modelo, x, y, z, angulo, -1, -1, -1, false);
	PutPlayerInVehicle(playerid, veiculo, 0);

	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Veiculo spawnado.");
	return 1;
}