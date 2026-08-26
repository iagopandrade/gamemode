#include <open.mp>

#include "modules/constants"

#include "../dependencies/sscanf2.inc"
#include "../dependencies/streamer"
#include "../dependencies/Pawn.CMD"
#include "../dependencies/foreach"
#include "../dependencies/easyDialog"
#include "../dependencies/a_mysql"
#include "../dependencies/whirlpool"
#include "../dependencies/WeatherSystem"
#include "../dependencies/gametext_plus"
#define OVERRIDE_NATIVE_GAMETEXT
#include "../dependencies/trunk_system"

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
/* teste */
#include "modules/baloon"

public OnPlayerConnect(playerid)
{
    //Remove Buildings///////////////////////////////////////////////////////////////////////////////////////////////
    RemoveBuildingForPlayer(playerid, 1408, 2099.851, -1813.906, 13.100, 0.250);
    RemoveBuildingForPlayer(playerid, 1408, 2099.858, -1819.359, 13.100, 0.250);
    RemoveBuildingForPlayer(playerid, 1408, 2099.858, -1799.421, 13.100, 0.250);
    RemoveBuildingForPlayer(playerid, 1408, 2102.601, -1822.078, 13.116, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2104.014, -1812.421, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2101.991, -1814.709, 12.569, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2103.406, -1817.303, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2102.125, -1819.953, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2101.632, -1798.171, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2103.959, -1800.562, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1408, 2099.851, -1793.975, 13.100, 0.250);
    RemoveBuildingForPlayer(playerid, 712, 2100.812, -1764.375, 21.389, 0.250);
    RemoveBuildingForPlayer(playerid, 1408, 2102.664, -1791.328, 13.100, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2103.687, -1795.906, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 1432, 2102.062, -1793.140, 12.670, 0.250);
    RemoveBuildingForPlayer(playerid, 620, 2105.084, -1765.609, 10.803, 0.250);
    RemoveBuildingForPlayer(playerid, 5418, 2112.938, -1797.088, 19.334, 0.250);
    RemoveBuildingForPlayer(playerid, 5530, 2112.938, -1797.088, 19.334, 0.250);

    #if defined Main_OnPlayerConnect
        return Main_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect Main_OnPlayerConnect

#if defined Main_OnPlayerConnect
    forward Main_OnPlayerConnect(playerid);
#endif

new tmpobjid;

public OnGameModeInit()
{
    DisableInteriorEnterExits();
    
    print("Gamemode initialized successfully.");

    if (!ConnectToDatabase()) {
        print("[Gamemode] Falha ao inicializar o banco de dados.");
        return 0;
    }

    InitializeCompanySystem();
    LoadCompanies();

    //Objects////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    tmpobjid = CreateDynamicObject(5418, 2112.939941, -1797.089965, 19.335899, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    SetDynamicObjectMaterial(tmpobjid, 1, 19297, "matlights", "invisible", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 3, 12946, "ce_bankalley1", "sw_warewall", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 7, 13364, "cetown3cs_t", "sw_barnwood2", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 11, 15046, "svcunthoose", "ab_flakeywall", 0x00000000);
    SetDynamicObjectMaterial(tmpobjid, 13, 5710, "cemetery_law", "brickgrey", 0x00000000);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tmpobjid = CreateDynamicObject(18248, 2105.730712, -1808.995117, 20.370698, 0.000000, 0.000000, 118.100036, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1358, 2101.995849, -1790.642578, 13.721858, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1358, 2100.674560, -1797.582153, 13.771860, 0.000000, 0.000000, 78.999977, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1408, 2097.443847, -1794.895385, 12.448392, 90.000000, 360.000000, -102.100006, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1408, 2106.368896, -1787.080200, 12.628396, 90.000000, 360.000000, 179.199996, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1408, 2102.844482, -1818.973632, 12.628396, 90.000000, 360.000000, -130.599975, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1408, 2102.499755, -1816.886596, 12.628396, 90.000000, 360.000000, 54.900077, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(943, 2119.468505, -1788.179321, 13.266586, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3529, 2105.382812, -1790.887573, 15.683735, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3529, 2123.845214, -1790.837524, 15.593747, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3529, 2123.836914, -1822.529663, 15.483748, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3529, 2105.425048, -1822.439575, 15.483748, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3529, 2104.220458, -1805.338134, 24.003231, 2.599997, -86.299995, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3530, 2111.171142, -1794.561523, 16.530639, 0.000000, 90.000000, 90.900070, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3675, 2113.830322, -1777.639038, 12.758969, 0.000000, 90.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3675, 2114.861328, -1779.049438, 12.758969, 0.000000, 90.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1685, 2109.320556, -1783.852172, 13.142663, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1685, 2111.181884, -1783.852172, 13.142663, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1685, 2113.623291, -1783.852172, 13.142663, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1685, 2112.392089, -1783.852172, 14.732666, 0.000000, 0.000000, -20.999998, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1685, 2110.223876, -1783.745849, 14.732666, 0.000000, 0.000000, 2.200001, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2973, 2096.621093, -1800.612182, 12.300100, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1827.674560, 12.899510, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1828.135009, 12.899510, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1828.555419, 12.899510, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1828.985839, 12.899510, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1829.456298, 12.899510, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1829.236083, 13.349518, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1828.675537, 13.349518, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1827.974853, 13.349518, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2960, 2099.902587, -1828.585449, 13.789525, 0.000000, 0.000000, 20.600000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3577, 2095.404541, -1823.998901, 13.261874, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2991, 2120.437744, -1824.750854, 13.132478, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2991, 2114.583740, -1825.806518, 13.132478, 0.000000, 0.000000, 32.599998, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3576, 2123.132080, -1771.506591, 13.804533, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3576, 2123.132080, -1775.986694, 13.804533, 0.000000, 0.000000, -161.800003, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3796, 2105.352294, -1772.251708, 12.355398, 0.000000, 0.000000, 136.199981, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2103.256835, -1763.364868, 12.608768, 90.000000, 360.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2098.645263, -1765.985839, 12.608768, 90.000000, 360.000000, -103.400039, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2097.507812, -1773.229492, 12.608768, 90.000000, 360.000000, -103.400039, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2095.872802, -1778.072143, 12.608768, 90.000000, 360.000000, -82.000045, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2094.539550, -1784.176269, 12.608768, 90.000000, 360.000000, -131.600021, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(970, 2096.166992, -1787.538330, 12.608768, 90.000000, 360.000000, -103.400039, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2102.789306, -1782.459960, 12.873310, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2103.850341, -1782.459960, 12.873310, 0.000000, 0.000000, -91.700004, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2103.810302, -1783.809448, 12.873310, 0.000000, 0.000000, -109.600006, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2102.162353, -1783.222290, 12.873310, 0.000000, 0.000000, -32.900024, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2100.348144, -1782.050537, 12.873310, 0.000000, 0.000000, 54.699970, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2102.087890, -1781.617187, 12.873310, 0.000000, 0.000000, 171.499969, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2121, 2100.571777, -1784.119750, 12.873310, 0.000000, 0.000000, -146.200042, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2635, 2102.853515, -1776.691772, 12.804190, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2635, 2102.853515, -1776.691772, 13.644192, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(2635, 2102.653320, -1777.008911, 12.882304, 0.000000, -92.400001, 47.100009, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3850, 2104.143554, -1794.275024, 12.564888, 0.000000, 90.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(3850, 2103.352783, -1800.244750, 12.564894, 0.000000, 90.000000, -53.900001, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(849, 2107.415283, -1803.910278, 21.379255, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(849, 2111.715576, -1770.709716, 12.689268, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2121.847412, -1762.484008, 12.764927, 0.000000, 0.000000, 105.400001, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2119.187988, -1762.484008, 12.734929, 0.000000, 0.000000, 90.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2116.267089, -1762.484008, 12.734929, 0.000000, 0.000000, 90.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2111.321289, -1762.676757, 12.804931, 0.000000, 0.000000, 74.699996, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2088.263183, -1791.510742, 12.743605, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2088.263183, -1793.911743, 12.743605, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1228, 2088.263183, -1797.461914, 12.743605, 0.000000, 0.000000, -36.500003, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1422, 2120.194335, -1763.480224, 12.772575, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 
    tmpobjid = CreateDynamicObject(1422, 2114.443847, -1763.290527, 12.802576, 0.000000, 0.000000, 0.000000, -1, -1, -1, 500.00, 500.00); 

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
