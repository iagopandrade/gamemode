#include <open.mp>

#include "modules/constants"

#include <sscanf2> 
#include <streamer>
#include <Pawn.CMD>
#include <foreach>
#include <easyDialog>
#include <a_mysql>
#include <whirlpool>
#include <WeatherSystem>
#include <gametext_plus>
#define OVERRIDE_NATIVE_GAMETEXT

#include "modules/utils/colors"
#include "modules/database/database"
#include "modules/player/player"
#include "modules/character/character"
#include "modules/account/account"
#include "modules/chat/chat"
#include "modules/animations/animations"
#include "modules/vehicle/vehicle"
#include "modules/time/time"
#include "modules/admin/admin"
#include "modules/house/house"
#include "modules/company/company"

public OnGameModeInit()
{
    print("Gamemode initialized successfully.");

    if (!ConnectToDatabase()) {
        print("[Gamemode] Falha ao inicializar o banco de dados.");
        return 0;
    }

    InitializeCompanySystem();
    LoadCompanies();
    return 1;
}

public OnGameModeExit()
{
    print("Gamemode unloaded successfully.");
    return 1;
}

main()
{
    print("Gamemode loaded successfully.");
}
