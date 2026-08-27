#if defined _INC_company_core
    #endinput
#endif
#define _INC_company_core

#define MAX_COMPANIES               (500)

#if !defined MAX_DYNAMIC_PICKUPS
    #define MAX_DYNAMIC_PICKUPS     (8192)
#endif

#if !defined MAX_DYNAMIC_CPS
    #define MAX_DYNAMIC_CPS         (1024)
#endif

#define COMPANY_TABLE               "companies"
#define COMPANY_GENERIC_PICKUP_TABLE "company_pickups"
#define COMPANY_INVALID_ID          (-1)
#define COMPANY_PICKUP_MODEL        (19135)

#define MAX_COMPANY_GENERIC_PICKUPS (10)

#define COMPANY_GENERIC_PICKUP_MODEL (1239)

#define COMPANY_INTERACT_RADIUS     (3.0)

#define COMPANY_SALES_CP_SIZE       (3.0)

#define COMPANY_NO_OWNER            (0)

enum e_COMPANY_DATA
{
    E_COMPANY_SQL_ID,
    E_COMPANY_NAME[32],

    bool:E_COMPANY_LOCKED,

    E_COMPANY_OPEN,
    E_COMPANY_CLOSE,

    Float:E_COMPANY_ENTER_X,
    Float:E_COMPANY_ENTER_Y,
    Float:E_COMPANY_ENTER_Z,
    E_COMPANY_ENTER_INTERIOR,

    Float:E_COMPANY_EXIT_X,
    Float:E_COMPANY_EXIT_Y,
    Float:E_COMPANY_EXIT_Z,
    E_COMPANY_EXIT_INTERIOR,

    STREAMER_TAG_PICKUP:E_COMPANY_ENTER_PICKUP,
    STREAMER_TAG_PICKUP:E_COMPANY_EXIT_PICKUP,

    bool:E_COMPANY_SALES_SET,
    Float:E_COMPANY_SALES_X,
    Float:E_COMPANY_SALES_Y,
    Float:E_COMPANY_SALES_Z,
    E_COMPANY_SALES_INTERIOR,
    Float:E_COMPANY_SALES_SIZE,
    STREAMER_TAG_CP:E_COMPANY_SALES_CP,

    bool:E_COMPANY_MAP_ICON_ENABLED,
    E_COMPANY_MAP_ICON_TYPE,
    E_COMPANY_MAP_ICON_COLOR,
    STREAMER_TAG_MAP_ICON:E_COMPANY_MAP_ICON,

    E_COMPANY_CATEGORY,

    E_COMPANY_OWNER,

    E_COMPANY_CASH
};

new s_CompanyData[MAX_COMPANIES][e_COMPANY_DATA];

new Iterator:Companies<MAX_COMPANIES>;

enum e_COMPANY_GENERIC_PICKUP
{
    E_GENERIC_SQL_ID,
    Float:E_GENERIC_X,
    Float:E_GENERIC_Y,
    Float:E_GENERIC_Z,
    E_GENERIC_INTERIOR,
    E_GENERIC_MODEL,
    STREAMER_TAG_PICKUP:E_GENERIC_PICKUP
};

static s_CompanyGenericPickups[MAX_COMPANIES][MAX_COMPANY_GENERIC_PICKUPS][e_COMPANY_GENERIC_PICKUP];
static s_CompanyGenericPickupCount[MAX_COMPANIES];

static enum e_ENTITY_TYPE
{
    E_ENTITY_NONE,
    E_ENTITY_COMPANY_ENTER_PICKUP,
    E_ENTITY_COMPANY_EXIT_PICKUP,
    E_ENTITY_COMPANY_GENERIC_PICKUP
};

static enum e_PICKUP_ENTITY
{
    E_PICKUP_COMPANY,
    e_ENTITY_TYPE:E_PICKUP_TYPE,
    E_PICKUP_GENERIC_INDEX /* so usado quando E_PICKUP_TYPE == E_ENTITY_COMPANY_GENERIC_PICKUP */
};

static s_PickupEntity[MAX_DYNAMIC_PICKUPS][e_PICKUP_ENTITY];

static enum e_CP_ENTITY_TYPE
{
    E_CP_ENTITY_NONE,
    E_CP_ENTITY_COMPANY_SALES
};

static enum e_CP_ENTITY
{
    E_CP_COMPANY,
    e_CP_ENTITY_TYPE:E_CP_TYPE
};

static s_CheckpointEntity[MAX_DYNAMIC_CPS][e_CP_ENTITY];

static s_PendingCompany[MAX_PLAYERS][e_COMPANY_DATA];

forward OnCompaniesLoaded();
forward OnCompanyInserted(playerid, companySlot);
forward OnCompanySaved(companyid, success);
forward OnCompanyDeleted(companyid, success);

stock IsValidCompany(companyid)
{
    return (0 <= companyid < MAX_COMPANIES && Iter_Contains(Companies, companyid));
}

stock GetCompanyIndexFromSQLID(sql_id)
{
    if (sql_id <= 0) {
        return COMPANY_INVALID_ID;
    }

    foreach (new i : Companies) {
        if (s_CompanyData[i][E_COMPANY_SQL_ID] == sql_id) {
            return i;
        }
    }

    return COMPANY_INVALID_ID;
}

static stock FindFreeCompanySlot()
{
    for (new i = 0; i < MAX_COMPANIES; i++) {
        if (!Iter_Contains(Companies, i)) {
            return i;
        }
    }

    return COMPANY_INVALID_ID;
}

stock bool:IsCompanyLimitReached()
{
    return (Iter_Count(Companies) >= MAX_COMPANIES);
}

stock GetCompanySQLID(companyid)
{
    if (!IsValidCompany(companyid)) {
        return COMPANY_INVALID_ID;
    }

    return s_CompanyData[companyid][E_COMPANY_SQL_ID];
}

stock GetCompanyName(companyid, name[], size = sizeof(name))
{
    if (!IsValidCompany(companyid)) {
        name[0] = '\0';
        return 0;
    }

    format(name, size, "%s", s_CompanyData[companyid][E_COMPANY_NAME]);
    return 1;
}

stock GetCompanyBusinessHour(companyid, &open, &close)
{
    if (!IsValidCompany(companyid)) {
        open = 0;
        close = 0;
        return 0;
    }

    open = s_CompanyData[companyid][E_COMPANY_OPEN];
    close = s_CompanyData[companyid][E_COMPANY_CLOSE];
    return 1;
}

stock GetCompanyEnterPos(companyid, &Float:x, &Float:y, &Float:z, &interior)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    x = s_CompanyData[companyid][E_COMPANY_ENTER_X];
    y = s_CompanyData[companyid][E_COMPANY_ENTER_Y];
    z = s_CompanyData[companyid][E_COMPANY_ENTER_Z];
    interior = s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR];
    return 1;
}

stock GetCompanyExitPos(companyid, &Float:x, &Float:y, &Float:z, &interior)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    x = s_CompanyData[companyid][E_COMPANY_EXIT_X];
    y = s_CompanyData[companyid][E_COMPANY_EXIT_Y];
    z = s_CompanyData[companyid][E_COMPANY_EXIT_Z];
    interior = s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR];
    return 1;
}

stock bool:IsCompanyLocked(companyid)
{
    if (!IsValidCompany(companyid)) {
        return true;
    }

    return s_CompanyData[companyid][E_COMPANY_LOCKED];
}

stock bool:IsCompanyOpen(companyid)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    new open  = s_CompanyData[companyid][E_COMPANY_OPEN];
    new close = s_CompanyData[companyid][E_COMPANY_CLOSE];

    if (open == close) {
        return true;
    }

    new hour;
    GetServerTime(hour);

    if (open < close) {
        /* horario "normal", ex: 08 -> 18 */
        return bool:(hour >= open && hour < close);
    }

    /* horario que atravessa a meia-noite, ex: 22 -> 04 */
    return bool:(hour >= open || hour < close);
}

stock IsCompanyAvailable(companyid)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    if (IsCompanyLocked(companyid)) {
        return false;
    }

    if (!IsCompanyOpen(companyid)) {
        return false;
    }

    return true;
}

stock SetCompanyLocked(companyid, bool:locked)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_LOCKED] = locked;
    SaveCompany(companyid);
    return 1;
}

stock SetCompanyName(companyid, const name[])
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    if (isnull(name) || strlen(name) >= 32) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_NAME][0] = '\0';
    strcat(s_CompanyData[companyid][E_COMPANY_NAME], name);

    SaveCompany(companyid);
    return 1;
}

stock SetCompanyBusinessHour(companyid, open, close)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    if (!(0 <= open <= 23) || !(0 <= close <= 23)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_OPEN]  = open;
    s_CompanyData[companyid][E_COMPANY_CLOSE] = close;

    SaveCompany(companyid);
    return 1;
}

stock SetCompanyEnterPos(companyid, Float:x, Float:y, Float:z, interior)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_ENTER_X] = x;
    s_CompanyData[companyid][E_COMPANY_ENTER_Y] = y;
    s_CompanyData[companyid][E_COMPANY_ENTER_Z] = z;
    s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR] = interior;

    DestroyCompanyPickups(companyid);
    CreateCompanyPickups(companyid);

    CreateCompanyMapIcon(companyid);

    SaveCompany(companyid);
    return 1;
}

stock SetCompanyExitPos(companyid, Float:x, Float:y, Float:z, interior)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_EXIT_X] = x;
    s_CompanyData[companyid][E_COMPANY_EXIT_Y] = y;
    s_CompanyData[companyid][E_COMPANY_EXIT_Z] = z;
    s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR] = interior;

    DestroyCompanyPickups(companyid);
    CreateCompanyPickups(companyid);

    SaveCompany(companyid);
    return 1;
}

stock CreateCompanyPickups(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    new STREAMER_TAG_PICKUP:pickup;

    pickup = CreateDynamicPickup(
        COMPANY_PICKUP_MODEL,
        1,
        s_CompanyData[companyid][E_COMPANY_ENTER_X],
        s_CompanyData[companyid][E_COMPANY_ENTER_Y],
        s_CompanyData[companyid][E_COMPANY_ENTER_Z]
    );

    if (!IsValidDynamicPickup(pickup)) {
        printf("[company_core] ERRO: falha ao criar pickup de entrada da empresa %d (SQL ID %d)",
            companyid, s_CompanyData[companyid][E_COMPANY_SQL_ID]);
    } else if (_:pickup >= MAX_DYNAMIC_PICKUPS) {
        printf("[company_core] ERRO: pickup %d fora do limite de MAX_DYNAMIC_PICKUPS (%d)",
            _:pickup, MAX_DYNAMIC_PICKUPS);
        DestroyDynamicPickup(pickup);
    } else {
        s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP] = pickup;
        s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = companyid;
        s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_COMPANY_ENTER_PICKUP;
    }

    pickup = CreateDynamicPickup(
        COMPANY_PICKUP_MODEL,
        1,
        s_CompanyData[companyid][E_COMPANY_EXIT_X],
        s_CompanyData[companyid][E_COMPANY_EXIT_Y],
        s_CompanyData[companyid][E_COMPANY_EXIT_Z]
    );

    if (!IsValidDynamicPickup(pickup)) {
        printf("[company_core] ERRO: falha ao criar pickup de saida da empresa %d (SQL ID %d)",
            companyid, s_CompanyData[companyid][E_COMPANY_SQL_ID]);
    } else if (_:pickup >= MAX_DYNAMIC_PICKUPS) {
        printf("[company_core] ERRO: pickup %d fora do limite de MAX_DYNAMIC_PICKUPS (%d)",
            _:pickup, MAX_DYNAMIC_PICKUPS);
        DestroyDynamicPickup(pickup);
    } else {
        s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP] = pickup;
        s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = companyid;
        s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_COMPANY_EXIT_PICKUP;
    }

    CreateCompanySalesCheckpoint(companyid);
    CreateCompanyGenericPickups(companyid);
    return 1;
}

stock DestroyCompanyPickups(companyid)
{
    if (!(0 <= companyid < MAX_COMPANIES)) {
        return 0;
    }

    new STREAMER_TAG_PICKUP:pickup;

    pickup = s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP];
    if (IsValidDynamicPickup(pickup)) {
        if (_:pickup < MAX_DYNAMIC_PICKUPS) {
            s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = 0;
            s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_NONE;
        }
        DestroyDynamicPickup(pickup);
    }
    s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

    pickup = s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP];
    if (IsValidDynamicPickup(pickup)) {
        if (_:pickup < MAX_DYNAMIC_PICKUPS) {
            s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = 0;
            s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_NONE;
        }
        DestroyDynamicPickup(pickup);
    }
    s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

    DestroyCompanySalesCheckpoint(companyid);
    DestroyCompanyGenericPickups(companyid);
    return 1;
}

stock CreateCompanySalesCheckpoint(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    if (!s_CompanyData[companyid][E_COMPANY_SALES_SET]) {
        return 1; /* ainda nao foi adicionado, nada a criar */
    }

    new Float:size = s_CompanyData[companyid][E_COMPANY_SALES_SIZE];

    if (size <= 0.0) {
        size = COMPANY_SALES_CP_SIZE;
    }

    new STREAMER_TAG_CP:cp = CreateDynamicCP(
        s_CompanyData[companyid][E_COMPANY_SALES_X],
        s_CompanyData[companyid][E_COMPANY_SALES_Y],
        s_CompanyData[companyid][E_COMPANY_SALES_Z],
        size,
        -1,
        s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR]
    );

    if (!IsValidDynamicCP(cp)) {
        printf("[company_core] ERRO: falha ao criar checkpoint de vendas da empresa %d (SQL ID %d)",
            companyid, s_CompanyData[companyid][E_COMPANY_SQL_ID]);
        return 0;
    }

    if (_:cp >= MAX_DYNAMIC_CPS) {
        printf("[company_core] ERRO: checkpoint %d fora do limite de MAX_DYNAMIC_CPS (%d)",
            _:cp, MAX_DYNAMIC_CPS);
        DestroyDynamicCP(cp);
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_SALES_CP] = cp;
    s_CheckpointEntity[_:cp][E_CP_COMPANY] = companyid;
    s_CheckpointEntity[_:cp][E_CP_TYPE] = E_CP_ENTITY_COMPANY_SALES;
    return 1;
}

stock DestroyCompanySalesCheckpoint(companyid)
{
    if (!(0 <= companyid < MAX_COMPANIES)) {
        return 0;
    }

    new STREAMER_TAG_CP:cp = s_CompanyData[companyid][E_COMPANY_SALES_CP];

    if (IsValidDynamicCP(cp)) {
        if (_:cp < MAX_DYNAMIC_CPS) {
            s_CheckpointEntity[_:cp][E_CP_COMPANY] = 0;
            s_CheckpointEntity[_:cp][E_CP_TYPE] = E_CP_ENTITY_NONE;
        }
        DestroyDynamicCP(cp);
    }

    s_CompanyData[companyid][E_COMPANY_SALES_CP] = STREAMER_TAG_CP:INVALID_STREAMER_ID;
    return 1;
}

stock bool:AddCompanySalesCheckpoint(companyid, Float:x, Float:y, Float:z, interior, Float:size = 0.0)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    if (s_CompanyData[companyid][E_COMPANY_SALES_SET]) {
        return false;
    }

    if (size <= 0.0) {
        size = COMPANY_SALES_CP_SIZE;
    }

    s_CompanyData[companyid][E_COMPANY_SALES_SET] = true;
    s_CompanyData[companyid][E_COMPANY_SALES_X] = x;
    s_CompanyData[companyid][E_COMPANY_SALES_Y] = y;
    s_CompanyData[companyid][E_COMPANY_SALES_Z] = z;
    s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR] = interior;
    s_CompanyData[companyid][E_COMPANY_SALES_SIZE] = size;

    CreateCompanySalesCheckpoint(companyid);

    SaveCompany(companyid);
    return true;
}

stock bool:EditCompanySalesCheckpoint(companyid, Float:x, Float:y, Float:z, interior, Float:size = 0.0)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    if (!s_CompanyData[companyid][E_COMPANY_SALES_SET]) {
        return false; /* ainda nao existe - use AddCompanySalesCheckpoint */
    }

    if (size <= 0.0) {
        size = s_CompanyData[companyid][E_COMPANY_SALES_SIZE];
    }

    if (size <= 0.0) {
        size = COMPANY_SALES_CP_SIZE;
    }

    s_CompanyData[companyid][E_COMPANY_SALES_X] = x;
    s_CompanyData[companyid][E_COMPANY_SALES_Y] = y;
    s_CompanyData[companyid][E_COMPANY_SALES_Z] = z;
    s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR] = interior;
    s_CompanyData[companyid][E_COMPANY_SALES_SIZE] = size;

    DestroyCompanySalesCheckpoint(companyid);
    CreateCompanySalesCheckpoint(companyid);

    SaveCompany(companyid);
    return true;
}

stock bool:RemoveCompanySalesCheckpoint(companyid)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    if (!s_CompanyData[companyid][E_COMPANY_SALES_SET]) {
        return false; /* nao ha nada para remover */
    }

    DestroyCompanySalesCheckpoint(companyid);

    s_CompanyData[companyid][E_COMPANY_SALES_SET] = false;
    s_CompanyData[companyid][E_COMPANY_SALES_X] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_Y] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_Z] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR] = 0;
    s_CompanyData[companyid][E_COMPANY_SALES_SIZE] = 0.0;

    SaveCompany(companyid);
    return true;
}

stock bool:CompanyHasSalesCheckpoint(companyid)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    return s_CompanyData[companyid][E_COMPANY_SALES_SET];
}

stock bool:IsPlayerAtCompanySalesCheckpoint(playerid, companyid)
{
    if (!IsPlayerConnected(playerid) || !IsValidCompany(companyid)) {
        return false;
    }

    if (!s_CompanyData[companyid][E_COMPANY_SALES_SET]) {
        return false;
    }

    new Float:radius = s_CompanyData[companyid][E_COMPANY_SALES_SIZE];

    if (radius <= 0.0) {
        radius = COMPANY_INTERACT_RADIUS;
    }

    return bool:IsPlayerInRangeOfPoint(
        playerid,
        radius,
        s_CompanyData[companyid][E_COMPANY_SALES_X],
        s_CompanyData[companyid][E_COMPANY_SALES_Y],
        s_CompanyData[companyid][E_COMPANY_SALES_Z]
    );
}

stock GetCompanySalesCheckpointPlayerIsAt(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return COMPANY_INVALID_ID;
    }

    foreach (new i : Companies) {
        if (IsPlayerAtCompanySalesCheckpoint(playerid, i)) {
            return i;
        }
    }

    return COMPANY_INVALID_ID;
}

forward OnCompanyGenericPickUp(playerid, companyid, index);

stock GetCompanyGenericPickupCount(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    return s_CompanyGenericPickupCount[companyid];
}

static stock CreateCompanyGenericPickups(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    for (new i = 0; i < s_CompanyGenericPickupCount[companyid]; i++) {
        new model = s_CompanyGenericPickups[companyid][i][E_GENERIC_MODEL];

        if (model <= 0) {
            model = COMPANY_GENERIC_PICKUP_MODEL;
        }

        new STREAMER_TAG_PICKUP:pickup = CreateDynamicPickup(
            model,
            1,
            s_CompanyGenericPickups[companyid][i][E_GENERIC_X],
            s_CompanyGenericPickups[companyid][i][E_GENERIC_Y],
            s_CompanyGenericPickups[companyid][i][E_GENERIC_Z]
        );

        if (!IsValidDynamicPickup(pickup) || _:pickup >= MAX_DYNAMIC_PICKUPS) {
            printf("[company_core] ERRO: falha ao criar pickup generico %d da empresa %d",
                i, companyid);

            if (IsValidDynamicPickup(pickup)) {
                DestroyDynamicPickup(pickup);
            }

            s_CompanyGenericPickups[companyid][i][E_GENERIC_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
            continue;
        }

        s_CompanyGenericPickups[companyid][i][E_GENERIC_PICKUP] = pickup;
        s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = companyid;
        s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_COMPANY_GENERIC_PICKUP;
        s_PickupEntity[_:pickup][E_PICKUP_GENERIC_INDEX] = i;
    }
    return 1;
}

static stock DestroyCompanyGenericPickups(companyid)
{
    if (!(0 <= companyid < MAX_COMPANIES)) {
        return 0;
    }

    for (new i = 0; i < s_CompanyGenericPickupCount[companyid]; i++) {
        new STREAMER_TAG_PICKUP:pickup = s_CompanyGenericPickups[companyid][i][E_GENERIC_PICKUP];

        if (IsValidDynamicPickup(pickup)) {
            if (_:pickup < MAX_DYNAMIC_PICKUPS) {
                s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = 0;
                s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_NONE;
            }
            DestroyDynamicPickup(pickup);
        }

        s_CompanyGenericPickups[companyid][i][E_GENERIC_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
    }
    return 1;
}

stock AddCompanyGenericPickup(companyid, Float:x, Float:y, Float:z, interior, model)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    if (s_CompanyGenericPickupCount[companyid] >= MAX_COMPANY_GENERIC_PICKUPS) {
        return 0;
    }

    if (model <= 0) {
        model = COMPANY_GENERIC_PICKUP_MODEL;
    }

    new index = s_CompanyGenericPickupCount[companyid];

    s_CompanyGenericPickups[companyid][index][E_GENERIC_SQL_ID] = 0;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_X] = x;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_Y] = y;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_Z] = z;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_INTERIOR] = interior;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_MODEL] = model;
    s_CompanyGenericPickups[companyid][index][E_GENERIC_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

    s_CompanyGenericPickupCount[companyid]++;

    new sqlId = GetCompanySQLID(companyid);
    new query[256];
    format(query, sizeof(query),
        "INSERT INTO `%s` (`company_id`, `model`, `x`, `y`, `z`, `interior`) VALUES (%d, %d, '%f', '%f', '%f', %d)",
        COMPANY_GENERIC_PICKUP_TABLE, sqlId, model, x, y, z, interior
    );
    mysql_tquery(GetDatabaseHandle(), query, "OnCompanyGenericPickupInsertQuery", "ii", companyid, index);

    new STREAMER_TAG_PICKUP:pickup = CreateDynamicPickup(model, 1, x, y, z);

    if (IsValidDynamicPickup(pickup) && _:pickup < MAX_DYNAMIC_PICKUPS) {
        s_CompanyGenericPickups[companyid][index][E_GENERIC_PICKUP] = pickup;
        s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = companyid;
        s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_COMPANY_GENERIC_PICKUP;
        s_PickupEntity[_:pickup][E_PICKUP_GENERIC_INDEX] = index;
    }

    return index;
}

forward OnCompanyGenericPickupInsertQuery(companyid, index);
public OnCompanyGenericPickupInsertQuery(companyid, index)
{
    new insertId = cache_insert_id();

    if (IsValidCompany(companyid) && 0 <= index < s_CompanyGenericPickupCount[companyid]) {
        s_CompanyGenericPickups[companyid][index][E_GENERIC_SQL_ID] = insertId;
    }
    return 1;
}

stock bool:RemoveCompanyGenericPickup(companyid, index)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    if (!(0 <= index < s_CompanyGenericPickupCount[companyid])) {
        return false;
    }

    new STREAMER_TAG_PICKUP:pickup = s_CompanyGenericPickups[companyid][index][E_GENERIC_PICKUP];

    if (IsValidDynamicPickup(pickup)) {
        if (_:pickup < MAX_DYNAMIC_PICKUPS) {
            s_PickupEntity[_:pickup][E_PICKUP_COMPANY] = 0;
            s_PickupEntity[_:pickup][E_PICKUP_TYPE] = E_ENTITY_NONE;
        }
        DestroyDynamicPickup(pickup);
    }

    new sqlId = s_CompanyGenericPickups[companyid][index][E_GENERIC_SQL_ID];

    if (sqlId > 0) {
        new query[128];
        format(query, sizeof(query), "DELETE FROM `%s` WHERE `id` = %d", COMPANY_GENERIC_PICKUP_TABLE, sqlId);
        mysql_tquery(GetDatabaseHandle(), query);
    }

    /* fecha o "buraco" deixado pela remocao, preservando a ordem */
    for (new i = index; i < s_CompanyGenericPickupCount[companyid] - 1; i++) {
        s_CompanyGenericPickups[companyid][i] = s_CompanyGenericPickups[companyid][i + 1];

        new STREAMER_TAG_PICKUP:moved = s_CompanyGenericPickups[companyid][i][E_GENERIC_PICKUP];
        if (IsValidDynamicPickup(moved) && _:moved < MAX_DYNAMIC_PICKUPS) {
            s_PickupEntity[_:moved][E_PICKUP_GENERIC_INDEX] = i;
        }
    }

    s_CompanyGenericPickupCount[companyid]--;
    return true;
}

stock CreateCompanyMapIcon(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    if (IsValidDynamicMapIcon(
        s_CompanyData[companyid][E_COMPANY_MAP_ICON]
    )) {
        DestroyDynamicMapIcon(
            s_CompanyData[companyid][E_COMPANY_MAP_ICON]
        );
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON] =
        STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

    if (!s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED]) {
        return 1;
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON] =
        CreateDynamicMapIcon(
            s_CompanyData[companyid][E_COMPANY_ENTER_X],
            s_CompanyData[companyid][E_COMPANY_ENTER_Y],
            s_CompanyData[companyid][E_COMPANY_ENTER_Z],
            s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE],
            s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR],
            -1,
            s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR],
            -1,
            1000.0
        );

    if (!IsValidDynamicMapIcon(
        s_CompanyData[companyid][E_COMPANY_MAP_ICON]
    )) {
        printf(
            "[company_core] ERRO: falha ao criar map icon da empresa %d (SQL ID %d)",
            companyid,
            s_CompanyData[companyid][E_COMPANY_SQL_ID]
        );

        s_CompanyData[companyid][E_COMPANY_MAP_ICON] =
            STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

        return 0;
    }

    return 1;
}

stock DestroyCompanyMapIcon(companyid)
{
    if (!(0 <= companyid < MAX_COMPANIES)) {
        return 0;
    }

    if (IsValidDynamicMapIcon(
        s_CompanyData[companyid][E_COMPANY_MAP_ICON]
    )) {
        DestroyDynamicMapIcon(
            s_CompanyData[companyid][E_COMPANY_MAP_ICON]
        );
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON] =
        STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

    return 1;
}

stock SetCompanyMapIconEnabled(companyid, bool:enabled)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED] = enabled;

    CreateCompanyMapIcon(companyid);

    SaveCompany(companyid);
    return 1;
}

stock GetCompanyCategory(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    return s_CompanyData[companyid][E_COMPANY_CATEGORY];
}

stock SetCompanyCategory(companyid, category)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_CATEGORY] = category;

    SaveCompany(companyid);
    return 1;
}

stock GetCompanyOwner(companyid)
{
    if (!IsValidCompany(companyid)) {
        return COMPANY_NO_OWNER;
    }

    return s_CompanyData[companyid][E_COMPANY_OWNER];
}

stock bool:CompanyHasOwner(companyid)
{
    return (GetCompanyOwner(companyid) != COMPANY_NO_OWNER);
}

stock SetCompanyOwner(companyid, ownerid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_OWNER] = ownerid;

    SaveCompany(companyid);
    return 1;
}

stock RemoveCompanyOwner(companyid)
{
    return SetCompanyOwner(companyid, COMPANY_NO_OWNER);
}

stock GetCompanyCash(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    return s_CompanyData[companyid][E_COMPANY_CASH];
}

stock AddCompanyCash(companyid, amount)
{
    if (!IsValidCompany(companyid) || amount <= 0) {
        return 0;
    }

    s_CompanyData[companyid][E_COMPANY_CASH] += amount;

    SaveCompany(companyid);
    return 1;
}

stock bool:RemoveCompanyCash(companyid, amount)
{
    if (!IsValidCompany(companyid) || amount <= 0) {
        return false;
    }

    if (s_CompanyData[companyid][E_COMPANY_CASH] < amount) {
        return false;
    }

    s_CompanyData[companyid][E_COMPANY_CASH] -= amount;

    SaveCompany(companyid);
    return true;
}

stock bool:IsPlayerAtCompanyPickup(playerid, companyid)
{
    if (!IsPlayerConnected(playerid) || !IsValidCompany(companyid)) {
        return false;
    }

    if (IsPlayerInRangeOfPoint(
        playerid,
        COMPANY_INTERACT_RADIUS,
        s_CompanyData[companyid][E_COMPANY_ENTER_X],
        s_CompanyData[companyid][E_COMPANY_ENTER_Y],
        s_CompanyData[companyid][E_COMPANY_ENTER_Z]
    )) {
        return true;
    }

    if (IsPlayerInRangeOfPoint(
        playerid,
        COMPANY_INTERACT_RADIUS,
        s_CompanyData[companyid][E_COMPANY_EXIT_X],
        s_CompanyData[companyid][E_COMPANY_EXIT_Y],
        s_CompanyData[companyid][E_COMPANY_EXIT_Z]
    )) {
        return true;
    }

    return false;
}

stock GetCompanyPlayerIsAt(playerid)
{
    if (!IsPlayerConnected(playerid)) {
        return COMPANY_INVALID_ID;
    }

    foreach (new i : Companies) {
        if (IsPlayerAtCompanyPickup(playerid, i)) {
            return i;
        }
    }

    return COMPANY_INVALID_ID;
}

stock LoadCompanies()
{
    print("[DEBUG COMPANY] Entrou em LoadCompanies().");

    printf("[DEBUG COMPANY] Database handle: %d",_:GetDatabaseHandle());

    printf("[DEBUG COMPANY] Database connected: %d",_:IsDatabaseConnected());
    
    if (!IsDatabaseConnected()) {
        print("[company_core] ERRO: banco de dados nao conectado.");
        return 0;
    }

    new query[512];

    format(query, sizeof(query),
        "SELECT `id`, `name`, `locked`, `open_hour`, `close_hour`, \
        `enter_x`, `enter_y`, `enter_z`, `enter_interior`, \
        `exit_x`, `exit_y`, `exit_z`, `exit_interior`, \
        `sales_set`, `sales_x`, `sales_y`, `sales_z`, `sales_interior`, `sales_size`, \
        `map_icon_enabled`, `map_icon`, `map_icon_color`, \
        `pickup_category`, `owner_id`, `cash` \
        FROM `%s`",
        COMPANY_TABLE
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnCompaniesLoaded", "");
    return 1;
}

public OnCompaniesLoaded()
{
    print("[DEBUG COMPANY] OnCompaniesLoaded() executado.");

    new rows = cache_num_rows();

    printf("[DEBUG COMPANY] cache_num_rows(): %d", rows);

    if (rows <= 0) {
        print("[company_core] Nenhuma empresa encontrada no banco de dados.");
        return 1;
    }

    new loaded = 0;

    for (new row = 0; row < rows; row++) {
        new companyid = FindFreeCompanySlot();

        if (companyid == COMPANY_INVALID_ID) {
            printf(
                "[company_core] ERRO: limite de %d empresas atingido durante o carregamento.",
                MAX_COMPANIES
            );
            break;
        }

        cache_get_value_name_int(row, "id", s_CompanyData[companyid][E_COMPANY_SQL_ID]);
        cache_get_value_name(row, "name", s_CompanyData[companyid][E_COMPANY_NAME], 32);

        new lockedValue;
        cache_get_value_name_int(row, "locked", lockedValue);
        s_CompanyData[companyid][E_COMPANY_LOCKED] = bool:lockedValue;

        cache_get_value_name_int(row, "open_hour", s_CompanyData[companyid][E_COMPANY_OPEN]);
        cache_get_value_name_int(row, "close_hour", s_CompanyData[companyid][E_COMPANY_CLOSE]);

        cache_get_value_name_float(row, "enter_x", s_CompanyData[companyid][E_COMPANY_ENTER_X]);
        cache_get_value_name_float(row, "enter_y", s_CompanyData[companyid][E_COMPANY_ENTER_Y]);
        cache_get_value_name_float(row, "enter_z", s_CompanyData[companyid][E_COMPANY_ENTER_Z]);
        cache_get_value_name_int(row, "enter_interior", s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR]);

        cache_get_value_name_float(row, "exit_x", s_CompanyData[companyid][E_COMPANY_EXIT_X]);
        cache_get_value_name_float(row, "exit_y", s_CompanyData[companyid][E_COMPANY_EXIT_Y]);
        cache_get_value_name_float(row, "exit_z", s_CompanyData[companyid][E_COMPANY_EXIT_Z]);
        cache_get_value_name_int(row, "exit_interior", s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR]);

        new salesSet;
        cache_get_value_name_int(row, "sales_set", salesSet);
        s_CompanyData[companyid][E_COMPANY_SALES_SET] = bool:salesSet;

        cache_get_value_name_float(row, "sales_x", s_CompanyData[companyid][E_COMPANY_SALES_X]);
        cache_get_value_name_float(row, "sales_y", s_CompanyData[companyid][E_COMPANY_SALES_Y]);
        cache_get_value_name_float(row, "sales_z", s_CompanyData[companyid][E_COMPANY_SALES_Z]);
        cache_get_value_name_int(row, "sales_interior", s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR]);
        cache_get_value_name_float(row, "sales_size", s_CompanyData[companyid][E_COMPANY_SALES_SIZE]);

        cache_get_value_name_int(
            row,
            "map_icon_enabled",
            s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED]
        );

        cache_get_value_name_int(
            row,
            "map_icon",
            s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE]
        );

        cache_get_value_name_int(
            row,
            "map_icon_color",
            s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR]
        );

        cache_get_value_name_int(
            row,
            "pickup_category",
            s_CompanyData[companyid][E_COMPANY_CATEGORY]
        );

        cache_get_value_name_int(
            row,
            "owner_id",
            s_CompanyData[companyid][E_COMPANY_OWNER]
        );

        cache_get_value_name_int(
            row,
            "cash",
            s_CompanyData[companyid][E_COMPANY_CASH]
        );

        s_CompanyData[companyid][E_COMPANY_MAP_ICON] = STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

        s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
        s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
        s_CompanyData[companyid][E_COMPANY_SALES_CP] = STREAMER_TAG_CP:INVALID_STREAMER_ID;

        s_CompanyGenericPickupCount[companyid] = 0;

        Iter_Add(Companies, companyid);
        
        CreateCompanyPickups(companyid);

        CreateCompanyMapIcon(companyid);

        loaded++;
    }

    printf("[company_core] %d empresa(s) carregada(s) do MySQL.", loaded);

    LoadCompanyGenericPickups();
    return 1;
}

stock LoadCompanyGenericPickups()
{
    if (!IsDatabaseConnected()) {
        return 0;
    }

    new query[128];
    format(query, sizeof(query), "SELECT `id`, `company_id`, `model`, `x`, `y`, `z`, `interior` FROM `%s`", COMPANY_GENERIC_PICKUP_TABLE);

    mysql_tquery(GetDatabaseHandle(), query, "OnCompanyGenericPickupsLoaded", "");
    return 1;
}

forward OnCompanyGenericPickupsLoaded();
public OnCompanyGenericPickupsLoaded()
{
    new rows = cache_num_rows();

    if (rows <= 0) {
        return 1;
    }

    new loaded = 0;

    for (new row = 0; row < rows; row++) {
        new sqlCompanyId;
        cache_get_value_name_int(row, "company_id", sqlCompanyId);

        new companyid = GetCompanyIndexFromSQLID(sqlCompanyId);

        if (!IsValidCompany(companyid)) {
            continue; /* empresa foi removida mas o pickup generico ficou orfao */
        }

        if (s_CompanyGenericPickupCount[companyid] >= MAX_COMPANY_GENERIC_PICKUPS) {
            continue;
        }

        new index = s_CompanyGenericPickupCount[companyid];

        cache_get_value_name_int(row, "id", s_CompanyGenericPickups[companyid][index][E_GENERIC_SQL_ID]);
        cache_get_value_name_int(row, "model", s_CompanyGenericPickups[companyid][index][E_GENERIC_MODEL]);
        cache_get_value_name_float(row, "x", s_CompanyGenericPickups[companyid][index][E_GENERIC_X]);
        cache_get_value_name_float(row, "y", s_CompanyGenericPickups[companyid][index][E_GENERIC_Y]);
        cache_get_value_name_float(row, "z", s_CompanyGenericPickups[companyid][index][E_GENERIC_Z]);
        cache_get_value_name_int(row, "interior", s_CompanyGenericPickups[companyid][index][E_GENERIC_INTERIOR]);
        s_CompanyGenericPickups[companyid][index][E_GENERIC_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

        s_CompanyGenericPickupCount[companyid]++;
        loaded++;
    }

    /* agora que todos os indices por empresa estao montados, cria os pickups */
    foreach (new i : Companies) {
        CreateCompanyGenericPickups(i);
    }

    printf("[company_core] %d pickup(s) generico(s) carregado(s) do MySQL.", loaded);
    return 1;
}

stock CreateCompanyInDB(
    playerid,
    const name[],
    open,
    close,
    Float:enterX, Float:enterY, Float:enterZ, enterInterior,
    Float:exitX, Float:exitY, Float:exitZ, exitInterior,
    category = 0
)
{
    if (isnull(name) || strlen(name) >= 32) {
        printf("[company_core] ERRO: nome invalido ao tentar criar empresa (player %d).", playerid);
        return 0;
    }

    if (!(0 <= open <= 23) || !(0 <= close <= 23)) {
        printf("[company_core] ERRO: horario invalido ao tentar criar empresa (player %d).", playerid);
        return 0;
    }

    if (IsCompanyLimitReached()) {
        printf("[company_core] ERRO: limite de %d empresas atingido, criacao recusada.", MAX_COMPANIES);
        return 0;
    }

    new nameEscaped[65];
    mysql_escape_string(name, nameEscaped);

    new query[620];
    format(query, sizeof(query),
        "INSERT INTO `%s` \
        (`name`, `locked`, `open_hour`, `close_hour`, \
        `enter_x`, `enter_y`, `enter_z`, `enter_interior`, \
        `exit_x`, `exit_y`, `exit_z`, `exit_interior`, \
        `sales_set`, `sales_x`, `sales_y`, `sales_z`, `sales_interior`, `sales_size`, \
        `pickup_category`, `owner_id`, `cash`) VALUES \
        ('%s', 0, %d, %d, '%f', '%f', '%f', %d, '%f', '%f', '%f', %d, 0, '0', '0', '0', 0, '0', %d, 0, 0)",
        COMPANY_TABLE,
        nameEscaped, open, close,
        enterX, enterY, enterZ, enterInterior,
        exitX, exitY, exitZ, exitInterior,
        category
    );

    /* guarda temporariamente os dados para o callback de insercao */
    s_PendingCompany[playerid][E_COMPANY_NAME][0] = '\0';
    strcat(s_PendingCompany[playerid][E_COMPANY_NAME], name);
    s_PendingCompany[playerid][E_COMPANY_OPEN] = open;
    s_PendingCompany[playerid][E_COMPANY_CLOSE] = close;
    s_PendingCompany[playerid][E_COMPANY_ENTER_X] = enterX;
    s_PendingCompany[playerid][E_COMPANY_ENTER_Y] = enterY;
    s_PendingCompany[playerid][E_COMPANY_ENTER_Z] = enterZ;
    s_PendingCompany[playerid][E_COMPANY_ENTER_INTERIOR] = enterInterior;
    s_PendingCompany[playerid][E_COMPANY_EXIT_X] = exitX;
    s_PendingCompany[playerid][E_COMPANY_EXIT_Y] = exitY;
    s_PendingCompany[playerid][E_COMPANY_EXIT_Z] = exitZ;
    s_PendingCompany[playerid][E_COMPANY_EXIT_INTERIOR] = exitInterior;
    s_PendingCompany[playerid][E_COMPANY_CATEGORY] = category;

    mysql_tquery(GetDatabaseHandle(), query, "OnCompanyInsertQuery", "d", playerid);
    return 1;
}

forward OnCompanyInsertQuery(playerid);
public OnCompanyInsertQuery(playerid)
{
    new insertId = cache_insert_id();

    if (insertId <= 0) {
        printf(
            "[company_core] ERRO: falha ao inserir empresa no banco (player %d).",
            playerid
        );
        return CallLocalFunction("OnCompanyInserted", "ii", playerid, COMPANY_INVALID_ID);
    }

    new companyid = FindFreeCompanySlot();

    if (companyid == COMPANY_INVALID_ID) {
        printf(
            "[company_core] ERRO: empresa inserida no banco (ID %d) mas nao ha slots livres em memoria.",
            insertId
        );
        return CallLocalFunction("OnCompanyInserted", "ii", playerid, COMPANY_INVALID_ID);
    }

    s_CompanyData[companyid][E_COMPANY_SQL_ID] = insertId;
    s_CompanyData[companyid][E_COMPANY_NAME][0] = '\0';
    strcat(s_CompanyData[companyid][E_COMPANY_NAME], s_PendingCompany[playerid][E_COMPANY_NAME]);
    s_CompanyData[companyid][E_COMPANY_LOCKED] = false;
    s_CompanyData[companyid][E_COMPANY_OPEN] = s_PendingCompany[playerid][E_COMPANY_OPEN];
    s_CompanyData[companyid][E_COMPANY_CLOSE] = s_PendingCompany[playerid][E_COMPANY_CLOSE];
    s_CompanyData[companyid][E_COMPANY_ENTER_X] = s_PendingCompany[playerid][E_COMPANY_ENTER_X];
    s_CompanyData[companyid][E_COMPANY_ENTER_Y] = s_PendingCompany[playerid][E_COMPANY_ENTER_Y];
    s_CompanyData[companyid][E_COMPANY_ENTER_Z] = s_PendingCompany[playerid][E_COMPANY_ENTER_Z];
    s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR] = s_PendingCompany[playerid][E_COMPANY_ENTER_INTERIOR];
    s_CompanyData[companyid][E_COMPANY_EXIT_X] = s_PendingCompany[playerid][E_COMPANY_EXIT_X];
    s_CompanyData[companyid][E_COMPANY_EXIT_Y] = s_PendingCompany[playerid][E_COMPANY_EXIT_Y];
    s_CompanyData[companyid][E_COMPANY_EXIT_Z] = s_PendingCompany[playerid][E_COMPANY_EXIT_Z];
    s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR] = s_PendingCompany[playerid][E_COMPANY_EXIT_INTERIOR];
    s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
    s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

    s_CompanyData[companyid][E_COMPANY_SALES_SET] = false;
    s_CompanyData[companyid][E_COMPANY_SALES_X] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_Y] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_Z] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR] = 0;
    s_CompanyData[companyid][E_COMPANY_SALES_SIZE] = 0.0;
    s_CompanyData[companyid][E_COMPANY_SALES_CP] = STREAMER_TAG_CP:INVALID_STREAMER_ID;

    s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED] = false;
    s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE] = 0;
    s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR] = 0xFFFFFFFF;
    s_CompanyData[companyid][E_COMPANY_MAP_ICON] = STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

    s_CompanyData[companyid][E_COMPANY_CATEGORY] = s_PendingCompany[playerid][E_COMPANY_CATEGORY];
    s_CompanyData[companyid][E_COMPANY_OWNER] = COMPANY_NO_OWNER;
    s_CompanyData[companyid][E_COMPANY_CASH] = 0;

    s_CompanyGenericPickupCount[companyid] = 0;

    Iter_Add(Companies, companyid);

    CreateCompanyPickups(companyid);
    CreateCompanyMapIcon(companyid);

    printf(
        "[company_core] Empresa '%s' criada (SQL ID %d, slot %d) pelo player %d.",
        s_CompanyData[companyid][E_COMPANY_NAME], insertId, companyid, playerid
    );

    CallLocalFunction("OnCompanyInserted", "ii", playerid, companyid);
    return 1;
}

stock SaveCompany(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    new nameEscaped[65];
    mysql_escape_string(s_CompanyData[companyid][E_COMPANY_NAME], nameEscaped);

    new query[1024];
    format(query, sizeof(query),
        "UPDATE `%s` SET \
        `name` = '%s', \
        `locked` = %d, \
        `open_hour` = %d, \
        `close_hour` = %d, \
        `enter_x` = '%f', \
        `enter_y` = '%f', \
        `enter_z` = '%f', \
        `enter_interior` = %d, \
        `exit_x` = '%f', \
        `exit_y` = '%f', \
        `exit_z` = '%f', \
        `exit_interior` = %d, \
        `sales_set` = %d, \
        `sales_x` = '%f', \
        `sales_y` = '%f', \
        `sales_z` = '%f', \
        `sales_interior` = %d, \
        `sales_size` = '%f', \
        `map_icon_enabled` = %d, \
        `map_icon` = %d, \
        `map_icon_color` = %u, \
        `pickup_category` = %d, \
        `owner_id` = %d, \
        `cash` = %d \
        WHERE `id` = %d",
        COMPANY_TABLE,
        nameEscaped,

        _:s_CompanyData[companyid][E_COMPANY_LOCKED],

        s_CompanyData[companyid][E_COMPANY_OPEN],
        s_CompanyData[companyid][E_COMPANY_CLOSE],

        s_CompanyData[companyid][E_COMPANY_ENTER_X],
        s_CompanyData[companyid][E_COMPANY_ENTER_Y],
        s_CompanyData[companyid][E_COMPANY_ENTER_Z],
        s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR],

        s_CompanyData[companyid][E_COMPANY_EXIT_X],
        s_CompanyData[companyid][E_COMPANY_EXIT_Y],
        s_CompanyData[companyid][E_COMPANY_EXIT_Z],
        s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR],

        _:s_CompanyData[companyid][E_COMPANY_SALES_SET],
        s_CompanyData[companyid][E_COMPANY_SALES_X],
        s_CompanyData[companyid][E_COMPANY_SALES_Y],
        s_CompanyData[companyid][E_COMPANY_SALES_Z],
        s_CompanyData[companyid][E_COMPANY_SALES_INTERIOR],
        s_CompanyData[companyid][E_COMPANY_SALES_SIZE],

        _:s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED],
        s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE],
        s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR],

        s_CompanyData[companyid][E_COMPANY_CATEGORY],
        s_CompanyData[companyid][E_COMPANY_OWNER],
        s_CompanyData[companyid][E_COMPANY_CASH],

        s_CompanyData[companyid][E_COMPANY_SQL_ID]
    );

    mysql_tquery(GetDatabaseHandle(), query, "OnCompanySaveQuery", "i", companyid);
    return 1;
}

forward OnCompanySaveQuery(companyid);
public OnCompanySaveQuery(companyid)
{
    new affected = cache_affected_rows();

    if (affected < 1) {
        printf(
            "[company_core] AVISO: UPDATE da empresa %d (SQL ID %d) nao afetou nenhuma linha.",
            companyid, IsValidCompany(companyid) ? s_CompanyData[companyid][E_COMPANY_SQL_ID] : -1
        );
    }

    CallLocalFunction("OnCompanySaved", "ii", companyid, affected >= 1);
    return 1;
}

stock DeleteCompany(companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    new sqlId = s_CompanyData[companyid][E_COMPANY_SQL_ID];

    new query[96];
    format(query, sizeof(query), "DELETE FROM `%s` WHERE `id` = %d", COMPANY_TABLE, sqlId);

    DestroyCompanyPickups(companyid);
    DestroyCompanyMapIcon(companyid);

    s_CompanyGenericPickupCount[companyid] = 0;

    Iter_Remove(Companies, companyid);

    mysql_tquery(GetDatabaseHandle(), query, "OnCompanyDeleteQuery", "ii", companyid, sqlId);
    return 1;
}

forward OnCompanyDeleteQuery(companyid, sqlId);
public OnCompanyDeleteQuery(companyid, sqlId)
{
    new affected = cache_affected_rows();

    if (affected < 1) {
        printf("[company_core] AVISO: DELETE da empresa SQL ID %d nao afetou nenhuma linha.", sqlId);
    } else {
        printf("[company_core] Empresa SQL ID %d removida do banco de dados.", sqlId);
    }

    CallLocalFunction("OnCompanyDeleted", "ii", companyid, affected >= 1);
    return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP:pickupid)
{
    if (_:pickupid >= MAX_DYNAMIC_PICKUPS) {
        return 1;
    }

    new type = _:s_PickupEntity[_:pickupid][E_PICKUP_TYPE];

    if (type == _:E_ENTITY_NONE) {
        return 1;
    }

    new companyid = s_PickupEntity[_:pickupid][E_PICKUP_COMPANY];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    switch (type) {
        case E_ENTITY_COMPANY_ENTER_PICKUP: {
            if (!IsCompanyAvailable(companyid)) {
                if (IsCompanyLocked(companyid)) {
                    SendClientMessage(playerid, -1, "A empresa esta trancada no momento.");
                } else {
                    SendClientMessage(playerid, -1, "A empresa esta fechada no momento.");
                }
                return 1;
            }

            SetPlayerPos(
                playerid,
                s_CompanyData[companyid][E_COMPANY_EXIT_X],
                s_CompanyData[companyid][E_COMPANY_EXIT_Y] + 2.0,
                s_CompanyData[companyid][E_COMPANY_EXIT_Z]
            );

            SetPlayerInterior(playerid, s_CompanyData[companyid][E_COMPANY_EXIT_INTERIOR]);
        }

        case E_ENTITY_COMPANY_EXIT_PICKUP: {
            SetPlayerPos(
                playerid,
                s_CompanyData[companyid][E_COMPANY_ENTER_X],
                s_CompanyData[companyid][E_COMPANY_ENTER_Y] + 2.0,
                s_CompanyData[companyid][E_COMPANY_ENTER_Z]
            );

            SetPlayerInterior(playerid, s_CompanyData[companyid][E_COMPANY_ENTER_INTERIOR]);
        }

        case E_ENTITY_COMPANY_GENERIC_PICKUP: {
            new index = s_PickupEntity[_:pickupid][E_PICKUP_GENERIC_INDEX];
            CallLocalFunction("OnCompanyGenericPickUp", "iii", playerid, companyid, index);
        }
    }
    return 1;
}

public OnPlayerEnterDynamicCP(playerid, STREAMER_TAG_CP:checkpointid)
{
    if (_:checkpointid >= MAX_DYNAMIC_CPS) {
        return 1;
    }

    if (s_CheckpointEntity[_:checkpointid][E_CP_TYPE] != E_CP_ENTITY_COMPANY_SALES) {
        return 1;
    }

    new companyid = s_CheckpointEntity[_:checkpointid][E_CP_COMPANY];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    if (!IsCompanyAvailable(companyid)) {
        if (IsCompanyLocked(companyid)) {
            SendClientMessage(playerid, -1, "A empresa esta trancada no momento.");
        } else {
            SendClientMessage(playerid, -1, "A empresa esta fechada no momento.");
        }
        return 1;
    }

    Company_ShowPurchaseDialog(playerid, companyid);
    return 1;
}

stock InitializeCompanySystem()
{
    for (new i = 0; i < MAX_DYNAMIC_PICKUPS; i++) {
        s_PickupEntity[i][E_PICKUP_COMPANY] = 0;
        s_PickupEntity[i][E_PICKUP_TYPE] = E_ENTITY_NONE;
    }

    for (new i = 0; i < MAX_DYNAMIC_CPS; i++) {
        s_CheckpointEntity[i][E_CP_COMPANY] = 0;
        s_CheckpointEntity[i][E_CP_TYPE] = E_CP_ENTITY_NONE;
    }

    for (new i = 0; i < MAX_COMPANIES; i++) {
        s_CompanyGenericPickupCount[i] = 0;
    }

    Iter_Clear(Companies);
    return 1;
}
