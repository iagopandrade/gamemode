#if defined _INC_COMPANY
    #endinput
#endif
#define _INC_COMPANY

#define MAX_DYNAMIC_AREAS       (4096)
#define MAX_COMPANY_NAME        (64)
#define MAX_COMPANIES           (200)
#define MAX_COMPANY_MAPICONS    (8)
#define MAX_COMPANY_POINTS      (16)

enum {
    BINCO_DOWNTOWN,
    GYM_GANTON,
    SEXY_SHOP
};

enum e_COMPANY_POINT_TYPE
{
    COMPANY_POINT_ENTRANCE,
    COMPANY_POINT_EXIT,
    COMPANY_POINT_BUY,
    COMPANY_POINT_SELL,
    COMPANY_POINT_CLOTHES,
    COMPANY_POINT_GYM,
    COMPANY_POINT_CUSTOM
};

enum e_COMPANY_POINT_DATA
{
    STREAMER_TAG_AREA:POINT_AREA_ID,
    STREAMER_TAG_PICKUP:POINT_PICKUP_ID,

    POINT_TYPE,

    Float:POINT_X,
    Float:POINT_Y,
    Float:POINT_Z,
    Float:POINT_RADIUS,
    Float:POINT_ANGLE,

    POINT_WORLD,
    POINT_INTERIOR,

    POINT_EXTRA_DATA
};

enum e_WORLD_AREA_DATA {
    WORLD_AREA_COMPANY,
    WORLD_AREA_POINT
}

static s_CompanyPoints[MAX_COMPANIES][MAX_COMPANY_POINTS][e_COMPANY_POINT_DATA];
static s_WorldAreaData[MAX_DYNAMIC_AREAS][e_WORLD_AREA_DATA];

enum e_COMPANY_MAPICON_DATA
{
    E_COMPANY_MAPICON_ICON,

    Float:E_COMPANY_MAPICON_X,
    Float:E_COMPANY_MAPICON_Y,
    Float:E_COMPANY_MAPICON_Z,

    E_COMPANY_MAPICON_COLOR,
    E_COMPANY_MAPICON_STYLE
};

static s_CompanyMapIcons[MAX_COMPANIES][MAX_COMPANY_MAPICONS][e_COMPANY_MAPICON_DATA];

enum e_COMPANY_TYPE
{
    COMPANY_TYPE_CLOTHES,
    COMPANY_TYPE_AMMUNATION,
    COMPANY_TYPE_GYM,
    COMPANY_TYPE_SEXY_SHOP
};

enum e_COMPANY_AREA_TYPE
{
    COMPANY_AREA_TYPE_ENTRANCE,
    COMPANY_AREA_TYPE_EXIT
};

enum e_COMPANY_DATA
{
    E_COMPANY_DATA_ID,
    E_COMPANY_DATA_NAME[MAX_COMPANY_NAME],
    E_COMPANY_DATA_TYPE,

    E_COMPANY_MAPICON_COUNT,

    E_COMPANY_POINT_COUNT,

    bool:E_COMPANY_CREATED,
    bool:E_COMPANY_ENABLED
};

static s_CompanyData[MAX_COMPANIES][e_COMPANY_DATA];

stock Company_FindPoint(companyid, type)
{
    if (companyid < 0 || companyid >= MAX_COMPANIES)
    {
        return -1;
    }

    for (new pointid; pointid < s_CompanyData[companyid][E_COMPANY_POINT_COUNT]; pointid++)
    {
        if (s_CompanyPoints[companyid][pointid][POINT_TYPE] == type)
        {
            return pointid;
        }
    }

    return -1;
}

stock Company_TeleportToPoint(playerid, companyid, pointid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0)
{
    if (pointid == -1)
    {
        return 0;
    }

    SetPlayerVirtualWorld(
        playerid,
        s_CompanyPoints[companyid][pointid][POINT_WORLD]
    );

    SetPlayerInterior(
        playerid,
        s_CompanyPoints[companyid][pointid][POINT_INTERIOR]
    );

    SetPlayerPos(
        playerid,
        s_CompanyPoints[companyid][pointid][POINT_X] + x,
        s_CompanyPoints[companyid][pointid][POINT_Y] + y,
        s_CompanyPoints[companyid][pointid][POINT_Z] + z
    );

    SetPlayerFacingAngle(
        playerid,
        s_CompanyPoints[companyid][pointid][POINT_ANGLE]
    );

    SetCameraBehindPlayer(playerid);

    new string[MAX_COMPANY_NAME];
    new nameCompany[MAX_COMPANY_NAME];

    Company_GetName(companyid, nameCompany);
    format(string, sizeof(string), "~w~%s", nameCompany);

    GameTextForPlayer(playerid, string, 0, 1);
    return 1;
}

stock Company_Enter(playerid, companyid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0)
{
    new pointid = Company_FindPoint(
        companyid,
        COMPANY_POINT_EXIT
    );

    return Company_TeleportToPoint(
        playerid,
        companyid,
        pointid,
        x,
        y,
        z
    );
}

stock Company_Exit(playerid, companyid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0)
{
    new pointid = Company_FindPoint(
        companyid,
        COMPANY_POINT_ENTRANCE
    );

    return Company_TeleportToPoint(
        playerid,
        companyid,
        pointid,
        x,
        y,
        z
    );
}

stock Company_GetName(companyid, dest[], size = MAX_COMPANY_NAME)
{
    strcopy(
        dest,
        s_CompanyData[companyid][E_COMPANY_DATA_NAME],
        size
    );

    return 1;
}

stock Company_GetPointType(companyid, pointid)
{
    if (companyid < 0 || companyid >= MAX_COMPANIES)
    {
        return -1;
    }

    if (pointid < 0 || pointid >= s_CompanyData[companyid][E_COMPANY_POINT_COUNT])
    {
        return -1;
    }

    return s_CompanyPoints[companyid][pointid][POINT_TYPE];
}

stock Company_AddMapIcon(
    companyid,
    icon,

    Float:x,
    Float:y,
    Float:z,
    color = 0,
    style = MAPICON_GLOBAL)
{
    if (companyid < 0 || companyid >= MAX_COMPANIES)
    {
        return 0;
    }

    if (!s_CompanyData[companyid][E_COMPANY_CREATED])
    {
        return 0;
    }

    new mapiconid =
        s_CompanyData[companyid][E_COMPANY_MAPICON_COUNT]++;

    if (mapiconid >= MAX_COMPANY_MAPICONS)
    {
        s_CompanyData[companyid][E_COMPANY_MAPICON_COUNT]--;
        return 0;
    }

    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_ICON] = icon;

    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_X] = x;
    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_Y] = y;
    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_Z] = z;

    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_COLOR] = color;
    s_CompanyMapIcons[companyid][mapiconid][E_COMPANY_MAPICON_STYLE] = style;

    return mapiconid;
}

stock Company_CreatePlayerMapIcons(playerid)
{
    new iconid = 0;

    for (new companyid; companyid < MAX_COMPANIES; companyid++)
    {
        if (!s_CompanyData[companyid][E_COMPANY_CREATED])
        {
            continue;
        }

        if (!s_CompanyData[companyid][E_COMPANY_ENABLED])
        {
            continue;
        }

        for (
            new i;
            i < s_CompanyData[companyid][E_COMPANY_MAPICON_COUNT];
            i++)
        {
            if (iconid >= 100)
            {
                return 1;
            }

            SetPlayerMapIcon(
                playerid,
                iconid++,
                s_CompanyMapIcons[companyid][i][E_COMPANY_MAPICON_X],
                s_CompanyMapIcons[companyid][i][E_COMPANY_MAPICON_Y],
                s_CompanyMapIcons[companyid][i][E_COMPANY_MAPICON_Z],
                s_CompanyMapIcons[companyid][i][E_COMPANY_MAPICON_ICON],
                0,
                MAPICON_GLOBAL
            );
        }
    }

    return 1;
}

Company_Create(
    companyid,
    const name[],
    type)
{
    if (companyid < 0 || companyid >= MAX_COMPANIES) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_DATA_ID] = companyid;

    strcopy(
        s_CompanyData[companyid][E_COMPANY_DATA_NAME],
        name
    );

    s_CompanyData[companyid][E_COMPANY_DATA_TYPE] = type;

    s_CompanyData[companyid][E_COMPANY_POINT_COUNT] = 0;
    s_CompanyData[companyid][E_COMPANY_MAPICON_COUNT] = 0;

    s_CompanyData[companyid][E_COMPANY_CREATED] = true;
    s_CompanyData[companyid][E_COMPANY_ENABLED] = true;
    return 1;
}

stock World_RegisterArea(
    STREAMER_TAG_AREA:areaid,
    companyid,
    pointid)
{
    new id = _:areaid;

    s_WorldAreaData[id][WORLD_AREA_COMPANY] = companyid;
    s_WorldAreaData[id][WORLD_AREA_POINT]   = pointid;

    return 1;
}

stock Company_AddPoint(
    companyid,
    type,

    Float:x,
    Float:y,
    Float:z,
    Float:angle = 0.0,

    Float:radius,

    pickupModel = -1,
    pickupType = 1,

    interior,
    world = 0,
    extra_data = 0)
{
    new pointid =
        s_CompanyData[companyid][E_COMPANY_POINT_COUNT]++;

    s_CompanyPoints[companyid][pointid][POINT_TYPE] = type;

    s_CompanyPoints[companyid][pointid][POINT_X] = x;
    s_CompanyPoints[companyid][pointid][POINT_Y] = y;
    s_CompanyPoints[companyid][pointid][POINT_Z] = z;
    s_CompanyPoints[companyid][pointid][POINT_ANGLE] = angle;

    s_CompanyPoints[companyid][pointid][POINT_RADIUS] = radius;

    s_CompanyPoints[companyid][pointid][POINT_WORLD] = world;
    s_CompanyPoints[companyid][pointid][POINT_INTERIOR] = interior;

    s_CompanyPoints[companyid][pointid][POINT_EXTRA_DATA] = extra_data;

    //
    // Área dinâmica
    //

    new STREAMER_TAG_AREA:areaid =
        CreateDynamicSphere(
            x,
            y,
            z,
            radius,
            world,
            interior
        );

    s_CompanyPoints[companyid][pointid][POINT_AREA_ID] = areaid;

    World_RegisterArea(
        areaid,
        companyid,
        pointid
    );

    //
    // Pickup (opcional)
    //

    if (pickupModel != -1)
    {
        s_CompanyPoints[companyid][pointid][POINT_PICKUP_ID] =
            CreateDynamicPickup(
                pickupModel,
                pickupType,
                x,
                y,
                z,
                world,
                interior
            );
    }

    return pointid;
}

hook OnGameModeInit@5()
{
    BincoInit();
    LoadSkins();
    GymGantonInit();
    SexyShop_Init();
}

hook OnPlayerConnect@1(playerid)
{
    Company_CreatePlayerMapIcons(playerid);
}

hook OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
{
    new id = _:areaid;

    new companyid = s_WorldAreaData[id][WORLD_AREA_COMPANY];
    new pointid   = s_WorldAreaData[id][WORLD_AREA_POINT];

    CallLocalFunction(
        "OnCompanyPoint",
        "iii",
        playerid,
        companyid,
        pointid
    );

    return 1;
}

/**                                                                                                                                              
    -@@@@@@@@@@@@@@@@@@@@@@@:                                                                                               
    @@@@@@@@@@@@@@@@@@@@@@@@@@%                                                                                              
*@@@@@@@@@@@@@@@@@@@@@@@@@@@+                                                                                             
@@@@@@@@@@@=   @@@@@@@@@@@@@@                                                                                             
.@@@@@@@@@#  .:   @@@@@@@@@@@@                                                                                             
+@@@@@@@@% =@@@@@     %@@@@@@@+                                                                                            
*@@@@@@@@ :@@@@#  :@%   @@@@@@*                                                                                            
*@@@@@@#  @@@@  -@@@@@@ :@@@@@*                                                                                            
*@@@@@    @@   @@@@@@@@% @@@@@*        @@@@@:   @@                  %    :@@@@@#   -@@@@@@#  @@@@@@@@@                     
*@@@@  @= @@ @@@@:  -@@@ +@@@@*      .@@@@@@@-  @@                *@%   #@@@@@@@@  =@@@@@@@@ @@@@@@@@@                     
*@@@% @@= @@ @@* +@*  #@ :@@@@*      @@@   @@@  @@                @@%   @@#   #@@  =@@   %@@    #@@                        
*@@@  @@= @@ @   @@@@-   -@@@@*     .@@     #   @@.@@@#   @@@@@- @@@@@ @@@     -.  -@@    @@    #@@                        
*@@@ =@@= @@  @@@  @@@@  #@@@@*     %@@         @@@@@@@  *@@%@@@ @@@@@ @@*         -@@   %@@    #@@                        
*@@@ .@@= @@ @@@@@  -@@@: @@@@*     @@@         @@=  @@=  =   @@  @@%  @@=  :@@@@# -@@@@@@@%    #@@                        
*@@@: @@@ %@ @@@@@ @* @@@ +@@@*     %@@         @@   @@=   #@@@@  @@%  @@*  -@@@@% -@@@@@@*     #@@                        
*@@@@ =@@@.  @@@@@ @@ *@@. @@@*     -@@     @#  @@   @@= #@@@+@@  @@%  @@@     @@% -@@          #@@                        
*@@@@+ .@@@@ .@@@. @@ *@@- @@@*      @@%   @@@  @@   @@= @@   @@  @@%   @@+    @@% -@@          #@@                        
*@@@@:   +@@@%   @ @@ *@@  @@@*      =@@@@@@@:  @@   @@= @@@ @@@  @@@@  @@@@@@@@@# -@@          #@@                        
*@@@@. @=  %@# +@@ @@ *@@ %@@@*       -@@@@@=   @@   @@= :@@@@@@- .@@@   +@@@@@@.  :@@          *@@                        
*@@@@= @@@.  :@@@@ @@ *@  @@@@*          .                 .                .                                              
*@@@@@ @@@@@@@@@   @@    @@@@@*                                                                                            
*@@@@@  @@@@@@+  @@@@  *@@@@@@*                                                                                            
*@@@@@@  :@@=  *@@@@: @@@@@@@@*                                                                                            
*@@@@@@@*     @@@@@= @@@@@@@@@+                                                                                            
:@@@@@@@@@@@@  .=.  *@@@@@@@@@                                                                                             
@@@@@@@@@@@@@#   :@@@@@@@@@@@                                                                                             
#@@@@@@@@@@@@@@@@@@@@@@@@@@@#                                                                                             
    @@@@@@@@@@@@@@@@@@@@@@@@@@@                                                                                              
    +@@@@@@@@@@@@@@@@@@@@@@@=
 */