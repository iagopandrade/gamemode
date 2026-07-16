new AgenciaPickup;
new Float:AgenciaSaidaPos[3] = {-2029.4583, -119.0372, 1035.1719};
new Float:AgenciaMaletaInternaPos[3] = {-2033.1250, -117.3180, 1035.1719};

public OnPlayerPickUpPickup(playerid, pickupid)
{
	if (pickupid == AgenciaPickup)
	{
		Dialog_Show(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if (newkeys & KEY_SECONDARY_ATTACK)
	{
		// Maleta de FORA -> entra na agencia
		if (!DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2]) && GetPlayerInterior(playerid) == 0)
		{
			SetPlayerInterior(playerid, 3);
			SetPlayerPos(playerid, AgenciaSaidaPos[0], AgenciaSaidaPos[1], AgenciaSaidaPos[2]);
			DentroAgencia[playerid] = true;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce entrou na Agencia de Emprego.");
			return 1;
		}

		// Maleta de DENTRO -> abre o catalogo de empregos
		if (DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaMaletaInternaPos[0], AgenciaMaletaInternaPos[1], AgenciaMaletaInternaPos[2]))
		{
			Dialog_Show(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
			return 1;
		}

		// Porta de SAIDA da Agencia -> volta pro mundo normal
		if (DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaSaidaPos[0], AgenciaSaidaPos[1], AgenciaSaidaPos[2]))
		{
			SetPlayerInterior(playerid, 0);
			SetPlayerPos(playerid, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2] + 1.0);
			DentroAgencia[playerid] = false;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce saiu da Agencia de Emprego.");
			return 1;
		}
    }
	return 1;
}