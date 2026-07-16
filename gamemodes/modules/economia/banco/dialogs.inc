enum {
	DIALOG_BANCO,
	DIALOG_BANCO_DEPOSITAR,
	DIALOG_BANCO_SACAR,
	DIALOG_BANCO_SALDO
}

Dialog:DIALOG_BANCO_DEPOSITAR(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new valor = strval(inputtext);

	if (valor <= 0) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Valor invalido."); return 1; }
	if (valor > GetPlayerMoney(playerid)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem esse valor em maos."); return 1; }

	GivePlayerMoney(playerid, -valor);
	SaldoBanco[playerid] += valor;
	SalvarConta(playerid);

	new string[100];
	format(string, sizeof(string), "[BC:RP] Voce depositou $%d. Novo saldo: $%d", valor, SaldoBanco[playerid]);
	SendClientMessage(playerid, COR_BCRP_DINHEIRO, string);
	return 1;
}

Dialog:DIALOG_BANCO_SACAR(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new valor = strval(inputtext);

	if (valor <= 0) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Valor invalido."); return 1; }
	if (valor > SaldoBanco[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao tem esse valor em conta."); return 1; }

	SaldoBanco[playerid] -= valor;
	GivePlayerMoney(playerid, valor);
	SalvarConta(playerid);

	new string[100];
	format(string, sizeof(string), "[BC:RP] Voce sacou $%d. Novo saldo: $%d", valor, SaldoBanco[playerid]);
	SendClientMessage(playerid, COR_BCRP_DINHEIRO, string);
	return 1;
}

Dialog:DIALOG_BANCO(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (listitem == 0)
	{
		new string[144];
		format(string, sizeof(string), "Saldo em conta: $%d\nDinheiro na mao: $%d", SaldoBanco[playerid], GetPlayerMoney(playerid));
		Dialog_Show(playerid, DIALOG_BANCO_SALDO, DIALOG_STYLE_MSGBOX, "Banco - Saldo", string, "Voltar", "Fechar");
	}
	else if (listitem == 1)
	{
		Dialog_Show(playerid, DIALOG_BANCO_DEPOSITAR, DIALOG_STYLE_INPUT, "Depositar", "Digite o valor que deseja depositar:", "Depositar", "Cancelar");
	}
	else if (listitem == 2)
	{
		Dialog_Show(playerid, DIALOG_BANCO_SACAR, DIALOG_STYLE_INPUT, "Sacar", "Digite o valor que deseja sacar:", "Sacar", "Cancelar");
	}
	return 1;
}

Dialog:DIALOG_BANCO_SALDO(playerid, response, listitem, inputtext[])
{
	if (response)
	{
		Dialog_Show(playerid, DIALOG_BANCO, DIALOG_STYLE_LIST,
			"Banco",
			"Ver Saldo\n\
			Depositar\n\
			Sacar",
			"Selecionar", "Fechar"
		);
	}
	return 1;
}