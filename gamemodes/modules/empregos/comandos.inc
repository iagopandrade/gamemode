CMD:emprego(playerid, params[])
{
	if (!DentroAgencia[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar na Agencia de Emprego para usar este comando."); return 1; }

	ShowPlayerDialog(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
	return 1;
}

CMD:sairagencia(playerid, params[])
{
	if (!DentroAgencia[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao esta em nenhum lugar pra sair."); return 1; }

	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2] + 1.0);
	DentroAgencia[playerid] = false;
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce saiu da Agencia de Emprego.");
	return 1;
}

CMD:ajuda(playerid, params[])
{
	if (strcmp(params, "emprego", true) == 0)
	{
		new string[1024];
		strcat(string, "{FFFFFF}/trabalhar {AAAAAA}- Inicia ou encerra seu turno (muda sua skin e spawna a Faggio)\n");
		strcat(string, "{FFFFFF}/prepararpizza {AAAAAA}- Pega uma caixa de pizza no balcao\n");
		strcat(string, "{FFFFFF}/guardarpizza {AAAAAA}- Guarda a pizza na Faggio (max 5)\n");
		strcat(string, "{FFFFFF}/retirarcaixa {AAAAAA}- Retira uma pizza guardada na Faggio\n");
		strcat(string, "{FFFFFF}/rotapizza {AAAAAA}- Inicia a rota de entregas (precisa estar na Faggio)\n");
		strcat(string, "{FFFFFF}/entregarpedido {AAAAAA}- Entrega a pizza na casa marcada no radar");

		ShowPlayerDialog(playerid, DIALOG_AJUDAEMPREGO, DIALOG_STYLE_MSGBOX, "Ajuda - Emprego Pizzaiolo", string, "Fechar", "");
		return 1;
	}

	SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /ajuda emprego");
	return 1;
}