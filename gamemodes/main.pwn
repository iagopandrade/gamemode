#include <open.mp>

#include "includes\sscanf2.inc"
#include "includes\streamer.inc"
#include "includes\Pawn.CMD.inc"
#include "includes/YSI-Includes\YSI_Coding\y_hooks.inc"
#include "includes/YSI-Includes\YSI_Data\y_iterate.inc"
#include "includes\easyDialog.inc"
#include "includes\a_mysql.inc"
#include "includes\whirlpool.inc"

#include "modules/core/colors.inc"

#include "modules/player/player.inc"

#include "modules/character/character.inc"

#include "modules/account/account.inc"
#include "modules/account/dialogs.inc"

/*
#include "modules/admin/admin.inc"
#include "modules/admin/comandos.inc"
*/

/*
#include "modules/player/fome.inc"
#include "modules/player/payday.inc"
*/
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

/*
#include "modules/empregos/comandos.inc"
#include "modules/empregos/pizza/pizza.inc"
#include "modules/empregos/pizza/comandos.inc"

#include "modules/economia/comandos.inc"

#include "modules/audio/musica_login.inc"
*/

forward AtualizarSistema();

main() 
{
	print("----------------------------------------");
	print("      Gamemode Iniciada!                ");
    print(" 	BPC Player's Community open.mp 2026");
	print("----------------------------------------");
}

/*
public OnGameModeInit()
{
	SetGameModeText("Text Based Roleplay");
	DisableInteriorEnterExits();
	LogoServidor = TextDrawCreate(320.0, 20.0, "Brasil City");
    TextDrawFont(LogoServidor, TEXT_DRAW_FONT_3);
    TextDrawLetterSize(LogoServidor, 0.7, 3.0);
    TextDrawColour(LogoServidor, 0x006400FF);
    TextDrawSetOutline(LogoServidor, 1);
    TextDrawSetShadow(LogoServidor, 0);
    TextDrawAlignment(LogoServidor, TEXT_DRAW_ALIGN_CENTER);
    TextDrawUseBox(LogoServidor, false);

    TextDraw Fome e Sede - Canto inferior direito (pequeno)
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

	CreatePickup(1274, 1, 2315.8311, -10.0434, 26.7422);
	CreateDynamic3DTextLabel("Balcao do Banco\n{FFFFFF}Use /banco aqui", 0xFFFF00FF, 2315.8311, -10.0434, 27.2422, 20.0);
	return 1;
}
*/

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
