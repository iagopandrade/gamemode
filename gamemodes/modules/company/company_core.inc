#if defined _INC_company_core
    #endinput
#endif
#define _INC_company_core

#define MAX_COMPANIES               (500)

#if !defined MAX_DYNAMIC_PICKUPS
    #define MAX_DYNAMIC_PICKUPS     (8192)
#endif

#define COMPANY_TABLE               "companies"
#define COMPANY_INVALID_ID          (-1)
#define COMPANY_PICKUP_MODEL        (19135)

static enum e_COMPANY_DATA
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
    STREAMER_TAG_PICKUP:E_COMPANY_EXIT_PICKUP
};

static s_CompanyData[MAX_COMPANIES][e_COMPANY_DATA];

new Iterator:Companies<MAX_COMPANIES>;

static enum e_ENTITY_TYPE
{
    E_ENTITY_NONE,
    E_ENTITY_COMPANY_ENTER_PICKUP,
    E_ENTITY_COMPANY_EXIT_PICKUP
};

static enum e_PICKUP_ENTITY
{
    E_PICKUP_COMPANY,
    e_ENTITY_TYPE:E_PICKUP_TYPE
};

static s_PickupEntity[MAX_DYNAMIC_PICKUPS][e_PICKUP_ENTITY];

static s_PendingCompany[MAX_PLAYERS][e_COMPANY_DATA];

forward OnCompaniesLoaded();
forward OnCompanyInserted(playerid, companySlot);
forward OnCompanySaved(companyid, success);
forward OnCompanyDeleted(companyid, success);

stock bool:IsValidCompany(companyid)
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

stock bool:IsCompanyAvailable(companyid)
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
    return 1;
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

    new query[256];

    format(query, sizeof(query),
        "SELECT `id`, `name`, `locked`, `open_hour`, `close_hour`, \
        `enter_x`, `enter_y`, `enter_z`, `enter_interior`, \
        `exit_x`, `exit_y`, `exit_z`, `exit_interior` FROM `%s`",
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

        s_CompanyData[companyid][E_COMPANY_ENTER_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;
        s_CompanyData[companyid][E_COMPANY_EXIT_PICKUP] = STREAMER_TAG_PICKUP:INVALID_STREAMER_ID;

        Iter_Add(Companies, companyid);
        CreateCompanyPickups(companyid);

        loaded++;
    }

    printf("[company_core] %d empresa(s) carregada(s) do MySQL.", loaded);
    return 1;
}

// insere uma nova empresa no banco e, em caso de sucesso, cria a
// empresa em memoria e seus pickups. "playerid" e repassado para
// OnCompanyInserted apenas para que o chamador saiba quem solicitou
// a criacao (ex.: para enviar uma mensagem de confirmacao).
stock CreateCompanyInDB(
    playerid,
    const name[],
    open,
    close,
    Float:enterX, Float:enterY, Float:enterZ, enterInterior,
    Float:exitX, Float:exitY, Float:exitZ, exitInterior
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

    new query[400];
    format(query, sizeof(query),
        "INSERT INTO `%s` \
        (`name`, `locked`, `open_hour`, `close_hour`, \
        `enter_x`, `enter_y`, `enter_z`, `enter_interior`, \
        `exit_x`, `exit_y`, `exit_z`, `exit_interior`) VALUES \
        ('%s', 0, %d, %d, '%f', '%f', '%f', %d, '%f', '%f', '%f', %d)",
        COMPANY_TABLE,
        nameEscaped, open, close,
        enterX, enterY, enterZ, enterInterior,
        exitX, exitY, exitZ, exitInterior
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

    Iter_Add(Companies, companyid);
    CreateCompanyPickups(companyid);

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

    new query[450];
    format(query, sizeof(query),
        "UPDATE `%s` SET `name` = '%s', `locked` = %d, `open_hour` = %d, `close_hour` = %d, \
        `enter_x` = '%f', `enter_y` = '%f', `enter_z` = '%f', `enter_interior` = %d, \
        `exit_x` = '%f', `exit_y` = '%f', `exit_z` = '%f', `exit_interior` = %d \
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

hook OnPlayerPickUpDynamicPickup(playerid, STREAMER_TAG_PICKUP:pickupid)
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
    }
    return 1;
}

// Cria os pickups de todas as empresas ja carregadas e prepara o
// modulo assim que a gamemode inicia.
stock InitializeCompanySystem()
{
    for (new i = 0; i < MAX_DYNAMIC_PICKUPS; i++) {
        s_PickupEntity[i][E_PICKUP_COMPANY] = 0;
        s_PickupEntity[i][E_PICKUP_TYPE] = E_ENTITY_NONE;
    }

    Iter_Clear(Companies);
    return 1;
}
