/*
    FilterScript Sistema de casas C-Casas.
    Criado por Cleyson Stein - StNN
    Não retire os créditos.
    Todos os direitos reservados. 02/05/2018*
    Link: https://sampforum.blast.hk/showthread.php?tid=653371
*/
#if defined _INC_house_core
	#endinput
#endif
#define _INC_house_core

#define MAX_HOUSES (100)

enum pInfo
{
    Nome[24],
    TemCasa,
    EntrouCasa
};
new p[MAX_PLAYERS][pInfo];

enum cInfo
{
    cSqlID,

    cDono[24],
    Float:cX,
    Float:cY,
    Float:cZ,

    cTemDono,
    cTrancada,

    cIcone,
    cPickup,

    cInt,

    Text3D:cText,

    cPreco
};
new c[MAX_HOUSES][cInfo];
new TOTALCASA;

enum inter
{
    Float:iX,
    Float:iY,
    Float:iZ,
    Float:iA,
    iID
}

new interiors[][inter] =
{
    {2196.84, -1204.36, 1049.02, 94.0010, 6},
    {2259.38, -1135.89, 1050.64, 275.3992, 10},
    {2282.99, -1140.28, 1050.89, 358.4660, 11},
    {2233.69, -1115.26, 1050.88, 358.4660, 5},
    {2218.39, -1076.21, 1050.48, 95.2635, 1},
    {2496.00, -1692.08, 1014.74, 177.8159, 3},
    {2365.25, -1135.58, 1050.88, 359.0367, 8},
    {2317.77, -1026.76, 1050.21, 359.0367, 9},
    {2324.41, -1149.54, 1050.71, 359.0367, 12},
    {1260.6603, -785.4005, 1091.9063, 270.9891, 5},
    {140.28, 1365.92, 1083.85, 9.6901, 5}
};

new DB:Database;

public OnGameModeInit()
{
    print("\n-------------------------------------");
    print("Sistema C-casas carregado com sucesso.");
    print("   Criado por Cleyson Stein - StNN.");
    print("    Todos os direitos reservados.");
    print("-------------------------------------\n");
    ConectarDB();
    CarregarCasas();

    #if defined HouseCore_OnGameModeInit
        return HouseCore_OnGameModeInit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit HouseCore_OnGameModeInit

#if defined HouseCore_OnGameModeInit
    forward HouseCore_OnGameModeInit();
#endif

public OnGameModeExit()
{
    DB_Close(Database);
    for (new i = 0; i < MAX_HOUSES; ++i) {
        DestroyDynamicPickup(c[i][cPickup]);
        DestroyDynamicMapIcon(c[i][cIcone]);
        DestroyDynamic3DTextLabel(c[i][cText]);
    }
    
    #if defined HouseCore_OnGameModeExit
        return HouseCore_OnGameModeExit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif

#define OnGameModeExit HouseCore_OnGameModeExit

#if defined HouseCore_OnGameModeExit
    forward HouseCore_OnGameModeExit();
#endif

public OnPlayerConnect(playerid)
{
    GetPlayerName(playerid, p[playerid][Nome], 24);
    CarregarDadosPlayer(playerid);

    #if defined HouseCore_OnPlayerConnect
        return HouseCore_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect HouseCore_OnPlayerConnect

#if defined HouseCore_OnPlayerConnect
    forward HouseCore_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if (p[playerid][TemCasa] > 0) {
        SalvarDadosPlayer(playerid);
    }

    #if defined HouseCore_OnPlayerDisconnect
        return HouseCore_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect HouseCore_OnPlayerDisconnect

#if defined HouseCore_OnPlayerDisconnect
    forward HouseCore_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (newkeys == KEY_SECONDARY_ATTACK) {
        for (new i = 0; i < MAX_HOUSES; i++) {
            if (IsPlayerInRangeOfPoint(playerid, 1, c[i][cX], c[i][cY], c[i][cZ])) {
                if(c[i][cTemDono] == 0) {
                    SetPlayerPos(playerid, interiors[c[i][cInt]][iX], interiors[c[i][cInt]][iY], interiors[c[i][cInt]][iZ]);
                    SetPlayerFacingAngle(playerid, interiors[c[i][cInt]][iA]);
                    SetPlayerInterior(playerid, interiors[c[i][cInt]][iID]);
                    p[playerid][EntrouCasa] = i;
                    
                    SendClientMessage(
                        playerid,
                        COLOR_ORANGE,
                        "Casa a venda! Para comprar use /comprarcasa no pickup de fora. Use 'N' para sair."
                    );
                    break;   
                }
                if (c[i][cTemDono] == 1) {
                    if (c[i][cTrancada] == 1 && p[playerid][TemCasa] != i) {
                        SendClientMessage(playerid, COLOR_RED, "Casa trancada.");
                        return;
                    }
                    
                    if (p[playerid][TemCasa] == i) {
                        SetPlayerPos(playerid, interiors[c[i][cInt]][iX], interiors[c[i][cInt]][iY], interiors[c[i][cInt]][iZ]);
                        SetPlayerFacingAngle(playerid, interiors[c[i][cInt]][iA]);
                        SetPlayerInterior(playerid, interiors[c[i][cInt]][iID]);
                        p[playerid][EntrouCasa] = i;
                        
                        SendClientMessage(
                            playerid,
                            COLOR_ORANGE,
                            "Bem vindo a sua casa. Use 'N' para sair."
                        );
                        break;
                    } else {
                        SendClientMessage(playerid, COLOR_RED, "Essa casa nao eh sua.");
                        return;
                    }
                }
            }
        }
    }

    if (newkeys == KEY_NO) {
        if (p[playerid][EntrouCasa] > 0) {
            SetPlayerPos(playerid, c[p[playerid][EntrouCasa]][cX], c[p[playerid][EntrouCasa]][cY], c[p[playerid][EntrouCasa]][cZ]);
            SetPlayerInterior(playerid, 0);
            p[playerid][EntrouCasa] = 0;
        }
    }
}

// ============== DATA 
ConectarDB()
{
    if((Database = DB_Open("C-Casas.db")) == DB:0)
    {
        SendRconCommand("exit");
        print("Falha ao se conectar a DataBase SQlite"); // DEBUG
        
        } else {
        
        print("Conectado com sucesso a DataBase SQlite."); // DEBUG
    }
    //criar a db esta em db.txt
    new Query[280];
    format(Query, sizeof(Query), "CREATE TABLE IF NOT EXISTS CASAS(ID INTEGER PRIMARY KEY,\
    DONO TEXT DEFAULT '',\
    X FLOAT DEFAULT 0.0,\
    Y FLOAT DEFAULT 0.0,\
    Z FLOAT DEFAULT 0.0,\
    INTERIOR INTEGER DEFAULT 0,\
    TRANCADA INTEGER DEFAULT 0,\
    TEMDONO INTEGER DEFAULT 0,\
    PRECO INTEGER DEFAULT 0)");    
    DB_ExecuteQuery(Database, Query);
    format(Query, sizeof(Query), "CREATE TABLE IF NOT EXISTS DONOS(ID INTEGER PRIMARY KEY, NOME TEXT DEFAULT '', CASAID INTEGER DEFAULT 0)");    
    DB_ExecuteQuery(Database, Query);
    return 1;
}

CarregarDadosPlayer(playerid)
{
    new DBResult:Result, Query[65];
    format(Query, sizeof(Query), "SELECT * FROM DONOS WHERE NOME = '%s'", p[playerid][Nome]);
    Result = DB_ExecuteQuery(Database, Query);
    if(DB_GetRowCount(Result))
    {
        p[playerid][TemCasa] = DB_GetFieldIntByName(Result, "CASAID");
        } else {
        new Query2[75];
        format(Query2, sizeof(Query2), "INSERT INTO DONOS (NOME, CASAID) VALUES ('%s', 0)", p[playerid][Nome]);
        DB_ExecuteQuery(Database, Query2);
        p[playerid][TemCasa] = 0;
    }
    DB_FreeResultSet(Result);
    return 1;
}

SalvarDadosPlayer(playerid)
{
    new Query[75];
    format(Query, sizeof(Query), "UPDATE DONOS SET CASAID =%d WHERE NOME ='%s'", p[playerid][TemCasa], p[playerid][Nome]);
    DB_ExecuteQuery(Database, Query); 
    return 1;
}

CarregarCasas()
{
    new DBResult:result;

    result = DB_ExecuteQuery(Database,
        "SELECT * FROM CASAS");

    if(result == DBResult:0)
    {
        print("[HOUSE] Erro ao carregar casas.");
        return 0;
    }

    TOTALCASA = 0;

    while(DB_SelectNextRow(result))
    {
        new id;

        id = DB_GetFieldIntByName(result, "ID");

        if(id < 0 || id >= MAX_HOUSES)
            continue;

        DB_GetFieldStringByName(result,
            "DONO",
            c[id][cDono],
            24);

        c[id][cX] = DB_GetFieldFloatByName(result, "X");
        c[id][cY] = DB_GetFieldFloatByName(result, "Y");
        c[id][cZ] = DB_GetFieldFloatByName(result, "Z");

        c[id][cTemDono] =
            DB_GetFieldIntByName(result, "TEMDONO");

        c[id][cTrancada] =
            DB_GetFieldIntByName(result, "TRANCADA");

        c[id][cPreco] =
            DB_GetFieldIntByName(result, "PRECO");

        c[id][cInt] =
            DB_GetFieldIntByName(result, "INTERIOR");

        CriarCasa(id);

        TOTALCASA++;
    }

    DB_FreeResultSet(result);

    printf("[HOUSE] Casas carregadas: %d", TOTALCASA);

    return 1;
}

CriarCasa(slot)
{
    if (c[slot][cX] == 0.0 &&
       c[slot][cY] == 0.0 &&
       c[slot][cZ] == 0.0)
    {
        printf("[HOUSE] Casa %d possui coordenadas inválidas.", slot);
        return 0;
    }

    new texto[200];
    if (c[slot][cTemDono] == 0) {
        c[slot][cIcone] = CreateDynamicMapIcon(c[slot][cX], c[slot][cY], c[slot][cZ], 31, -1);
        c[slot][cPickup] = CreateDynamicPickup(1273, 21, c[slot][cX], c[slot][cY], c[slot][cZ]);
        format(texto, sizeof(texto), "{2E8B57}CASA ID: {FFFFFF}%d\n{3CB371}Dono: {FFFFFF}%s\n{3CB371}Preзo: {FFFFFF}$ %d\n{3CB371}Aperte 'F' para entrar\n/ajudacasa", slot, c[slot][cDono], c[slot][cPreco]);
        c[slot][cText] = CreateDynamic3DTextLabel(texto, 0xFFFFFFAA, c[slot][cX], c[slot][cY], c[slot][cZ]+1, 20);
    } else if (c[slot][cTemDono] == 1) {
        c[slot][cIcone] = CreateDynamicMapIcon(c[slot][cX], c[slot][cY], c[slot][cZ], 32, -1);
        c[slot][cPickup] = CreateDynamicPickup(19522, 21, c[slot][cX], c[slot][cY], c[slot][cZ]);//1272
        format(texto, sizeof(texto), "{B22222}CASA ID: {FFFFFF}%d\n{CD5C5C}Dono: {FFFFFF}%s\n{CD5C5C}Preзo: {FFFFFF}$ %d{CD5C5C}\n/ajudacasa", slot, c[slot][cDono], c[slot][cPreco]);
        c[slot][cText] = CreateDynamic3DTextLabel(texto, 0xFFFFFFAA, c[slot][cX], c[slot][cY], c[slot][cZ]+1, 20);
    }
    
    printf(
        "[HOUSE] Casa %d criada. Pickup=%d Icon=%d",
        slot,
        c[slot][cPickup],
        c[slot][cIcone]);

    return 1;
}

Criar_Casa(playerid, id, preco, interior)
{
    if(id < 0 || id >= MAX_HOUSES)
    {
        SendClientMessage(playerid, COLOR_RED, "ID invalido.");
        return 1;
    }

    new DBResult:result;
    new query[256];

    format(query, sizeof(query),
        "SELECT * FROM CASAS WHERE ID=%d",
        id);

    result = DB_ExecuteQuery(Database, query);

    if(DB_GetRowCount(result))
    {
        DB_FreeResultSet(result);
        SendClientMessage(playerid, COLOR_RED, "Esse ID ja existe.");
        return 1;
    }

    DB_FreeResultSet(result);

    GetPlayerPos(playerid,
        c[id][cX],
        c[id][cY],
        c[id][cZ]);

    format(c[id][cDono], 24, "Nenhum");

    c[id][cTemDono]  = 0;
    c[id][cTrancada] = 0;
    c[id][cPreco]    = preco;
    c[id][cInt]      = interior;

    format(query,sizeof(query),
        "INSERT INTO CASAS \
        (ID,DONO,X,Y,Z,INTERIOR,TRANCADA,TEMDONO,PRECO) \
        VALUES \
        (%d,'%s',%f,%f,%f,%d,%d,%d,%d)",
        id,
        c[id][cDono],
        c[id][cX],
        c[id][cY],
        c[id][cZ],
        c[id][cInt],
        c[id][cTrancada],
        c[id][cTemDono],
        c[id][cPreco]);

    DB_ExecuteQuery(Database, query);

    CriarCasa(id);

    TOTALCASA++;

    SendClientMessage(playerid,
        COLOR_ORANGE,
        "Casa criada com sucesso.");

    return 1;
}

Editar_Casa(playerid, id, preco, interior)
{
    if(id < 0 || id >= MAX_HOUSES)
        return 0;

    c[id][cPreco] = preco;
    c[id][cInt]   = interior;

    new query[128];

    format(query,sizeof(query),
        "UPDATE CASAS SET PRECO=%d, INTERIOR=%d WHERE ID=%d",
        preco,
        interior,
        id);

    DB_ExecuteQuery(Database,query);

    DestroyDynamicPickup(c[id][cPickup]);
    DestroyDynamicMapIcon(c[id][cIcone]);
    DestroyDynamic3DTextLabel(c[id][cText]);

    CriarCasa(id);

    SendClientMessage(playerid,
        COLOR_ORANGE,
        "Casa editada.");

    return 1;
}

Ir_Casa(playerid, slot)
{
    if(slot < 0 || slot >= TOTALCASA)
        return 0;

    SetPlayerPos(
        playerid,
        c[slot][cX],
        c[slot][cY],
        c[slot][cZ]);

    SetPlayerInterior(playerid,0);

    return 1;
}

Deletar_Casa(playerid, id)
{
    if(id < 0 || id >= MAX_HOUSES)
        return 0;

    new query[64];

    format(query,sizeof(query),
        "DELETE FROM CASAS WHERE ID=%d",
        id);

    DB_ExecuteQuery(Database,query);

    DestroyDynamicPickup(c[id][cPickup]);
    DestroyDynamicMapIcon(c[id][cIcone]);
    DestroyDynamic3DTextLabel(c[id][cText]);

    c[id][cX] = 0.0;
    c[id][cY] = 0.0;
    c[id][cZ] = 0.0;

    c[id][cPreco] = 0;
    c[id][cTemDono] = 0;
    c[id][cTrancada] = 0;
    c[id][cInt] = 0;

    format(c[id][cDono],24,"");

    TOTALCASA--;

    SendClientMessage(playerid,
        COLOR_ORANGE,
        "Casa removida.");

    return 1;
}
