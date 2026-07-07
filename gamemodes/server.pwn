#include <a_samp>
#include <sscanf2>
#include <streamer>
#include <zcmd>
#include <dof2>
forward AtualizarSistema();


//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - VARIAVEIS E DEFINES
//////////////////////////////////////////////////

#define DIALOG_ACMDS 102
#define LEVEL_HELPER      1
#define LEVEL_GAMEADMIN   2
#define LEVEL_ENCARREGADO 3
#define LEVEL_FUNDADOR    4
#define DIALOG_STATS 104
#define DIALOG_EMPREGOS 105


new AdminLevel[MAX_PLAYERS];
new bool:AdminDuty[MAX_PLAYERS];
new Warnings[MAX_PLAYERS];
new bool:Muted[MAX_PLAYERS];
new bool:Jailed[MAX_PLAYERS];
new Float:PosAntesEspectar[MAX_PLAYERS][3];
new bool:Espectando[MAX_PLAYERS];
new Nivel[MAX_PLAYERS];
new TempoJogado[MAX_PLAYERS]; // em segundos, desde o ultimo level up
new MinutosPayday[MAX_PLAYERS]; // contador ate completar 60 minutos
new Float:AgenciaPos[3] = {1412.3226, -1700.2135, 13.5395};
new bool:DentroAgencia[MAX_PLAYERS];
new Text:LogoServidor;
new SkinSalva[MAX_PLAYERS];
new Float:UltimaPosX[MAX_PLAYERS];
new Float:UltimaPosY[MAX_PLAYERS];
new Float:UltimaPosZ[MAX_PLAYERS];
new UltimoInterior[MAX_PLAYERS];

///////////////////////////////////////////////////ESTABELECIMENTOS////////////
new Float:PizzariaLSPos[3] = {2104.9644,-1806.5123,13.5547};   
new Float:PizzariaLSSaidaPos[3] = {372.5565, -131.3607, 1001.4922};
new bool:DentroPizzariaLS[MAX_PLAYERS];   

///////////////////////////////////////////////////AGENCIA DE EMPREGO//////////
#define PROFISSAO_DESEMPREGADO 0
#define PROFISSAO_PIZZAIOLO    1

new Profissao[MAX_PLAYERS];
new AgenciaPickup;
new Float:AgenciaSaidaPos[3] = {-2029.4583, -119.0372, 1035.1719};
new Float:AgenciaMaletaInternaPos[3] = {-2033.1250, -117.3180, 1035.1719};

//////////////////////////////////////////////////
// SISTEMA DE BANCO
//////////////////////////////////////////////////

#define DIALOG_BANCO 107
#define DIALOG_BANCO_DEPOSITAR 108
#define DIALOG_BANCO_SACAR 109

new Float:BancoEntradaPos[3] = {1467.2704, -1011.1876, 26.8438};
new Float:BancoInternoPos[3] = {2306.3826, -15.2365, 26.7496};
new bool:DentroBanco[MAX_PLAYERS];
new SaldoBanco[MAX_PLAYERS];

//////////////////////////////////////////////////
// SISTEMA DE EMPREGO - PIZZAIOLO
//////////////////////////////////////////////////

#define DIALOG_AJUDAEMPREGO 106

new bool:Trabalhando[MAX_PLAYERS];
new SkinAntesTrabalho[MAX_PLAYERS];
new bool:CarregandoPizza[MAX_PLAYERS];
new PizzasNaMoto[MAX_PLAYERS];
new FaggioVeiculo[MAX_PLAYERS];
new bool:EmRota[MAX_PLAYERS];
new EntregaAtual[MAX_PLAYERS];
new EntregasRestantes[MAX_PLAYERS];
new RotaOrdem[MAX_PLAYERS][5];

new Float:PizzaPickupPos[3] = {378.3459, -114.3801, 1001.4922};
new Float:GaragemPizzaPos[3] = {2094.7092, -1816.9680, 13.3828};

new Float:EntregasPos[5][3] = {
	{2068.2512, -1731.6548, 13.8762},
	{2069.1462, -1628.9583, 13.8762},
	{2016.5723, -1629.7964, 13.5469},
	{1969.4503, -1655.8328, 15.9688},
	{2156.9600, -1708.5455, 15.0859}
};

forward VerificarFaggio(playerid);

////////////////////////////////////SISTEMA DE LOGIN///////////////////////////
#define DIALOG_LOGIN    100
#define DIALOG_REGISTER 101
#define DIALOG_SKIN 103

new bool:IsLoggedIn[MAX_PLAYERS];
new SenhaHash[MAX_PLAYERS];
new SkinAtual[MAX_PLAYERS];

stock UserPath(playerid)
{
	new name[MAX_PLAYER_NAME], path[64];
	GetPlayerName(playerid, name, sizeof(name));
	format(path, sizeof(path), "Users/%s.ini", name);
	return path;
}

stock HashSenha(senha[])
{
	new hash = 0;
	for (new i = 0; i < strlen(senha); i++) hash = (hash * 31) + senha[i];
	return hash;
}
stock SalvarConta(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new File:conta = fopen(UserPath(playerid), io_write);
	new linha[280];
	format(linha, sizeof(linha), "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%f\r\n%f\r\n%f\r\n%d\r\n",
		SenhaHash[playerid], AdminLevel[playerid], GetPlayerSkin(playerid), GetPlayerMoney(playerid),
		Nivel[playerid], TempoJogado[playerid], MinutosPayday[playerid], Profissao[playerid], SaldoBanco[playerid],
		x, y, z, GetPlayerInterior(playerid));
	fwrite(conta, linha);
	fclose(conta);
	return 1;
}
stock MostrarSkin(playerid)
{
	new titulo[64];
	format(titulo, sizeof(titulo), "Skin atual: ID %d", SkinAtual[playerid]);
	ShowPlayerDialog(playerid, DIALOG_SKIN, DIALOG_STYLE_LIST, titulo, "Proxima Skin >>\n<< Skin Anterior\nConfirmar Skin", "Selecionar", "");
	return 1;
}
stock bool:SkinDePolicia(skinid)
{
	if (skinid >= 265 && skinid <= 267) return true; // Tenpenny, Pulaski, Hernandez
	if (skinid >= 280 && skinid <= 288) return true; // LAPD, SFPD, LVPD, Sheriff, SWAT, FBI, Army
	if (skinid >= 301 && skinid <= 307) return true; // variantes de policia sem colete
	return false;
}


////////////////////////////////////////////////////////////////////////////////

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print(" Blank Filterscript by your name here");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

#else

main()
{
	print("\n----------------------------------");
	print(" Blank Script");
	print("----------------------------------\n");
}

#endif

public OnGameModeInit()
{
	SetGameModeText("Blank Script");
	AddPlayerClass(0, 1685.8695, -2241.0386, -2.6973, 179.8575, 0, 0, 0, 0, 0, 0);
	DisableInteriorEnterExits();
	LogoServidor = TextDrawCreate(320.0, 20.0, "Brasil City");
    TextDrawFont(LogoServidor, 3);
    TextDrawLetterSize(LogoServidor, 0.7, 3.0);
    TextDrawColor(LogoServidor, 0x006400FF);
    TextDrawSetOutline(LogoServidor, 1);
    TextDrawSetShadow(LogoServidor, 0);
    TextDrawAlignment(LogoServidor, 2);
    TextDrawUseBox(LogoServidor, 1);
    TextDrawBoxColor(LogoServidor, 0x00000090);
    TextDrawBackgroundColor(LogoServidor, 0x000000FF);
	SetTimer("AtualizarSistema", 60000, true);

	CreatePickup(1210, 0, 1412.3226, -1700.2135, 13.5395);
	CreateDynamic3DTextLabel("Agencia de Emprego\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 1412.3226, -1700.2135, 14.0395, 20.0);///agencia de emprego

	CreatePickup(1210, 1, -2033.1250, -117.3180, 1035.1719);
    CreateDynamic3DTextLabel("Catalogo de Empregos\n{FFFFFF}Aperte F", 0xFFFF00FF, -2033.1250, -117.3180, 1035.6719, 20.0);///agencia de emprego

	CreatePickup(1318, 0, 2104.9644,-1806.5123,13.5547); // troque 1210 pelo icone que quiser (ver lista de pickups)
    CreateDynamic3DTextLabel("Pizzaria\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 2104.9644,-1806.5123,13.5547 + 0.5, 20.0); 

	CreatePickup(1212, 1, 375.7113,-119.2369,1001.4995);
	CreateDynamic3DTextLabel("Balcao de Atendimento\n{FFFFFF}Aperte F para acessar o menu de pedido", 0xFFFF00FF, 375.7113,-119.2369,1001.4995 + 0.5, 20.0);

	CreatePickup(1582, 1, 378.3459, -114.3801, 1001.4922);
    CreateDynamic3DTextLabel("Use /prepararpizza para pegar uma pizza", 0xFFFF00FF, 378.3459, -114.3801, 1001.9922, 20.0);
	
	CreatePickup(1318, 1, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]);
	CreateDynamic3DTextLabel("Garagem do Pizzaiolo\n{FFFFFF}Aperte F para spawnar sua Faggio", 0xFFFF00FF, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]+0.5, 15.0);

	CreatePickup(1210, 0, 1467.2704, -1011.1876, 26.8438);
    CreateDynamic3DTextLabel("Banco\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 1467.2704, -1011.1876, 27.3438, 20.0);

	return 1;
}

public OnGameModeExit()
{
    DOF2_Exit();
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetSpawnInfo(playerid, 0, 0, 1685.8695, -2241.0386, -2.6973, 179.8575, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return 1;
}

//////////////////////////////////////////////////
// SISTEMA DE NIVEL E PAYDAY
//////////////////////////////////////////////////

public AtualizarSistema()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (IsPlayerConnected(i) && IsLoggedIn[i])
		{
			TempoJogado[i] += 60;
			MinutosPayday[i]++;

			if (MinutosPayday[i] >= 60)
			{
				new pagamento = 100 + (Nivel[i] * 50);
				GivePlayerMoney(i, pagamento);

				new stringPayday[100];
				format(stringPayday, sizeof(stringPayday), "Payday! Voce recebeu $%d.", pagamento);
				SendClientMessage(i, 0x00FF00FF, stringPayday);

				MinutosPayday[i] = 0;
				SalvarConta(i);
			}

			new horasNecessarias = (Nivel[i] < 7) ? (4 * 3600) : (8 * 3600);

			if (TempoJogado[i] >= horasNecessarias)
			{
				Nivel[i]++;
                SetPlayerScore(i, Nivel[i]);
                TempoJogado[i] -= horasNecessarias;

				new stringLevel[100];
				format(stringLevel, sizeof(stringLevel), "Parabens! Voce subiu para o nivel %d.", Nivel[i]);
				SendClientMessage(i, 0xFFFF00FF, stringLevel);

				SalvarConta(i);
			}
		}
	}
	return 1;
}

public OnPlayerConnect(playerid)
{
	IsLoggedIn[playerid] = false;
	TogglePlayerControllable(playerid, false);

	if (fexist(UserPath(playerid)))
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - {00FF00}BRASIL PROJETO CITY", "Esta conta ja existe.\nDigite sua senha para entrar:", "Entrar", "Sair");
	else
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Login - {00FF00}BRASIL PROJETO CITY", "Esta conta nao existe.\nDigite uma senha para cria-la:", "Registrar", "Sair");
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (IsLoggedIn[playerid])
	{
		SalvarConta(playerid);
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if (!IsLoggedIn[playerid])
	{
		TogglePlayerControllable(playerid, false);
	}
	else
	{
		SetPlayerSkin(playerid, SkinSalva[playerid]);
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	SpawnPlayer(playerid);
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp("/mycommand", cmdtext, true, 10) == 0)
	{
		// Do something here
		return 1;
	}
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	// Checkpoint inicial ao aceitar emprego
	if (Profissao[playerid] == PROFISSAO_PIZZAIOLO && !Trabalhando[playerid])
	{
		if (IsPlayerInRangeOfPoint(playerid, 5.0, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2]))
		{
			DisablePlayerCheckpoint(playerid);
			SendClientMessage(playerid, 0xFFFF00FF, "Voce chegou na Pizzaria! Use /trabalhar para iniciar.");
			return 1;
		}
	}

	// Dica para entrega
	if (EmRota[playerid])
	{
		SendClientMessage(playerid, -1, "Use /entregarpedido para entregar a pizza!");
	}
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	if (pickupid == AgenciaPickup)
	{
		ShowPlayerDialog(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
	}
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_SECONDARY_ATTACK)
	{
		// Maleta de FORA -> entra na agencia
		if (!DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2]) && GetPlayerInterior(playerid) == 0)
		{
			SetPlayerInterior(playerid, 3);
			SetPlayerPos(playerid, AgenciaSaidaPos[0], AgenciaSaidaPos[1], AgenciaSaidaPos[2]);
			DentroAgencia[playerid] = true;
			SendClientMessage(playerid, -1, "Voce entrou na Agencia de Emprego.");
			return 1;
		}

		// Maleta de DENTRO -> abre o catalogo de empregos
		if (DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaMaletaInternaPos[0], AgenciaMaletaInternaPos[1], AgenciaMaletaInternaPos[2]))
		{
			ShowPlayerDialog(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
			return 1;
		}

		// Porta de SAIDA da Agencia -> volta pro mundo normal
		if (DentroAgencia[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, AgenciaSaidaPos[0], AgenciaSaidaPos[1], AgenciaSaidaPos[2]))
		{
			SetPlayerInterior(playerid, 0);
			SetPlayerPos(playerid, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2] + 1.0);
			DentroAgencia[playerid] = false;
			SendClientMessage(playerid, -1, "Voce saiu da Agencia de Emprego.");
			return 1;
		}

		// Porta de FORA da Pizzaria -> entra
		if (!DentroPizzariaLS[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2]) && GetPlayerInterior(playerid) == 0)
		{
			SetPlayerInterior(playerid, 5);
			SetPlayerPos(playerid, PizzariaLSSaidaPos[0], PizzariaLSSaidaPos[1], PizzariaLSSaidaPos[2]);
			DentroPizzariaLS[playerid] = true;
			SendClientMessage(playerid, -1, "Bem vindo, senhor(a). Se aproxime ao balcao para ser atendida.");
			return 1;
		}

		// Porta de SAIDA da Pizzaria -> volta pro mundo normal
		if (DentroPizzariaLS[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, PizzariaLSSaidaPos[0], PizzariaLSSaidaPos[1], PizzariaLSSaidaPos[2]))
		{
			SetPlayerInterior(playerid, 0);
			SetPlayerPos(playerid, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2] + 1.0);
			DentroPizzariaLS[playerid] = false;
			SendClientMessage(playerid, -1, "Volte sempre, senhor(a)!");
			return 1;
		}

		// GARAGEM PIZZAIOLO - SPAWN FAGGIO
		if (Trabalhando[playerid] && IsPlayerInRangeOfPoint(playerid, 3.0, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]))
		{
			if (FaggioVeiculo[playerid] != 0)
				return SendClientMessage(playerid, -1, "Voce ja tem uma Faggio spawnada.");

			new Float:x = GaragemPizzaPos[0] + 1.5;
			new Float:y = GaragemPizzaPos[1] + 2.0;
			new Float:z = GaragemPizzaPos[2];

			FaggioVeiculo[playerid] = CreateVehicle(448, x, y, z, 90.0, -1, -1, -1, false);
			PutPlayerInVehicle(playerid, FaggioVeiculo[playerid], 0);

			SendClientMessage(playerid, -1, "Faggio spawnada! Use /rotapizza quando estiver pronto.");
			return 1;
		}

		// Entrada do BANCO
		if (!DentroBanco[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, BancoEntradaPos[0], BancoEntradaPos[1], BancoEntradaPos[2]))
		{
			SetPlayerPos(playerid, BancoInternoPos[0], BancoInternoPos[1], BancoInternoPos[2]);
			DentroBanco[playerid] = true;
			SendClientMessage(playerid, -1, "Voce entrou no Banco. Use /banco para acessar sua conta, ou /sairbanco para sair.");
			return 1;
		}
	}
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == DIALOG_LOGIN)
	{
		if (!response) { Kick(playerid); return 1; }
		if (isnull(inputtext)) { ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - {00FF00}BRASIL PROJETO CITY", "{FF0000}Senha invalida.\nDigite sua senha para entrar:", "Entrar", "Sair"); return 1; }

        new File:conta = fopen(UserPath(playerid), io_read);
		new linhaSenha[64], linhaAdmin[64], linhaSkin[64], linhaDinheiro[64], linhaNivel[64], linhaTempo[64], linhaPayday[64], linhaProfissao[64], linhaBanco[64], linhaX[64], linhaY[64], linhaZ[64], linhaInterior[64];
		fread(conta, linhaSenha);
		fread(conta, linhaAdmin);
		fread(conta, linhaSkin);
		fread(conta, linhaDinheiro);
		fread(conta, linhaNivel);
		fread(conta, linhaTempo);
		fread(conta, linhaPayday);
		fread(conta, linhaProfissao);
		fread(conta, linhaBanco);
		fread(conta, linhaX);
		fread(conta, linhaY);
		fread(conta, linhaZ);
		fread(conta, linhaInterior);
		fclose(conta);

		if (strval(linhaSenha) == HashSenha(inputtext))
		{
			IsLoggedIn[playerid] = true;
			SenhaHash[playerid] = strval(linhaSenha);
			AdminLevel[playerid] = strval(linhaAdmin);

			SkinSalva[playerid] = strval(linhaSkin);
            SetPlayerSkin(playerid, SkinSalva[playerid]);
			ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, strval(linhaDinheiro));

			Nivel[playerid] = strval(linhaNivel);
			if (Nivel[playerid] < 1) Nivel[playerid] = 1;
			SetPlayerScore(playerid, Nivel[playerid]);
			TempoJogado[playerid] = strval(linhaTempo);
			MinutosPayday[playerid] = strval(linhaPayday);
			Profissao[playerid] = strval(linhaProfissao);
			SaldoBanco[playerid] = strval(linhaBanco);

			TogglePlayerControllable(playerid, true);
			TextDrawShowForPlayer(playerid, LogoServidor);
			SendClientMessage(playerid, -1, "Login efetuado com sucesso!");
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "{FF0000}Senha incorreta.\nDigite sua senha para entrar:", "Entrar", "Sair");
		}
		if (dialogid == DIALOG_BANCO)
        {
			if (!response) return 1; 

			if (listitem == 0) //ver saldo
			{
				new string[144];
				format(string, sizeof(string), "Saldo em conta: $%d\nDinheiro na mao: $%d", SaldoBanco[playerid], GetPlayerMoney(playerid));
				ShowPlayerDialog(playerid, DIALOG_BANCO, DIALOG_STYLE_MSGBOX, "Banco - Saldo", string, "Voltar", "Fechar");
			}
			else if (listitem == 1) //depositar
			{
				ShowPlayerDialog(playerid, DIALOG_BANCO_DEPOSITAR, DIALOG_STYLE_INPUT, "Depositar", "Digite o valor que deseja depositar:", "Depositar", "Cancelar");
			}
			else if (listitem == 2) //sacar
			{
				ShowPlayerDialog(playerid, DIALOG_BANCO_SACAR, DIALOG_STYLE_INPUT, "Sacar", "Digite o valor que deseja sacar:", "Sacar", "Cancelar");
			}
			return 1;
		}
		if (dialogid == DIALOG_BANCO_DEPOSITAR)
		{
			if (!response) return 1; 

			new valor = strval(inputtext);

			if (valor <= 0) { SendClientMessage(playerid, -1, "Valor invalido."); return 1; }
			if (valor > GetPlayerMoney(playerid)) { SendClientMessage(playerid, -1, "Voce nao tem esse valor em maos."); return 1; }

			GivePlayerMoney(playerid, -valor);
			SaldoBanco[playerid] += valor;
			SalvarConta(playerid);

			new string[100];
			format(string, sizeof(string), "Voce depositou $%d. Novo saldo: $%d", valor, SaldoBanco[playerid]);
			SendClientMessage(playerid, -1, string);
			return 1;
		}
		if (dialogid == DIALOG_BANCO_SACAR)
		{
			if (!response) return 1; 

			new valor = strval(inputtext); 

			if (valor <= 0) { SendClientMessage(playerid, -1, "Valor invalido."); return 1; }
			if (valor > SaldoBanco[playerid]) { SendClientMessage(playerid, -1, "Voce nao tem esse valor em conta."); return 1; }

			SaldoBanco[playerid] -= valor;
			UltimaPosX[playerid] = floatstr(linhaX);
			UltimaPosY[playerid] = floatstr(linhaY);
			UltimaPosZ[playerid] = floatstr(linhaZ);
			UltimoInterior[playerid] = strval(linhaInterior);
            SendClientMessage(playerid, 0xFFFF00FF, "Use /carregarp para voltar ao local onde estava antes de sair.");
			GivePlayerMoney(playerid, valor);
			SalvarConta(playerid);

			new string[100];
			format(string, sizeof(string), "Voce sacou $%d. Novo saldo: $%d", valor, SaldoBanco[playerid]);
			SendClientMessage(playerid, -1, string);
			return 1;
		}
	}

	if (dialogid == DIALOG_REGISTER)
	{
		if (!response) { Kick(playerid); return 1; }
		if (isnull(inputtext)) { ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registro", "Digite uma senha valida para criar sua conta:", "Registrar", "Sair"); return 1; }

		SenhaHash[playerid] = HashSenha(inputtext);
		AdminLevel[playerid] = 0;
		SkinAtual[playerid] = 1;
		SetPlayerSkin(playerid, 1);
		MostrarSkin(playerid);
		return 1;
	}

	if (dialogid == DIALOG_SKIN)
	{
		if (!response)
		{
			MostrarSkin(playerid);
			return 1;
		}

		if (listitem == 0) // Proxima Skin
		{
			do
			{
				SkinAtual[playerid]++;
				if (SkinAtual[playerid] > 311) SkinAtual[playerid] = 1;
			}
			while (SkinDePolicia(SkinAtual[playerid]));

			SetPlayerSkin(playerid, SkinAtual[playerid]);
			MostrarSkin(playerid);
		}
		else if (listitem == 1) // Skin Anterior
		{
			do
			{
				SkinAtual[playerid]--;
				if (SkinAtual[playerid] < 1) SkinAtual[playerid] = 311;
			}
			while (SkinDePolicia(SkinAtual[playerid]));

			SetPlayerSkin(playerid, SkinAtual[playerid]);
			MostrarSkin(playerid);
		}
		else if (listitem == 2) // Confirmar Skin
		{
			SkinSalva[playerid] = SkinAtual[playerid];
	        ResetPlayerMoney(playerid);
			ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, 500);
			Nivel[playerid] = 1;
			SetPlayerScore(playerid, 1);
			TempoJogado[playerid] = 0;
			MinutosPayday[playerid] = 0;
			Profissao[playerid] = PROFISSAO_DESEMPREGADO;
			SalvarConta(playerid);
			IsLoggedIn[playerid] = true;
			TogglePlayerControllable(playerid, true);
			TextDrawShowForPlayer(playerid, LogoServidor);
			SendClientMessage(playerid, -1, "Conta criada e login efetuado com sucesso!");
		}
		return 1;
	}

	if (dialogid == DIALOG_EMPREGOS)
	{
		if (!response) return 1;

		if (listitem == 0) // Pizzaiolo
    {
         Profissao[playerid] = PROFISSAO_PIZZAIOLO;
         SendClientMessage(playerid, -1, "Voce agora e Pizzaiolo!");
         SendClientMessage(playerid, 0xFFFF00FF, "Trabalho marcado no mapa - em caso de duvida, use /ajuda emprego");
         SetPlayerCheckpoint(playerid, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2], 3.0);
         SalvarConta(playerid);
    }
		else if (listitem == 1)
		{
			Profissao[playerid] = PROFISSAO_DESEMPREGADO;
			SendClientMessage(playerid, -1, "Voce ficou desempregado.");
		}

		SalvarConta(playerid);
		return 1;
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
CMD:me(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, -1, "Uso: /me [acao]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "* %s %s", name, params);
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}
CMD:do(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, -1, "Uso: /do [descricao]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "* %s (( %s ))", params, name);
	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}
CMD:b(playerid, params[])
{
	if (isnull(params))
	{
		SendClientMessage(playerid, -1, "Uso: /b [mensagem]");
		return 1;
	}

	new string[128], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "((%s: %s))", name, params);
	SendClientMessageToAll(0xAAAAAAFF, string);
	return 1;
}
CMD:pm(playerid, params[])
{
	new id, mensagem[128];

	if (sscanf(params, "us[128]", id, mensagem))
	{
		SendClientMessage(playerid, -1, "Uso: /pm [id] [mensagem]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, -1, "Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, -1, "Voce nao pode mandar PM para si mesmo.");
		return 1;
	}

	new string[144], nomeQuemFala[MAX_PLAYER_NAME], nomeQuemRecebe[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nomeQuemFala, sizeof(nomeQuemFala));
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	format(string, sizeof(string), "[PM de %s]: %s", nomeQuemFala, mensagem);
	SendClientMessage(id, 0xFFFF00FF, string);

	format(string, sizeof(string), "[PM para %s]: %s", nomeQuemRecebe, mensagem);
	SendClientMessage(playerid, 0xFFFF00FF, string);

	return 1;
}
CMD:su(playerid, params[])
{
	new id, mensagem[128];

	if (sscanf(params, "us[128]", id, mensagem))
	{
		SendClientMessage(playerid, -1, "Uso: /su [id] [mensagem]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, -1, "Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, -1, "Voce nao pode sussurrar para si mesmo.");
		return 1;
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (!IsPlayerInRangeOfPoint(id, 3.0, x, y, z))
	{
		SendClientMessage(playerid, -1, "Esse jogador esta longe demais para sussurrar.");
		return 1;
	}

	new string[144], nomeQuemFala[MAX_PLAYER_NAME], nomeQuemRecebe[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nomeQuemFala, sizeof(nomeQuemFala));
	GetPlayerName(id, nomeQuemRecebe, sizeof(nomeQuemRecebe));

	format(string, sizeof(string), "%s sussurra para voce: %s", nomeQuemFala, mensagem);
	SendClientMessage(id, 0xFFFF00FF, string);

	format(string, sizeof(string), "Voce sussurra para %s: %s", nomeQuemRecebe, mensagem);
	SendClientMessage(playerid, 0xFFFF00FF, string);

	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 1 (HELPER)
//////////////////////////////////////////////////

CMD:achat(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }
	if (isnull(params)) { SendClientMessage(playerid, -1, "Uso: /achat [mensagem]"); return 1; }

	new string[144], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(string, sizeof(string), "[ADMIN CHAT] %s: %s", name, params);

	for (new i = 0; i < MAX_PLAYERS; i++)
		if (IsPlayerConnected(i) && AdminLevel[i] >= LEVEL_HELPER)
			SendClientMessage(i, 0x00FFFFFF, string);
	return 1;
}

CMD:servico(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	AdminDuty[playerid] = !AdminDuty[playerid];
	SendClientMessage(playerid, -1, AdminDuty[playerid] ? ("Voce entrou em servico.") : ("Voce saiu de servico."));
	return 1;
}

CMD:irpara(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /irpara [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	new Float:x, Float:y, Float:z;
	GetPlayerPos(id, x, y, z);
	SetPlayerPos(playerid, x, y, z);
	SendClientMessage(playerid, -1, "Voce foi teleportado.");
	return 1;
}

CMD:trazer(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /trazer [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPos(id, x, y, z);
	SendClientMessage(playerid, -1, "Jogador trazido ate voce.");
	return 1;
}

CMD:congelar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /congelar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	TogglePlayerControllable(id, false);
	SendClientMessage(id, -1, "Voce foi congelado por um administrador.");
	SendClientMessage(playerid, -1, "Jogador congelado.");
	return 1;
}

CMD:descongelar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /descongelar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	TogglePlayerControllable(id, true);
	SendClientMessage(id, -1, "Voce foi descongelado.");
	SendClientMessage(playerid, -1, "Jogador descongelado.");
	return 1;
}

CMD:espectar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /espectar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	if (!Espectando[playerid])
	{
		GetPlayerPos(playerid, PosAntesEspectar[playerid][0], PosAntesEspectar[playerid][1], PosAntesEspectar[playerid][2]);
		Espectando[playerid] = true;
	}

	TogglePlayerSpectating(playerid, true);
	PlayerSpectatePlayer(playerid, id);
	SendClientMessage(playerid, -1, "Espectando jogador.");
	return 1;
}

CMD:pararespectar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }
	if (!Espectando[playerid]) { SendClientMessage(playerid, -1, "Voce nao esta espectando ninguem."); return 1; }

	TogglePlayerSpectating(playerid, false);
	SetPlayerPos(playerid, PosAntesEspectar[playerid][0], PosAntesEspectar[playerid][1], PosAntesEspectar[playerid][2]);
	Espectando[playerid] = false;
	SendClientMessage(playerid, -1, "Voce parou de espectar.");
	return 1;
}

CMD:advertir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, -1, "Uso: /advertir [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	Warnings[id]++;
	new string[144];
	format(string, sizeof(string), "Voce recebeu uma advertencia (%d). Motivo: %s", Warnings[id], motivo);
	SendClientMessage(id, -1, string);
	SendClientMessage(playerid, -1, "Jogador advertido.");
	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 2 (GAME ADMIN)
//////////////////////////////////////////////////

CMD:expulsar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, -1, "Uso: /expulsar [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	new string[144];
	format(string, sizeof(string), "Voce foi expulso. Motivo: %s", motivo);
	SendClientMessage(id, -1, string);
	Kick(id);
	SendClientMessage(playerid, -1, "Jogador expulso.");
	return 1;
}

CMD:mutar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /mutar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	Muted[id] = true;
	SendClientMessage(id, -1, "Voce foi mutado por um administrador.");
	SendClientMessage(playerid, -1, "Jogador mutado.");
	return 1;
}

CMD:desmutar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /desmutar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	Muted[id] = false;
	SendClientMessage(id, -1, "Voce foi desmutado.");
	SendClientMessage(playerid, -1, "Jogador desmutado.");
	return 1;
}

CMD:prender(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /prender [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	Jailed[id] = true;
	SetPlayerPos(id, 264.6432, 77.1918, 1001.0391);
	SetPlayerInterior(id, 6);
	SendClientMessage(id, -1, "Voce foi preso por um administrador.");
	SendClientMessage(playerid, -1, "Jogador preso.");
	return 1;
}

CMD:soltar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /soltar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	Jailed[id] = false;
	SetPlayerInterior(id, 0);
	SetPlayerPos(id, 1542.0, -1675.0, 13.5);
	SendClientMessage(id, -1, "Voce foi solto.");
	SendClientMessage(playerid, -1, "Jogador solto.");
	return 1;
}

CMD:curar(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /curar [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	SetPlayerHealth(id, 100.0);
	SendClientMessage(id, -1, "Voce foi curado por um administrador.");
	SendClientMessage(playerid, -1, "Jogador curado.");
	return 1;
}

CMD:colete(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id;
	if (sscanf(params, "u", id)) { SendClientMessage(playerid, -1, "Uso: /colete [id]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	SetPlayerArmour(id, 100.0);
	SendClientMessage(id, -1, "Voce recebeu colete de um administrador.");
	SendClientMessage(playerid, -1, "Colete dado.");
	return 1;
}

CMD:definirskin(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, skinid;
	if (sscanf(params, "ui", id, skinid)) { SendClientMessage(playerid, -1, "Uso: /definirskin [id] [skinid]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	SetPlayerSkin(id, skinid);
	SendClientMessage(playerid, -1, "Skin alterada.");
	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 3 (ENCARREGADO)
//////////////////////////////////////////////////

CMD:banir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, motivo[128];
	if (sscanf(params, "us[128]", id, motivo)) { SendClientMessage(playerid, -1, "Uso: /banir [id] [motivo]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	new string[144];
	format(string, sizeof(string), "Voce foi banido. Motivo: %s", motivo);
	SendClientMessage(id, -1, string);
	Ban(id);
	SendClientMessage(playerid, -1, "Jogador banido.");
	return 1;
}

CMD:desbanir(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new ip[16];
	if (sscanf(params, "s[16]", ip)) { SendClientMessage(playerid, -1, "Uso: /desbanir [ip]"); return 1; }

	new string[64];
	format(string, sizeof(string), "unbanip %s", ip);
	SendRconCommand(string);
	SendClientMessage(playerid, -1, "IP desbanido.");
	return 1;
}

CMD:clima(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new valor;
	if (sscanf(params, "i", valor)) { SendClientMessage(playerid, -1, "Uso: /clima [valor]"); return 1; }

	SetWeather(valor);
	SendClientMessage(playerid, -1, "Clima alterado.");
	return 1;
}

CMD:hora(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new valor;
	if (sscanf(params, "i", valor)) { SendClientMessage(playerid, -1, "Uso: /hora [valor de 0 a 23]"); return 1; }

	SetWorldTime(valor);
	SendClientMessage(playerid, -1, "Hora alterada.");
	return 1;
}

CMD:darinheiro(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_ENCARREGADO) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, valor;
	if (sscanf(params, "ui", id, valor)) { SendClientMessage(playerid, -1, "Uso: /darinheiro [id] [valor]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }

	GivePlayerMoney(id, valor);
	SendClientMessage(playerid, -1, "Dinheiro dado.");
	return 1;
}
//////////////////////////////////////////////////
// SISTEMA DE ADMINISTRACAO - NIVEL 4 (FUNDADOR)
//////////////////////////////////////////////////

CMD:definirnivel(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_FUNDADOR) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, nivel;
	if (sscanf(params, "ui", id, nivel)) { SendClientMessage(playerid, -1, "Uso: /definirnivel [id] [nivel 0-4]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }
	if (nivel < 0 || nivel > 4) { SendClientMessage(playerid, -1, "Nivel invalido. Use de 0 a 4."); return 1; }

	AdminLevel[id] = nivel;
	new string[144];
	format(string, sizeof(string), "Seu nivel de admin foi definido para %d.", nivel);
	SendClientMessage(id, -1, string);
	SendClientMessage(playerid, -1, "Nivel definido.");
	return 1;
}
CMD:acmds(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new string[1024];
	strcat(string, "{FFFFFF}NIVEL 1 - HELPER\n");
	strcat(string, "{AAAAAA}/achat, /servico, /irpara, /trazer, /congelar, /descongelar, /espectar, /pararespectar, /advertir\n\n");
	strcat(string, "{FFFFFF}NIVEL 2 - GAME ADMIN\n");
	strcat(string, "{AAAAAA}(tudo do Helper) + /expulsar, /mutar, /desmutar, /prender, /soltar, /curar, /colete, /definirskin\n\n");
	strcat(string, "{FFFFFF}NIVEL 3 - ENCARREGADO\n");
	strcat(string, "{AAAAAA}(tudo do Game Admin) + /banir, /desbanir, /clima, /hora, /darinheiro\n\n");
	strcat(string, "{FFFFFF}NIVEL 4 - FUNDADOR\n");
	strcat(string, "{AAAAAA}(tudo do Encarregado) + /definirnivel, /definirlevel");

	ShowPlayerDialog(playerid, DIALOG_ACMDS, DIALOG_STYLE_MSGBOX, "Painel Administrativo", string, "Fechar", "");
	return 1;
}
CMD:definirlevel(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_FUNDADOR) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new id, level;
	if (sscanf(params, "ui", id, level)) { SendClientMessage(playerid, -1, "Uso: /definirlevel [id] [level]"); return 1; }
	if (!IsPlayerConnected(id)) { SendClientMessage(playerid, -1, "Jogador nao conectado."); return 1; }
	if (level < 1) { SendClientMessage(playerid, -1, "O level minimo e 1."); return 1; }

	Nivel[id] = level;
	SetPlayerScore(id, level);
	TempoJogado[id] = 0;
	SalvarConta(id);

	new string[100];
	format(string, sizeof(string), "Seu level foi definido para %d.", level);
	SendClientMessage(id, -1, string);
	SendClientMessage(playerid, -1, "Level definido.");
	return 1;
}
CMD:stats(playerid, params[])
{
	new nome[MAX_PLAYER_NAME];
	GetPlayerName(playerid, nome, sizeof(nome));

	new horasNecessarias = (Nivel[playerid] < 7) ? (4 * 3600) : (8 * 3600);
	new restante = horasNecessarias - TempoJogado[playerid];
	if (restante < 0) restante = 0;

	new horasRestantes = restante / 3600;
	new minutosRestantes = (restante % 3600) / 60;

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
		"{FFFF00}Level: {FFFFFF}%d\n"\
		"{FFFF00}Dinheiro na mao: {FFFFFF}$%d\n"\
		"{FFFF00}Falta para o proximo level: {FFFFFF}%dh %dmin",
		nome, nomeProfissao, Nivel[playerid], GetPlayerMoney(playerid), horasRestantes, minutosRestantes
	);

	ShowPlayerDialog(playerid, DIALOG_STATS, DIALOG_STYLE_MSGBOX, "{FFFF00}Estatisticas", string, "Fechar", "");
	return 1;
}
CMD:veiculo(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_GAMEADMIN) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	new modelo;
	if (sscanf(params, "i", modelo)) { SendClientMessage(playerid, -1, "Uso: /veiculo [id do veiculo]"); return 1; }
	if (modelo < 400 || modelo > 611) { SendClientMessage(playerid, -1, "ID de veiculo invalido. Use de 400 a 611."); return 1; }

	new Float:x, Float:y, Float:z, Float:angulo;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angulo);

	new veiculo = CreateVehicle(modelo, x, y, z, angulo, -1, -1, -1, false);
	PutPlayerInVehicle(playerid, veiculo, 0);

	SendClientMessage(playerid, -1, "Veiculo spawnado.");
	return 1;
}
CMD:emprego(playerid, params[])
{
	if (!DentroAgencia[playerid]) { SendClientMessage(playerid, -1, "Voce precisa estar na Agencia de Emprego para usar este comando."); return 1; }

	ShowPlayerDialog(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
	return 1;
}
CMD:sair(playerid, params[])
{
	if (!DentroAgencia[playerid]) { SendClientMessage(playerid, -1, "Voce nao esta em nenhum lugar pra sair."); return 1; }

	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid, AgenciaPos[0], AgenciaPos[1], AgenciaPos[2] + 1.0);
	DentroAgencia[playerid] = false;
	SendClientMessage(playerid, -1, "Voce saiu da Agencia de Emprego.");
	return 1;
}
CMD:tpconfig(playerid, params[])
{
	if (AdminLevel[playerid] < LEVEL_HELPER) { SendClientMessage(playerid, -1, "Voce nao tem permissao para usar este comando."); return 1; }

	SetPlayerInterior(playerid, 0);
	SetPlayerPos(playerid, 1467.2704,-1011.1876,26.8438 + 1.0);
	SendClientMessage(playerid, -1, "Voce foi teleportado com sucesso!");
	return 1;
}
CMD:trabalhar(playerid, params[])
{
	if (Profissao[playerid] != PROFISSAO_PIZZAIOLO)
	{
		SendClientMessage(playerid, -1, "Voce precisa ser Pizzaiolo para usar este comando.");
		return 1;
	}

	if (!Trabalhando[playerid])
	{
		SkinAntesTrabalho[playerid] = GetPlayerSkin(playerid);
		SetPlayerSkin(playerid, 155);
		Trabalhando[playerid] = true;
		SendClientMessage(playerid, -1, "Voce entrou em modo de trabalho. Va ate a garagem e aperte F.");
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
		SendClientMessage(playerid, -1, "Voce saiu do modo de trabalho.");
	}
	return 1;
}

CMD:prepararpizza(playerid, params[])
{
	if (!Trabalhando[playerid]) { SendClientMessage(playerid, -1, "Voce precisa estar trabalhando. Use /trabalhar."); return 1; }
	if (!IsPlayerInRangeOfPoint(playerid, 2.0, PizzaPickupPos[0], PizzaPickupPos[1], PizzaPickupPos[2])) { SendClientMessage(playerid, -1, "Voce precisa estar perto do balcao."); return 1; }
	if (PizzasNaMoto[playerid] >= 5) { SendClientMessage(playerid, -1, "Sua Faggio ja esta cheia."); return 1; }
	if (CarregandoPizza[playerid]) { SendClientMessage(playerid, -1, "Voce ja esta carregando uma pizza."); return 1; }

	CarregandoPizza[playerid] = true;
	SendClientMessage(playerid, -1, "Voce pegou uma pizza. Va ate a Faggio e use /guardarpizza.");
	return 1;
}

CMD:guardarpizza(playerid, params[])
{
	if (!Trabalhando[playerid] || !CarregandoPizza[playerid]) { SendClientMessage(playerid, -1, "Voce nao esta carregando pizza."); return 1; }
	if (FaggioVeiculo[playerid] == 0) { SendClientMessage(playerid, -1, "Sua Faggio nao existe."); return 1; }

	new Float:vx, Float:vy, Float:vz;
	GetVehiclePos(FaggioVeiculo[playerid], vx, vy, vz);

	if (!IsPlayerInRangeOfPoint(playerid, 3.0, vx, vy, vz)) { SendClientMessage(playerid, -1, "Chegue mais perto da Faggio."); return 1; }

	CarregandoPizza[playerid] = false;
	PizzasNaMoto[playerid]++;

	new string[64];
	format(string, sizeof(string), "Pizza guardada (%d/5)", PizzasNaMoto[playerid]);
	SendClientMessage(playerid, -1, string);
	return 1;
}

CMD:retirarcaixa(playerid, params[])
{
	if (PizzasNaMoto[playerid] <= 0)
	{
		SendClientMessage(playerid, -1, "Nao ha pizzas guardadas na Faggio.");
		return 1;
	}

	PizzasNaMoto[playerid]--;
	SendClientMessage(playerid, -1, "Voce retirou uma caixa de pizza da Faggio.");
	return 1;
}

CMD:rotapizza(playerid, params[])
{
	if (!Trabalhando[playerid] || FaggioVeiculo[playerid] == 0 || !IsPlayerInVehicle(playerid, FaggioVeiculo[playerid]))
	{
		SendClientMessage(playerid, -1, "Voce precisa estar na sua Faggio trabalhando.");
		return 1;
	}
	if (PizzasNaMoto[playerid] <= 0) { SendClientMessage(playerid, -1, "Guarde pelo menos uma pizza."); return 1; }
	if (EmRota[playerid]) { SendClientMessage(playerid, -1, "Voce ja esta em rota."); return 1; }

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
	SendClientMessage(playerid, -1, "Rota iniciada!");
	return 1;
}

CMD:entregarpedido(playerid, params[])
{
	if (!EmRota[playerid])
	{
		SendClientMessage(playerid, -1, "Voce nao esta em rota de entrega.");
		return 1;
	}

	new casa = RotaOrdem[playerid][EntregaAtual[playerid]];

	if (!IsPlayerInRangeOfPoint(playerid, 4.0, EntregasPos[casa][0], EntregasPos[casa][1], EntregasPos[casa][2]))
	{
		SendClientMessage(playerid, -1, "Chegue mais perto da casa.");
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
		SendClientMessage(playerid, -1, "Rota finalizada!");
	}
	else
	{
		new prox = RotaOrdem[playerid][EntregaAtual[playerid]];
		SetPlayerCheckpoint(playerid, EntregasPos[prox][0], EntregasPos[prox][1], EntregasPos[prox][2], 4.0);
		SendClientMessage(playerid, -1, "Siga para a proxima casa.");
	}
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

	SendClientMessage(playerid, -1, "Uso: /ajuda emprego");
	return 1;
}
CMD:banco(playerid, params[])
{
	if (!DentroBanco[playerid]) { SendClientMessage(playerid, -1, "Voce precisa estar dentro do Banco para usar este comando."); return 1; }

	ShowPlayerDialog(playerid, DIALOG_BANCO, DIALOG_STYLE_LIST, "Banco", "Ver Saldo\nDepositar\nSacar", "Selecionar", "Fechar");
	return 1;
}
CMD:sairbanco(playerid, params[])
{
	if (!DentroBanco[playerid]) { SendClientMessage(playerid, -1, "Voce nao esta no Banco."); return 1; }

	SetPlayerPos(playerid, BancoEntradaPos[0], BancoEntradaPos[1], BancoEntradaPos[2]);
	DentroBanco[playerid] = false;
	SendClientMessage(playerid, -1, "Voce saiu do Banco.");
	return 1;
}
CMD:pagar(playerid, params[])
{
	new id, valor;

	if (sscanf(params, "ui", id, valor))
	{
		SendClientMessage(playerid, -1, "Uso: /pagar [id] [valor]");
		return 1;
	}

	if (!IsPlayerConnected(id))
	{
		SendClientMessage(playerid, -1, "Jogador nao conectado.");
		return 1;
	}

	if (id == playerid)
	{
		SendClientMessage(playerid, -1, "Voce nao pode pagar para si mesmo.");
		return 1;
	}

	if (valor <= 0)
	{
		SendClientMessage(playerid, -1, "Valor invalido.");
		return 1;
	}

	if (valor > GetPlayerMoney(playerid))
	{
		SendClientMessage(playerid, -1, "Voce nao tem esse valor em maos.");
		return 1;
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (!IsPlayerInRangeOfPoint(id, 3.0, x, y, z))
	{
		SendClientMessage(playerid, -1, "Esse jogador esta longe demais para receber o pagamento.");
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
CMD:carregarp(playerid, params[])
{
	if (!IsLoggedIn[playerid]) { SendClientMessage(playerid, -1, "Voce precisa estar logado."); return 1; }
	if (UltimaPosX[playerid] == 0.0 && UltimaPosY[playerid] == 0.0 && UltimaPosZ[playerid] == 0.0)
	{
		SendClientMessage(playerid, -1, "Nao ha nenhuma posicao salva ainda.");
		return 1;
	}

	SetPlayerInterior(playerid, UltimoInterior[playerid]);
	SetPlayerPos(playerid, UltimaPosX[playerid], UltimaPosY[playerid], UltimaPosZ[playerid]);
	SendClientMessage(playerid, -1, "Voce foi levado de volta ao local onde estava antes de sair.");
	return 1;
}