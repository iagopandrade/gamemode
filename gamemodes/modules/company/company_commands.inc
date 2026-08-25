#if defined _INC_company_commands
    #endinput
#endif
#define _INC_company_commands

#define COMPANY_BREAKIN_TIME        (15000)
#define COMPANY_BREAKIN_TICK        (1000)

stock Company_GetPlayerOwnerID(playerid)
{
    return GetAccountId(playerid);
}

stock bool:Company_IsPlayerOwner(playerid, companyid)
{
    if (!IsValidCompany(companyid)) {
        return false;
    }

    new ownerid = GetCompanyOwner(companyid);

    if (ownerid == COMPANY_NO_OWNER) {
        return false;
    }

    return bool:(ownerid == Company_GetPlayerOwnerID(playerid));
}

flags:empresasetentrada(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresasetentrada(playerid, params[])
{
    new sqlId, companyid;

    if (sscanf(params, "d", sqlId) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresasetentrada [id] (o mesmo ID mostrado em /editarempresa)");
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    SetCompanyEnterPos(companyid, x, y, z, GetPlayerInterior(playerid));
    SendClientMessage(playerid, -1, "Entrada da empresa atualizada para a sua posicao atual.");
    return 1;
}

flags:empresasetsaida(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresasetsaida(playerid, params[])
{
    new sqlId, companyid;

    if (sscanf(params, "d", sqlId) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresasetsaida [id] (o mesmo ID mostrado em /editarempresa)");
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    SetCompanyExitPos(companyid, x, y, z, GetPlayerInterior(playerid));
    SendClientMessage(playerid, -1, "Saida da empresa atualizada para a sua posicao atual.");
    return 1;
}

flags:empresasetmapicone(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresasetmapicone(playerid, params[])
{
    new sqlId, companyid, tipo, corHex[9];

    if (sscanf(params, "dds[9]", sqlId, tipo, corHex) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresasetmapicone [id] [tipo 0-63] [cor hexadecimal, ex: FFFFFFFF]");
        return 1;
    }

    if (!(0 <= tipo <= 63)) {
        SendClientMessage(playerid, -1, "O tipo do icone deve estar entre 0 e 63.");
        return 1;
    }

    new cor;
    if (sscanf(corHex, "h", cor)) {
        SendClientMessage(playerid, -1, "Cor invalida. Exemplo: FFFFFFFF");
        return 1;
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE] = tipo;
    s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR] = cor;

    SetCompanyMapIconEnabled(companyid, true);

    SendClientMessage(playerid, -1, "Icone do mapa da empresa atualizado.");
    return 1;
}

flags:empresasetpickup(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresasetpickup(playerid, params[])
{
    new sqlId, companyid, categoria;

    if (sscanf(params, "dd", sqlId, categoria) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        new msg[220];
        format(msg, sizeof(msg),
            "Uso: /empresasetpickup [id] [categoria] (afeta so o pickup de VENDAS)\n\
            0 = Generica, 1 = Roupas, 2 = 24 Horas, 3 = Restaurante, 4 = Armaria"
        );
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    if (!(0 <= categoria < _:E_COMPANY_CAT_MAX)) {
        SendClientMessage(playerid, -1, "Categoria inválida.");
        return 1;
    }

    SetCompanyCategory(companyid, categoria);

    new categoryName[24];
    GetCompanyCategoryName(categoria, categoryName);

    new msg[150];
    if (CompanyHasSalesCheckpoint(companyid)) {
        format(msg, sizeof(msg), "Categoria da empresa atualizada para: %s.", categoryName);
    } else {
        format(msg, sizeof(msg), "Categoria da empresa atualizada para: %s. Use /empresaaddcheckpoint %d para adicionar o checkpoint de vendas.", categoryName, sqlId);
    }
    SendClientMessage(playerid, -1, msg);
    return 1;
}

flags:empresaaddcheckpoint(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaaddcheckpoint(playerid, params[])
{
    new sqlId, companyid;
    new Float:tamanho = 0.0;

    if (sscanf(params, "d[f]", sqlId, tamanho) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaaddcheckpoint [id] [tamanho opcional]");
        return 1;
    }

    if (CompanyHasSalesCheckpoint(companyid)) {
        new msg[150];
        format(msg, sizeof(msg), "Esta empresa ja tem um checkpoint de vendas. Use /empresaeditarcheckpoint %d para reposiciona-lo.", sqlId);
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    AddCompanySalesCheckpoint(companyid, x, y, z, GetPlayerInterior(playerid), tamanho);
    SendClientMessage(playerid, -1, "Checkpoint de vendas adicionado na sua posicao atual.");
    return 1;
}

flags:empresaeditarcheckpoint(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaeditarcheckpoint(playerid, params[])
{
    new sqlId, companyid;
    new Float:tamanho = 0.0;

    if (sscanf(params, "d[f]", sqlId, tamanho) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaeditarcheckpoint [id] [tamanho opcional]");
        return 1;
    }

    if (!CompanyHasSalesCheckpoint(companyid)) {
        new msg[150];
        format(msg, sizeof(msg), "Esta empresa ainda nao tem um checkpoint de vendas. Use /empresaaddcheckpoint %d para adiciona-lo.", sqlId);
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    EditCompanySalesCheckpoint(companyid, x, y, z, GetPlayerInterior(playerid), tamanho);
    SendClientMessage(playerid, -1, "Checkpoint de vendas atualizado para a sua posicao atual.");
    return 1;
}

flags:empresaremovercheckpoint(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaremovercheckpoint(playerid, params[])
{
    new sqlId, companyid;

    if (sscanf(params, "d", sqlId) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaremovercheckpoint [id]");
        return 1;
    }

    if (!RemoveCompanySalesCheckpoint(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao tem um checkpoint de vendas para remover.");
        return 1;
    }

    SendClientMessage(playerid, -1, "Checkpoint de vendas removido.");
    return 1;
}

flags:empresaaddpickupgenerico(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaaddpickupgenerico(playerid, params[])
{
    new sqlId, companyid, modelo = 0;

    if (sscanf(params, "d[d]", sqlId, modelo) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaaddpickupgenerico [id] [modelo (opcional)]");
        return 1;
    }

    if (GetCompanyGenericPickupCount(companyid) >= MAX_COMPANY_GENERIC_PICKUPS) {
        SendClientMessage(playerid, -1, "Esta empresa ja atingiu o limite de pickups genericos.");
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new index = AddCompanyGenericPickup(companyid, x, y, z, GetPlayerInterior(playerid), modelo);

    new msg[96];
    format(msg, sizeof(msg), "Pickup generico #%d adicionado na sua posicao atual.", index);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

flags:empresaremoverpickupgenerico(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaremoverpickupgenerico(playerid, params[])
{
    new sqlId, companyid, index;

    if (sscanf(params, "dd", sqlId, index) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaremoverpickupgenerico [id] [indice]");
        return 1;
    }

    if (!RemoveCompanyGenericPickup(companyid, index)) {
        SendClientMessage(playerid, -1, "Indice de pickup generico invalido.");
        return 1;
    }

    SendClientMessage(playerid, -1, "Pickup generico removido.");
    return 1;
}

flags:empresalistarpickups(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresalistarpickups(playerid, params[])
{
    new sqlId, companyid;

    if (sscanf(params, "d", sqlId) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresalistarpickups [id]");
        return 1;
    }

    new msg[144];

    format(msg, sizeof(msg), "Checkpoint de vendas: %s", CompanyHasSalesCheckpoint(companyid) ? "definido" : "nao definido");
    SendClientMessage(playerid, -1, msg);

    new count = GetCompanyGenericPickupCount(companyid);

    format(msg, sizeof(msg), "Pickups genericos: %d/%d", count, MAX_COMPANY_GENERIC_PICKUPS);
    SendClientMessage(playerid, -1, msg);

    for (new i = 0; i < count; i++) {
        format(msg, sizeof(msg), "  #%d", i);
        SendClientMessage(playerid, -1, msg);
    }
    return 1;
}

flags:empresasetdono(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresasetdono(playerid, params[])
{
    new sqlId, companyid, targetid;

    if (sscanf(params, "dd", sqlId, targetid) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresasetdono [id da empresa] [id do jogador]");
        return 1;
    }

    if (!IsPlayerConnected(targetid)) {
        SendClientMessage(playerid, -1, "Jogador invalido.");
        return 1;
    }

    SetCompanyOwner(companyid, Company_GetPlayerOwnerID(targetid));

    new companyName[32], targetName[MAX_PLAYER_NAME];
    GetCompanyName(companyid, companyName);
    GetPlayerName(targetid, targetName, sizeof(targetName));

    new msg[128];
    format(msg, sizeof(msg), "%s agora e o dono da empresa '%s'.", targetName, companyName);
    SendClientMessage(playerid, -1, msg);
    SendClientMessage(targetid, -1, msg);
    return 1;
}

flags:empresaremoverdono(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:empresaremoverdono(playerid, params[])
{
    new sqlId, companyid;

    if (sscanf(params, "d", sqlId) || (companyid = GetCompanyIndexFromSQLID(sqlId)) == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Uso: /empresaremoverdono [id]");
        return 1;
    }

    RemoveCompanyOwner(companyid);
    SendClientMessage(playerid, -1, "A empresa nao possui mais dono.");
    return 1;
}

flags:trancarempresa(CMD_LOGGED)
CMD:trancarempresa(playerid, params[])
{
    new companyid = GetCompanyPlayerIsAt(playerid);

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Voce precisa estar na entrada ou saida de uma empresa para usar este comando.");
        return 1;
    }

    if (!Company_IsPlayerOwner(playerid, companyid)) {
        SendClientMessage(playerid, -1, "Voce nao e o dono desta empresa.");
        return 1;
    }

    if (IsCompanyLocked(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa ja esta trancada.");
        return 1;
    }

    SetCompanyLocked(companyid, true);
    SendClientMessage(playerid, -1, "Voce trancou a empresa.");
    return 1;
}

flags:destrancar(CMD_LOGGED)
CMD:destrancar(playerid, params[])
{
    new companyid = GetCompanyPlayerIsAt(playerid);

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Voce precisa estar na entrada ou saida de uma empresa para usar este comando.");
        return 1;
    }

    if (!Company_IsPlayerOwner(playerid, companyid)) {
        SendClientMessage(playerid, -1, "Voce nao e o dono desta empresa.");
        return 1;
    }

    if (!IsCompanyLocked(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa ja esta destrancada.");
        return 1;
    }

    SetCompanyLocked(companyid, false);
    SendClientMessage(playerid, -1, "Voce destrancou a empresa.");
    return 1;
}

static s_BreakInCompany[MAX_PLAYERS];
static s_BreakInElapsed[MAX_PLAYERS];
static s_BreakInTimer[MAX_PLAYERS];

flags:arrombar(CMD_LOGGED)
CMD:arrombar(playerid, params[])
{
    if (s_BreakInTimer[playerid] != 0) {
        SendClientMessage(playerid, -1, "Voce ja esta tentando arrombar uma empresa.");
        return 1;
    }

    new companyid = GetCompanyPlayerIsAt(playerid);

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Voce precisa estar na entrada ou saida de uma empresa para usar este comando.");
        return 1;
    }

    if (!IsCompanyLocked(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao esta trancada.");
        return 1;
    }

    if (Company_IsPlayerOwner(playerid, companyid)) {
        SendClientMessage(playerid, -1, "Voce e o dono desta empresa, use /destrancar.");
        return 1;
    }

    s_BreakInCompany[playerid] = companyid;
    s_BreakInElapsed[playerid] = 0;
    s_BreakInTimer[playerid] = SetTimerEx("Company_BreakInTick", COMPANY_BREAKIN_TICK, true, "i", playerid);

    SendClientMessage(playerid, -1, "Voce comecou a arrombar a fechadura. Fique parado perto da entrada/saida ate terminar.");
    return 1;
}

forward Company_BreakInTick(playerid);
public Company_BreakInTick(playerid)
{
    new companyid = s_BreakInCompany[playerid];

    if (!IsPlayerConnected(playerid) || !IsValidCompany(companyid) || !IsPlayerAtCompanyPickup(playerid, companyid)) {
        Company_CancelBreakIn(playerid);
        SendClientMessage(playerid, -1, "Voce se afastou e perdeu o progresso do arrombamento.");
        return 1;
    }

    if (!IsCompanyLocked(companyid)) {
        Company_CancelBreakIn(playerid);
        SendClientMessage(playerid, -1, "A empresa foi destrancada, arrombamento cancelado.");
        return 1;
    }

    s_BreakInElapsed[playerid] += COMPANY_BREAKIN_TICK;

    if (s_BreakInElapsed[playerid] >= COMPANY_BREAKIN_TIME) {
        Company_CancelBreakIn(playerid);

        SetCompanyLocked(companyid, false);
        SendClientMessage(playerid, -1, "Voce arrombou a fechadura! A empresa esta destrancada.");
        return 1;
    }

    new msg[64];
    format(msg, sizeof(msg), "Arrombando... %d%%", (s_BreakInElapsed[playerid] * 100) / COMPANY_BREAKIN_TIME);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

static stock Company_CancelBreakIn(playerid)
{
    if (s_BreakInTimer[playerid] != 0) {
        KillTimer(s_BreakInTimer[playerid]);
    }

    s_BreakInTimer[playerid] = 0;
    s_BreakInCompany[playerid] = COMPANY_INVALID_ID;
    s_BreakInElapsed[playerid] = 0;
    return 1;
}

stock Company_HandleDisconnect(playerid)
{
    Company_CancelBreakIn(playerid);
    return 1;
}

flags:caixa(CMD_LOGGED)
CMD:caixa(playerid, params[])
{
    new companyid = GetCompanySalesCheckpointPlayerIsAt(playerid);

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Voce precisa estar no checkpoint de vendas de uma empresa para usar o caixa.");
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

flags:sacarcaixa(CMD_LOGGED)
CMD:sacarcaixa(playerid, params[])
{
    new companyid = GetCompanyPlayerIsAt(playerid);

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Voce precisa estar na entrada ou saida da sua empresa para sacar o caixa.");
        return 1;
    }

    if (!Company_IsPlayerOwner(playerid, companyid)) {
        SendClientMessage(playerid, -1, "Voce nao e o dono desta empresa.");
        return 1;
    }

    new amount;

    if (sscanf(params, "d", amount) || amount <= 0) {
        new msg[64];
        format(msg, sizeof(msg), "Uso: /sacarcaixa [valor] (caixa atual: $%d)", GetCompanyCash(companyid));
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    if (!RemoveCompanyCash(companyid, amount)) {
        SendClientMessage(playerid, -1, "O caixa da empresa nao tem saldo suficiente.");
        return 1;
    }

    GivePlayerMoney(playerid, amount);

    new msg[64];
    format(msg, sizeof(msg), "Voce sacou $%d do caixa da empresa.", amount);
    SendClientMessage(playerid, -1, msg);
    return 1;
}
