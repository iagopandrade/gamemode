CMD:trabalhar(playerid, params[])
{
	if (Profissao[playerid] != PROFISSAO_PIZZAIOLO)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa ser Pizzaiolo para usar este comando.");
		return 1;
	}

	if (!Trabalhando[playerid])
	{
		SkinAntesTrabalho[playerid] = GetPlayerSkin(playerid);
		SetPlayerSkin(playerid, 155);
		Trabalhando[playerid] = true;
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce entrou em modo de trabalho. Va ate a garagem e aperte F.");
	}
	else
	{
		SetPlayerSkin(playerid, SkinAntesTrabalho[playerid]);
		Trabalhando[playerid] = false;
		CarregandoPizza[playerid] = false;
		PizzasNaMoto[playerid] = 0;
		EmRota[playerid] = false;

		if (FaggioVeiculo[playerid] != 0)
		{
			DestroyVehicle(FaggioVeiculo[playerid]);
			FaggioVeiculo[playerid] = 0;
		}

		DisablePlayerCheckpoint(playerid);
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce saiu do modo de trabalho.");
	}
	return 1;
}

CMD:prepararpizza(playerid, params[])
{
	if (!Trabalhando[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar trabalhando. Use /trabalhar."); return 1; }
	if (!IsPlayerInRangeOfPoint(playerid, 2.0, PizzaPickupPos[0], PizzaPickupPos[1], PizzaPickupPos[2])) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar perto do balcao."); return 1; }
	if (PizzasNaMoto[playerid] >= 5) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Sua Faggio ja esta cheia."); return 1; }
	if (CarregandoPizza[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce ja esta carregando uma pizza."); return 1; }

	CarregandoPizza[playerid] = true;
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce pegou uma pizza. Va ate a Faggio e use /guardarpizza.");
	return 1;
}

CMD:guardarpizza(playerid, params[])
{
	if (!Trabalhando[playerid] || !CarregandoPizza[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao esta carregando pizza."); return 1; }
	if (FaggioVeiculo[playerid] == 0) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Sua Faggio nao existe."); return 1; }

	new Float:vx, Float:vy, Float:vz;
	GetVehiclePos(FaggioVeiculo[playerid], vx, vy, vz);

	if (!IsPlayerInRangeOfPoint(playerid, 3.0, vx, vy, vz)) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Chegue mais perto da Faggio."); return 1; }

	CarregandoPizza[playerid] = false;
	PizzasNaMoto[playerid]++;

	new string[64];
	format(string, sizeof(string), "[BC:RP] Pizza guardada (%d/5)", PizzasNaMoto[playerid]);
	SendClientMessage(playerid, COR_BCRP_INFO, string);
	return 1;
}

CMD:retirarcaixa(playerid, params[])
{
	if (PizzasNaMoto[playerid] <= 0)
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Nao ha pizzas guardadas na Faggio.");
		return 1;
	}

	PizzasNaMoto[playerid]--;
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce retirou uma caixa de pizza da Faggio.");
	return 1;
}

CMD:rotapizza(playerid, params[])
{
	if (!Trabalhando[playerid] || FaggioVeiculo[playerid] == 0 || !IsPlayerInVehicle(playerid, FaggioVeiculo[playerid]))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce precisa estar na sua Faggio trabalhando.");
		return 1;
	}
	if (PizzasNaMoto[playerid] <= 0) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Guarde pelo menos uma pizza."); return 1; }
	if (EmRota[playerid]) { SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce ja esta em rota."); return 1; }

	new ordem[5] = {0,1,2,3,4};
	for (new i = 4; i > 0; i--)
	{
		new j = random(i+1), temp = ordem[i];
		ordem[i] = ordem[j]; ordem[j] = temp;
	}
	for (new i = 0; i < 5; i++) RotaOrdem[playerid][i] = ordem[i];

	EntregasRestantes[playerid] = PizzasNaMoto[playerid];
	EntregaAtual[playerid] = 0;
	EmRota[playerid] = true;

	SetPlayerCheckpoint(playerid, EntregasPos[RotaOrdem[playerid][0]][0], EntregasPos[RotaOrdem[playerid][0]][1], EntregasPos[RotaOrdem[playerid][0]][2], 4.0);
	SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Rota iniciada!");
	return 1;
}

CMD:entregarpedido(playerid, params[])
{
	if (!EmRota[playerid])
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce nao esta em rota de entrega.");
		return 1;
	}

	new casa = RotaOrdem[playerid][EntregaAtual[playerid]];

	if (!IsPlayerInRangeOfPoint(playerid, 4.0, EntregasPos[casa][0], EntregasPos[casa][1], EntregasPos[casa][2]))
	{
		SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Chegue mais perto da casa.");
		return 1;
	}

	PizzasNaMoto[playerid]--;
	EntregasRestantes[playerid]--;

	new nomes[5][20] = {"Rose", "Carlos", "Ana", "Roberto", "Juliana"};
	new gorjeta = 150 + random(51);
	GivePlayerMoney(playerid, gorjeta);

	new string[144];
	format(string, sizeof(string), "Pizza entregue - %s: Obrigado! Gorjeta: $%d", nomes[random(5)], gorjeta);
	SendClientMessage(playerid, 0x006400FF, string);

	EntregaAtual[playerid]++;

	if (EntregasRestantes[playerid] <= 0)
	{
		EmRota[playerid] = false;
		DisablePlayerCheckpoint(playerid);
		PizzasNaMoto[playerid] = 0;
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Rota finalizada!");
	}
	else
	{
		new prox = RotaOrdem[playerid][EntregaAtual[playerid]];
		SetPlayerCheckpoint(playerid, EntregasPos[prox][0], EntregasPos[prox][1], EntregasPos[prox][2], 4.0);
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Siga para a proxima casa.");
	}
	return 1;
}