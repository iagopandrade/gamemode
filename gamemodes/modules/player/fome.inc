new Text:TDFome;
new Text:TDSede;

public AtualizarFomeSede()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (IsPlayerConnected(i) && IsLoggedIn[i])
		{
			if (Fome[i] > 0) Fome[i] -= 2;
			if (Sede[i] > 0) Sede[i] -= 3;

			if (Fome[i] < 0) Fome[i] = 0;
			if (Sede[i] < 0) Sede[i] = 0;

			new strFome[32], strSede[32];
			format(strFome, sizeof(strFome), "Fome: %d%%", Fome[i]);
			format(strSede, sizeof(strSede), "Sede: %d%%", Sede[i]);

			TextDrawSetString(TDFome, strFome);
			TextDrawSetString(TDSede, strSede);

			// Dano quando baixo
			if (Fome[i] <= 15 || Sede[i] <= 15)
			{
				new Float:health;
				GetPlayerHealth(i, health);
				SetPlayerHealth(i, health - 2.0);
			}
		}
	}
	return 1;
}

stock CriarBarrasFomeSede(playerid)
{
	TDFomeBarra[playerid] = CreatePlayerTextDraw(playerid, 590.0, 20.0, "_");
	PlayerTextDrawFont(playerid, TDFomeBarra[playerid], 1);
	PlayerTextDrawLetterSize(playerid, TDFomeBarra[playerid], 0.2, 1.0);
	PlayerTextDrawTextSize(playerid, TDFomeBarra[playerid], 30.0, 10.0);
	PlayerTextDrawUseBox(playerid, TDFomeBarra[playerid], 1);
	PlayerTextDrawBoxColor(playerid, TDFomeBarra[playerid], 0xFFA500FF);
	PlayerTextDrawColor(playerid, TDFomeBarra[playerid], 0x00000000);
	PlayerTextDrawShow(playerid, TDFomeBarra[playerid]);

	TDSedeBarra[playerid] = CreatePlayerTextDraw(playerid, 590.0, 40.0, "_");
	PlayerTextDrawFont(playerid, TDSedeBarra[playerid], 1);
	PlayerTextDrawLetterSize(playerid, TDSedeBarra[playerid], 0.2, 1.0);
	PlayerTextDrawTextSize(playerid, TDSedeBarra[playerid], 30.0, 10.0);
	PlayerTextDrawUseBox(playerid, TDSedeBarra[playerid], 1);
	PlayerTextDrawBoxColor(playerid, TDSedeBarra[playerid], 0x00BFFFFF);
	PlayerTextDrawColor(playerid, TDSedeBarra[playerid], 0x00000000);
	PlayerTextDrawShow(playerid, TDSedeBarra[playerid]);

	TextDrawShowForPlayer(playerid, TDFomeIcone);
	TextDrawShowForPlayer(playerid, TDSedeIcone);
	return 1;
}

stock AtualizarBarra(playerid, PlayerText:barra, valor)
{
	new Float:largura = (30.0 * valor) / 100;
	PlayerTextDrawTextSize(playerid, barra, largura, 10.0);
	PlayerTextDrawShow(playerid, barra);
	return 1;
}
