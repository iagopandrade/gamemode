static LastCommand[MAX_PLAYERS];

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
    if (!HasPlayerLogged(playerid)) {
		GameTextForPlayer(playerid, "Voce precisa estar logado.", 3000, 3);
        return 0;
    }

	if (GetTickCount() - LastCommand[playerid] < 500) {
		GameTextForPlayer(playerid, "Aguarde o comando atual terminar.", 3000, 3);
        return 0;
    }

    LastCommand[playerid] = GetTickCount();
    return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
	if (result == -1) {
		GameTextForPlayer(playerid, "~r~Comando inexistente", 3000, 3);
	}
    return 1;
}

/*
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
*/

CMD:money(playerid, params[])
{
	GivePlayerMoney(playerid, 10000);
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	PlayerPlaySound(playerid, 5405, x, y, z);
	return 1;
}
