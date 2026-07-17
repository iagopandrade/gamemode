#include <open.mp>

#include "includes\sscanf2.inc"
#include "includes\streamer.inc"
#include "includes\Pawn.CMD.inc"
#include "includes\DOF2.1.inc"
#include "includes/YSI-Includes\YSI_Coding\y_hooks.inc"
#include "includes/YSI-Includes\YSI_Data\y_iterate.inc"
#include "includes\easyDialog.inc"

#include "modules/core/colors.inc"

#include "modules/account/account.inc"

#include "modules/admin/admin.inc"
#include "modules/admin/comandos.inc"

#include "modules/player/fome.inc"
#include "modules/player/payday.inc"
#include "modules/player/comandos.inc"
#include "modules/player/death/death.inc"
#include "modules/player/death/commands.inc"

#include "modules/chat/chat.inc"
#include "modules/chat/comandos.inc"

#include "modules/animation/comandos.inc"

#include "modules/veiculo/comandos.inc"

#include "modules/company/company.inc"
#include "modules/company/binco.inc"
#include "modules/company/gym.inc"
#include "modules/company/sexy_shop.inc"
#include "modules/company/pizzeria.inc"
#include "modules/company/bank.inc"

#include "modules/empregos/comandos.inc"
#include "modules/empregos/pizza/pizza.inc"
#include "modules/empregos/pizza/comandos.inc"

#include "modules/economia/comandos.inc"

#include "modules/audio/musica_login.inc"

forward AtualizarSistema();

main() 
{
	printf("%d", 20 + 20 + 20 + 7);
}

public OnGameModeInit()
{
	SetGameModeText("Text Based Roleplay");
	AddPlayerClass(0, 1685.8695, -2241.0386, -2.6973, 179.8575, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
	DisableInteriorEnterExits();
	LogoServidor = TextDrawCreate(320.0, 20.0, "Brasil City");
    TextDrawFont(LogoServidor, TEXT_DRAW_FONT_3);
    TextDrawLetterSize(LogoServidor, 0.7, 3.0);
    TextDrawColour(LogoServidor, 0x006400FF);
    TextDrawSetOutline(LogoServidor, 1);
    TextDrawSetShadow(LogoServidor, 0);
    TextDrawAlignment(LogoServidor, TEXT_DRAW_ALIGN_CENTER);
    TextDrawUseBox(LogoServidor, false);

    // TextDraw Fome e Sede - Canto inferior direito (pequeno)
	TDFome = TextDrawCreate(550.0, 420.0, "Fome: 100");
	TextDrawFont(TDFome, TEXT_DRAW_FONT_1);
	TextDrawLetterSize(TDFome, 0.22, 1.0);
	TextDrawColour(TDFome, 0xFFA500FF);
	TextDrawSetOutline(TDFome, 1);
	TextDrawSetShadow(TDFome, 0);
	TextDrawAlignment(TDFome, TEXT_DRAW_ALIGN_RIGHT);

	TDSede = TextDrawCreate(550.0, 435.0, "Sede: 100");
	TextDrawFont(TDSede, TEXT_DRAW_FONT_1);
	TextDrawLetterSize(TDSede, 0.22, 1.0);
	TextDrawColour(TDSede, 0x00BFFFFF);
	TextDrawSetOutline(TDSede, 1);
	TextDrawSetShadow(TDSede, 0);
	TextDrawAlignment(TDSede, TEXT_DRAW_ALIGN_RIGHT);

	SetTimer("AtualizarFomeSede", 85000, true); // 85 segundos

	SetTimer("AtualizarSistema", 60000, true);

	CreatePickup(1210, 0, 1412.3226, -1700.2135, 13.5395);
	CreateDynamic3DTextLabel("Agencia de Emprego\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 1412.3226, -1700.2135, 14.0395, 20.0);///agencia de emprego

	CreatePickup(1210, 1, -2033.1250, -117.3180, 1035.1719);
    CreateDynamic3DTextLabel("Catalogo de Empregos\n{FFFFFF}Aperte F", 0xFFFF00FF, -2033.1250, -117.3180, 1035.6719, 20.0);///agencia de emprego

	// CreatePickup(1318, 0, 2104.9644,-1806.5123,13.5547); // troque 1210 pelo icone que quiser (ver lista de pickups)
    // CreateDynamic3DTextLabel("Pizzaria\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 2104.9644,-1806.5123,13.5547 + 0.5, 20.0); 

	CreatePickup(1212, 1, 375.7113,-119.2369,1001.4995);
	CreateDynamic3DTextLabel("Balcao de Atendimento\n{FFFFFF}Aperte F para acessar o menu de pedido", 0xFFFF00FF, 375.7113,-119.2369,1001.4995 + 0.5, 20.0);

	CreatePickup(1582, 1, 378.3459, -114.3801, 1001.4922);
    CreateDynamic3DTextLabel("Use /prepararpizza para pegar uma pizza", 0xFFFF00FF, 378.3459, -114.3801, 1001.9922, 20.0);
	
	CreatePickup(1318, 1, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]);
	CreateDynamic3DTextLabel("Garagem do Pizzaiolo\n{FFFFFF}Aperte F para spawnar sua Faggio", 0xFFFF00FF, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]+0.5, 15.0);

	// CreatePickup(1210, 0, 1467.2704, -1011.1876, 26.8438);
    // CreateDynamic3DTextLabel("Banco\n{FFFFFF}Aperte F para entrar", 0xFFFF00FF, 1467.2704, -1011.1876, 27.3438, 20.0);

	CreatePickup(1274, 1, 2315.8311, -10.0434, 26.7422);
	CreateDynamic3DTextLabel("Balcao do Banco\n{FFFFFF}Use /banco aqui", 0xFFFF00FF, 2315.8311, -10.0434, 27.2422, 20.0);
	return 1;
}

public OnGameModeExit()
{
    DOF2_Exit();
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if (IsPlayerNPC(playerid)) {
		return 1;
	}
   
    TogglePlayerSpectating(playerid, true);

	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    return 0;
}

public OnPlayerConnect(playerid)
{
	IsLoggedIn[playerid] = false;
	TogglePlayerControllable(playerid, false);

	// Resetar fome e sede
	Fome[playerid] = 100;
	Sede[playerid] = 100;

	if (fexist(UserPath(playerid)))
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - {00FF00}BRASIL CITY", "Esta conta ja existe.\nDigite sua senha para entrar:", "Entrar", "Sair");
	else
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Login - {00FF00}BRASIL CITY", "Esta conta nao existe.\nDigite uma senha para cria-la:", "Registrar", "Sair");

	TocarMusicaLogin(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if (IsLoggedIn[playerid])
	{
		PararMusicaLogin(playerid);   // ← Adicione esta linha
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

	ApplyAnimation(playerid, "PED", "IDLE", 4.1, false, false, false, false, false, SYNC_ALL);
	ClearAnimations(playerid, SYNC_ALL);
	ApplyAnimation(playerid, "ped", "XPRESSscratch", 4.1, false, false, false, true, SYNC_ALL);
	ClearAnimations(playerid, SYNC_ALL);
	return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	SpawnPlayer(playerid);
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
		SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Use /entregarpedido para entregar a pizza!");
	}
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
			ShowPlayerDialog(playerid, DIALOG_EMPREGOS, DIALOG_STYLE_LIST, "Agencia de Emprego", "Pizzaiolo\nDesempregado (sair do emprego atual)", "Selecionar", "Cancelar");
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

		// Porta de FORA da Pizzaria -> entra
		if (!DentroPizzariaLS[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2]) && GetPlayerInterior(playerid) == 0)
		{
			SetPlayerInterior(playerid, 5);
			SetPlayerPos(playerid, PizzariaLSSaidaPos[0], PizzariaLSSaidaPos[1], PizzariaLSSaidaPos[2]);
			DentroPizzariaLS[playerid] = true;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Bem vindo, senhor(a). Se aproxime ao balcao para ser atendida.");
			return 1;
		}

		// Porta de SAIDA da Pizzaria -> volta pro mundo normal
		if (DentroPizzariaLS[playerid] && IsPlayerInRangeOfPoint(playerid, 2.0, PizzariaLSSaidaPos[0], PizzariaLSSaidaPos[1], PizzariaLSSaidaPos[2]))
		{
			SetPlayerInterior(playerid, 0);
			SetPlayerPos(playerid, PizzariaLSPos[0], PizzariaLSPos[1], PizzariaLSPos[2] + 1.0);
			DentroPizzariaLS[playerid] = false;
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Volte sempre, senhor(a)!");
			return 1;
		}

		// GARAGEM PIZZAIOLO - SPAWN FAGGIO
		if (Trabalhando[playerid] && IsPlayerInRangeOfPoint(playerid, 3.0, GaragemPizzaPos[0], GaragemPizzaPos[1], GaragemPizzaPos[2]))
		{
			if (FaggioVeiculo[playerid] != 0)
				return SendClientMessage(playerid, COR_BCRP_ERRO, "[BC:RP] Voce ja tem uma Faggio spawnada.");

			new Float:x = GaragemPizzaPos[0] + 1.5;
			new Float:y = GaragemPizzaPos[1] + 2.0;
			new Float:z = GaragemPizzaPos[2];

			FaggioVeiculo[playerid] = CreateVehicle(448, x, y, z, 90.0, -1, -1, -1, false);
			PutPlayerInVehicle(playerid, FaggioVeiculo[playerid], 0);

			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Faggio spawnada! Use /rotapizza quando estiver pronto.");
			return 1;
		}
	}
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == DIALOG_LOGIN) {
		if (!response) {
			Kick(playerid); 
			return 1;
		}

		if (isnull(inputtext)) {
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, 
				"Login - {00FF00}BRASIL CITY", 
				"{FF0000}Senha invalida.\nDigite sua senha para entrar:", 
				"Entrar", "Sair"
			);
			return 1; 
		}

		new File:conta = fopen(UserPath(playerid), io_read);
		new linhaSenha[64], linhaAdmin[64], linhaSkin[64], linhaDinheiro[64], linhaNivel[64], linhaTempo[64], linhaPayday[64], linhaProfissao[64], linhaBanco[64], linhaX[64], linhaY[64], linhaZ[64], linhaInterior[64], linhaFome[64], linhaSede[64];
		
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
		fread(conta, linhaFome);
		fread(conta, linhaSede);
		fclose(conta);

		if (strval(linhaSenha) == HashSenha(inputtext)) {
			IsLoggedIn[playerid] = true;
			TocarMusicaLogin(playerid);
			SenhaHash[playerid] = strval(linhaSenha);
			AdminLevel[playerid] = strval(linhaAdmin);

			SkinSalva[playerid] = strval(linhaSkin);
			ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, strval(linhaDinheiro));

			Nivel[playerid] = strval(linhaNivel);
			if (Nivel[playerid] < 1) Nivel[playerid] = 1;
			SetPlayerScore(playerid, Nivel[playerid]);
			TempoJogado[playerid] = strval(linhaTempo);
            if (TempoJogado[playerid] > 1000) TempoJogado[playerid] = 0;
			MinutosPayday[playerid] = strval(linhaPayday);
			Profissao[playerid] = strval(linhaProfissao);
			SaldoBanco[playerid] = strval(linhaBanco);
            PararMusicaLogin(playerid);
			UltimaPosX[playerid] = floatstr(linhaX);
			UltimaPosY[playerid] = floatstr(linhaY);
			UltimaPosZ[playerid] = floatstr(linhaZ);
			UltimoInterior[playerid] = strval(linhaInterior);

			// Carregamento de Fome e Sede
			Fome[playerid] = strval(linhaFome);
			Sede[playerid] = strval(linhaSede);
			if (Fome[playerid] <= 0) Fome[playerid] = 100;
			if (Sede[playerid] <= 0) Sede[playerid] = 100;

			TogglePlayerControllable(playerid, true);
			TextDrawShowForPlayer(playerid, LogoServidor);
			TextDrawShowForPlayer(playerid, LogoLinha);
			TextDrawShowForPlayer(playerid, LogoEstrelaEsquerda);
			TextDrawShowForPlayer(playerid, LogoEstrelaDireita);
			TextDrawShowForPlayer(playerid, TDFome);
			TextDrawShowForPlayer(playerid, TDSede);
			
			SetSpawnInfo(playerid, NO_TEAM, SkinSalva[playerid], 1683.0283, -2242.7976, -2.6917, 176.9883, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FLOWER, 1);
			TogglePlayerSpectating(playerid, false);
			SpawnPlayer(playerid);

			SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
			SendClientMessage(playerid, 0x2ECC71FF, "{2ECC71}B{27AE60}r{2ECC71}a{27AE60}s{2ECC71}i{27AE60}l {F1C40F}C{F39C12}i{F1C40F}t{F39C12}y {FFFFFF}Roleplay");
			SendClientMessage(playerid, 0xFFFFFFFF, "{00FF7F}| {FFFFFF}Login efetuado com sucesso! Seja bem-vindo ao {F1C40F}Brasil City{FFFFFF}.");
			SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
			SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
			SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
			SendClientMessage(playerid, 0xFFFFFFFF, "{1ABC9C}| {FFFFFF}Para parar a musica utilize o comando {F1C40F}/pararmusica{FFFFFF}.");
			SendClientMessage(playerid, 0xFFFFFFFF, "{3498DB}-> {FFFFFF}Use {F1C40F}/carregarp {FFFFFF}para retornar ao local onde estava antes de desconectar.");
			SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
		} else {
			ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - BRASIL CITY", "{FF0000}Senha incorreta.\nDigite sua senha para entrar:", "Entrar", "Sair");
		}
		return 1;
	}

	if (dialogid == DIALOG_BANCO)
	{
		if (!response) return 1;

		if (listitem == 0)
		{
			new string[144];
			format(string, sizeof(string), "Saldo em conta: $%d\nDinheiro na mao: $%d", SaldoBanco[playerid], GetPlayerMoney(playerid));
			ShowPlayerDialog(playerid, DIALOG_BANCO_SALDO, DIALOG_STYLE_MSGBOX, "Banco - Saldo", string, "Voltar", "Fechar");
		}
		else if (listitem == 1)
		{
			ShowPlayerDialog(playerid, DIALOG_BANCO_DEPOSITAR, DIALOG_STYLE_INPUT, "Depositar", "Digite o valor que deseja depositar:", "Depositar", "Cancelar");
		}
		else if (listitem == 2)
		{
			ShowPlayerDialog(playerid, DIALOG_BANCO_SACAR, DIALOG_STYLE_INPUT, "Sacar", "Digite o valor que deseja sacar:", "Sacar", "Cancelar");
		}
		return 1;
	}

	if (dialogid == DIALOG_BANCO_DEPOSITAR)
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

	if (dialogid == DIALOG_BANCO_SACAR)
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

	if (dialogid == DIALOG_REGISTER) {
		if (!response) {
			Kick(playerid);
			return 1;
		}
		if (isnull(inputtext)) {
			ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, 
				"Registro",
				"Digite uma senha valida para criar sua conta:",
				"Registrar", "Sair"
			); 
	        return 1;
		}

		SenhaHash[playerid] = HashSenha(inputtext);
		AdminLevel[playerid] = 0;
		SkinAtual[playerid] = 1;
		SetPlayerSkin(playerid, 1);
		MostrarSkin(playerid);

		SetSpawnInfo(playerid, NO_TEAM, SkinSalva[playerid], 1683.0283, -2242.7976, -2.6917, 176.9883, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FLOWER, 1);
		TogglePlayerSpectating(playerid, false);
		SpawnPlayer(playerid);

		SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
		SendClientMessage(playerid, 0x2ECC71FF, "{2ECC71}B{27AE60}r{2ECC71}a{27AE60}s{2ECC71}i{27AE60}l {F1C40F}C{F39C12}i{F1C40F}t{F39C12}y {FFFFFF}Roleplay");
		SendClientMessage(playerid, 0xFFFFFFFF, "{00FF7F}| {FFFFFF}Conta registrada com sucesso! Seja bem-vindo ao {F1C40F}Brasil City{FFFFFF}.");
		SendClientMessage(playerid, 0xFFFFFFFF, "{00BFFF}i {FFFFFF}O servidor encontra-se em fase {F39C12}Pre-Alpha{FFFFFF} e recebe atualizacoes diariamente.");
		SendClientMessage(playerid, 0xFFFFFFFF, "{FFD700}* {FFFFFF}Nosso objetivo eh reviver o {2ECC71}Roleplay Text-Based{FFFFFF} que marcou a comunidade em {2ECC71}2019{FFFFFF}.");
		SendClientMessage(playerid, 0xFFFFFFFF, "{9B59B6}| {FFFFFF}Sua participacao faz diferenca! Envie sugestoes e reporte bugs em nosso {5865F2}Discord{FFFFFF}.");
		SendClientMessage(playerid, 0xFFFFFFFF, "{1ABC9C}| {FFFFFF}Para parar a musica utilize o comando {F1C40F}/pararmusica{FFFFFF}.");
		SendClientMessage(playerid, 0xFFFFFFFF, "{3498DB}-> {FFFFFF}Use {F1C40F}/carregarp {FFFFFF}para retornar ao local onde estava antes de desconectar.");
		SendClientMessage(playerid, 0xF7D358FF, "__________________________________________________________");
		return 1;
	}

	if (dialogid == DIALOG_SKIN)
	{
		if (!response)
		{
			MostrarSkin(playerid);
			return 1;
		}

		if (listitem == 0)
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
		else if (listitem == 1)
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
		else if (listitem == 2)
		{
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
			TextDrawShowForPlayer(playerid, LogoLinha);
			TextDrawShowForPlayer(playerid, LogoEstrelaEsquerda);
			TextDrawShowForPlayer(playerid, LogoEstrelaDireita);
			SendClientMessage(playerid, COR_BCRP_INFO, "[BC:RP] Conta criada e login efetuado com sucesso!");
		}
		return 1;
	}

	if (dialogid == DIALOG_EMPREGOS)
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
	if (dialogid == DIALOG_BANCO_SALDO)
    {
	    if (response)
		{
			ShowPlayerDialog(playerid, DIALOG_BANCO, DIALOG_STYLE_LIST, "Banco", "Ver Saldo\nDepositar\nSacar", "Selecionar", "Fechar");
		}

	    return 1;
    }
	return 1;
}

CMD:pararmusica(playerid, params[])
{
	StopAudioStreamForPlayer(playerid);
	SendClientMessage(playerid, -1, "Musica parada.");
	return 1;
}

/**
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢛⢄⠍⣴⣿⣿⣿⣿⣿⣿⣿⡿⢿⠍⠙⠻⠻⢿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠑⢀⡂⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣤⣤⣄⣀⣐⠘⠂⠘⠀⢈⠉⠭⠩⢽
⣿⣿⣿⣿⣿⣿⣿⠟⠑⣡⢔⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⢢⢖⣼⣿
⣿⣿⣿⣿⣿⣿⢑⢄⡼⣱⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠑⠀⢡⣾⣿
⣿⣿⣿⣿⡿⠃⢔⣵⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠐⡐⣼⣿⣿⣿
⣿⣿⣿⡿⠂⣪⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢏⠠⣢⣾⣿⣿⣿⣿
⣿⣿⣟⠠⣰⣿⠿⠟⠛⡛⠛⠻⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠡⡐⣷⣿⣿⣿⣿⣿⣿
⣿⡟⠂⠘⣡⣴⣾⣾⣿⣿⣿⣷⣶⣦⡐⢄⠭⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⠂⢄⣿⣿⣿⣿⣿⣿⣿⣿
⣿⠁⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⢱⠈⡾⣹⣿⣿⣿⣿⣿⣛⣛⢛⢛⠫⠃⠴⡾⠿⠿⣿⣿⣿⣿⣿⣿
⡿⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⢴⢷⣿⣿⣿⣿⣿⣷⣶⣶⢴⠆⢲⣶⣤⣬⣭⣽⣿⣿⣿⣿⣿
⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠠⠄⢫⣿⣿⣿⣿⣿⣿⣿⠯⠍⢼⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣧⠊⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢣⠐⡔⢸⣿⣿⣿⣿⣿⣿⡟⢜⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⡔⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣡⢎⢄⣶⣿⣿⣿⣿⣿⣿⣿⡇⡬⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣔⢄⠛⢿⣿⣿⣿⣿⣿⡿⢛⡵⡊⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⢰⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣷⣤⣒⠢⠬⣍⣚⣒⠬⣐⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⢡⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
 */