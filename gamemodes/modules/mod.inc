// --- Hook de OnGameModeInit no Mod 1 ---
public OnGameModeInit()
{
    print("Mod 1: OnGameModeInit executado.");

    // Se existir outro módulo carregado DEPOIS deste, repassa a execução
    #if defined Mod1_OnGameModeInit
        return Mod1_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit Mod1_OnGameModeInit
#if defined Mod1_OnGameModeInit
    forward Mod1_OnGameModeInit();
#endif


// --- Hook de OnPlayerSpawn no Mod 1 ---
public OnPlayerSpawn(playerid)
{
    print("Mod 1: Player spawnou.");

    #if defined Mod1_OnPlayerSpawn
        return Mod1_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn Mod1_OnPlayerSpawn
#if defined Mod1_OnPlayerSpawn
    forward Mod1_OnPlayerSpawn(playerid);
#endif