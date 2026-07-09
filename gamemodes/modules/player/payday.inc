stock GetSalario(playerid)
{
	switch (Profissao[playerid])
	{
		case PROFISSAO_PIZZAIOLO: return 450;
		default: return 0;
	}
	return 0;
}

stock GetXPNecessaria(nivel)
{
	return 4 + ((nivel - 1) * 2);
}

stock ExecutarPayday(playerid)
{
	new pagamento = GetSalario(playerid);

	if (pagamento > 0)
	{
		SaldoBanco[playerid] += pagamento;

		new stringPayday[144];
		format(stringPayday, sizeof(stringPayday), "Payday! Voce recebeu $%d diretamente na sua conta bancaria.", pagamento);
		SendClientMessage(playerid, 0x00FF00FF, stringPayday);
	}

	TempoJogado[playerid]++;

	new necessaria = GetXPNecessaria(Nivel[playerid]);

	if (TempoJogado[playerid] >= necessaria)
	{
		Nivel[playerid]++;
		SetPlayerScore(playerid, Nivel[playerid]);
		TempoJogado[playerid] -= necessaria;

		new stringLevel[100];
		format(stringLevel, sizeof(stringLevel), "Parabens! Voce subiu para o nivel %d.", Nivel[playerid]);
		SendClientMessage(playerid, 0xFFFF00FF, stringLevel);
	}

	MinutosPayday[playerid] = 0;
	SalvarConta(playerid);
	return 1;
}

public AtualizarSistema()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (IsPlayerConnected(i) && IsLoggedIn[i])
		{
			MinutosPayday[i]++;

			if (MinutosPayday[i] >= 60)
			{
				ExecutarPayday(i);
			}
		}
	}
	return 1;
}