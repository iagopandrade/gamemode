Dialog:DIALOG_EMPREGOS(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (listitem == 0)
	{
		Profissao[playerid] = PROFISSAO_PIZZAIOLO;
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce agora e Pizzaiolo!");
		SendClientMessage(playerid, 0xFFFF00FF, "Trabalho marcado no mapa - em caso de duvida, use /ajuda emprego");
		SetPlayerCheckpoint(playerid, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2], 3.0);
	}
	else if (listitem == 1)
	{
		Profissao[playerid] = PROFISSAO_DESEMPREGADO;
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce ficou desempregado.");
	}

	SalvarConta(playerid);
	return 1;
}