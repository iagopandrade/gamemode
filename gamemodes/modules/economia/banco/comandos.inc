CMD:banco(playerid, params[])
{
	if (!DentroBanco[playerid]) {
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar dentro do Banco para usar este comando.");
		return 1;
	}
	if (!IsPlayerInRangeOfPoint(playerid, 2.0, BalcaoBancoPos[0], BalcaoBancoPos[1], BalcaoBancoPos[2])) {
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar perto do balcao do banco.");
		return 1;
	}
	Dialog_Show(playerid, DIALOG_BANCO, DIALOG_STYLE_LIST, "Banco", "Ver Saldo\nDepositar\nSacar", "Selecionar", "Fechar");
	return 1;
}

CMD:sairbanco(playerid, params[])
{
	if (!DentroBanco[playerid]) {
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao esta no Banco.");
		return 1;
	}

	SetPlayerPos(playerid, BancoEntradaPos[0], BancoEntradaPos[1], BancoEntradaPos[2]);
	DentroBanco[playerid] = false;
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce saiu do Banco.");
	return 1;
}

CMD:pagar(playerid, params[])
{
	new id, valor;

	if (sscanf(params, "ui", id, valor))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Uso: /pagar [id] [valor]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao pode pagar para si mesmo.");
		return 1;
	}

	if (valor <= 0)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Valor invalido.");
		return 1;
	}

	if (valor > GetPlayerMoney(playerid))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem esse valor em maos.");
		return 1;
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (!IsPlayerInRangeOfPoint(id, 3.0, x, y, z))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Esse jogador esta longe demais para receber o pagamento.");
		return 1;
	}

	GivePlayerMoney(playerid, -valor);
	GivePlayerMoney(id, valor);

	new nomeQuemPaga[MAX_PLAYER_NAME], nomeQuemRecebe[MAX_PLAYER_NAME], string[144];
	GetPlayerName(playerid, nomeQuemPaga, sizeof(nomeQuemPaga));
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	format(string, sizeof(string), "Voce pagou $%d para %s.", valor, nomeQuemRecebe);
	SendClientMessage(playerid, 0x00FF00FF, string);

	format(string, sizeof(string), "Voce recebeu $%d de %s.", valor, nomeQuemPaga);
	SendClientMessage(id, 0x00FF00FF, string);
	return 1;
}