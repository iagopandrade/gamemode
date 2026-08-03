#include <open.mp>

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

#include "modules/utils/colors.inc"
#include "modules/database/database.inc"
#include "modules/character/character.inc"
#include "modules/account/account.inc"
#include "modules/player/player.inc"
#include "modules/chat/chat.inc"
#include "modules/animations/animations.inc"
#include "modules/vehicle/vehicle.inc"
#include "modules/time/time.inc"
#include "modules/admin/admin.inc"
#include "modules/house/house.inc"
#include "modules/company/company.inc"

public OnGameModeInit()
{
    print("Gamemode initialized successfully.");
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
