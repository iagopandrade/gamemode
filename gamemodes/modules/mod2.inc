// --- Hook de OnGameModeInit no Mod 2 ---
public OnGameModeInit()
{
    print("Mod 2: OnGameModeInit executado.");

    #if defined Mod2_OnGameModeInit
        return Mod2_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit Mod2_OnGameModeInit
#if defined Mod2_OnGameModeInit
    forward Mod2_OnGameModeInit();
#endif


// --- Hook de OnPlayerSpawn no Mod 2 ---
public OnPlayerSpawn(playerid)
{
    print("Mod 2: Player spawnou.");

    #if defined Mod2_OnPlayerSpawn
        return Mod2_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn Mod2_OnPlayerSpawn
#if defined Mod2_OnPlayerSpawn
    forward Mod2_OnPlayerSpawn(playerid);
#endif