new Float:BancoEntradaPos[3] = {1467.2704, -1011.1876, 26.8438};
new Float:BancoInternoPos[3] = {2306.3826, -15.2365, 26.7496};
new bool:DentroBanco[MAX_PLAYERS];

new Float:BalcaoBancoPos[3] = {2315.8311, -10.0434, 26.7422};
new Float:SaidaBancoPos[3] = {2306.3047, -16.0743, 26.7496};

hook OnPlayerKeyStateChange@2(playerid, KEY:newkeys, KEY:oldkeys)
{
	if (newkeys & KEY_SECONDARY_ATTACK)
	{
		// Entrada do BANCO
		if (!DentroBanco[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, BancoEntradaPos[0], BancoEntradaPos[1], BancoEntradaPos[2]))
		{
			SetPlayerPos(playerid, BancoInternoPos[0], BancoInternoPos[1], BancoInternoPos[2]);
			DentroBanco[playerid] = true;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce entrou no Banco. Use /banco para acessar sua conta, ou /sairbanco para sair.");
			return 1;
		}
		// Saida do BANCO com F
		if (DentroBanco[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, SaidaBancoPos[0], SaidaBancoPos[1], SaidaBancoPos[2]))
		{
			SetPlayerPos(playerid, BancoEntradaPos[0], BancoEntradaPos[1], BancoEntradaPos[2]);
			DentroBanco[playerid] = false;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Voce saiu do Banco.");
			return 1;
		}
	}
	return 1;
}