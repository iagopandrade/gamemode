#include <open.mp>

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
