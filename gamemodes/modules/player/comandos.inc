CMD:stats(playerid, params[])
{
	new nome[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nome, sizeof(nome));

	new necessaria = GetXPNecessaria(Nivel[playerid]);

	new nomeProfissao[32];
	switch (Profissao[playerid])
	{
		case PROFISSAO_PIZZAIOLO: nomeProfissao = "Pizzaiolo";
		default: nomeProfissao = "Desempregado";
	}

	new string[512];
	format(string, sizeof(string),
		"{FFFF00}Nome: {FFFFFF}%s\n"\
		"{FFFF00}Profissao: {FFFFFF}%s\n"\
		"{FFFF00}Salario: {FFFFFF}$%d por payday\n"\
		"{FFFF00}Level: {FFFFFF}%d\n"\
		"{FFFF00}Dinheiro na mao: {FFFFFF}$%d\n"\
		"{FFFF00}Experiencia: {FFFFFF}%d/%d",
		nome, nomeProfissao, GetSalario(playerid), Nivel[playerid], GetPlayerMoney(playerid), TempoJogado[playerid], necessaria
	);

	ShowPlayerDialog(playerid, DIALOG_STATS, DIALOG_STYLE_MSGBOX, "{FFFF00}Estatisticas", string, "Fechar", "");
	return 1;
}

CMD:carregarp(playerid, params[])
{
	if (!IsLoggedIn[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar logado."); return 1; }
	if (UltimaPosX[playerid] == 0.0 && UltimaPosY[playerid] == 0.0 && UltimaPosZ[playerid] == 0.0)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Nao ha nenhuma posicao salva ainda.");
		return 1;
	}

	SetPlayerInterior(playerid, UltimoInterior[playerid]);
	SetPlayerPos(playerid, UltimaPosX[playerid], UltimaPosY[playerid], UltimaPosZ[playerid]);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce foi levado de volta ao local onde estava antes de sair.");
	return 1;
}