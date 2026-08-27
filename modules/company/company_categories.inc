#if defined _INC_company_categories
    #endinput
#endif
#define _INC_company_categories

enum e_COMPANY_CATEGORY
{
    E_COMPANY_CAT_GENERICA = 0,
    E_COMPANY_CAT_ROUPAS,
    E_COMPANY_CAT_24HORAS,
    E_COMPANY_CAT_RESTAURANTE,
    E_COMPANY_CAT_ARMARIA,

    E_COMPANY_CAT_MAX
};

static const g_CompanyCategoryPickupModel[E_COMPANY_CAT_MAX] =
{
    19135, // E_COMPANY_CAT_GENERICA   - sacola (generico)
    19135, // E_COMPANY_CAT_ROUPAS     - sacola de compras
    19194, // E_COMPANY_CAT_24HORAS    - icone estilo loja de conveniencia
    19222, // E_COMPANY_CAT_RESTAURANTE- icone estilo comida
    1247   // E_COMPANY_CAT_ARMARIA    - icone estilo armaria
};

stock GetCompanyCategoryName(category, name[], size = sizeof(name))
{
    switch (category) {
        case E_COMPANY_CAT_GENERICA:    format(name, size, "Generica");
        case E_COMPANY_CAT_ROUPAS:      format(name, size, "Loja de Roupas");
        case E_COMPANY_CAT_24HORAS:     format(name, size, "24 Horas");
        case E_COMPANY_CAT_RESTAURANTE: format(name, size, "Restaurante");
        case E_COMPANY_CAT_ARMARIA:     format(name, size, "Armaria");
        default:                      format(name, size, "Desconhecida");
    }
    return 1;
}

stock GetCompanyCategoryPickupModel(category)
{
    if (!(0 <= category < E_COMPANY_CAT_MAX)) {
        return 0;
    }

    return g_CompanyCategoryPickupModel[category];
}

/* ------------------------------------------------------------------
 * Itens pre-configurados por categoria (nome, preco, "efeito").
 *
 * Para E_COMPANY_CAT_ROUPAS o "efeito" e o ID de skin aplicado ao
 * comprar (exemplo simples). Para as outras categorias o "efeito" e
 * ignorado por enquanto - so debita o dinheiro do jogador e credita
 * no caixa da empresa (Company_Purchase cuida disso, ver abaixo).
 * ------------------------------------------------------------------ */

#define COMPANY_MAX_ITEMS_PER_CATEGORY (8)

enum e_COMPANY_ITEM
{
    E_ITEM_NAME[32],
    E_ITEM_PRICE,
    E_ITEM_EFFECT /* ex.: skin id, para roupas */
};

static const g_CompanyItemsGenerica[][e_COMPANY_ITEM] =
{
    {"Nao ha itens configurados", 0, -1}
};

static const g_CompanyItemsRoupas[][e_COMPANY_ITEM] =
{
    {"Camisa Casual",      150, 59},
    {"Terno Executivo",    600, 165},
    {"Roupa Esportiva",    250, 68},
    {"Jaqueta de Couro",   400, 73},
    {"Roupa de Praia",     120, 108}
};

static const g_CompanyItemsRestaurante[][e_COMPANY_ITEM] =
{
    {"Hamburguer",   15, -1},
    {"Refrigerante",  8, -1},
    {"Pizza",        25, -1},
    {"Combo Completo",40, -1}
};

static const g_CompanyItems247[][e_COMPANY_ITEM] =
{
    {"Agua",           5, -1},
    {"Lanche",        12, -1},
    {"Cigarros",       20, -1},
    {"Jornal",          3, -1},
    {"Kit de Primeiros Socorros", 35, -1}
};

static const g_CompanyItemsArmaria[][e_COMPANY_ITEM] =
{
    {"Colete Balistico", 500, -1}
};

stock Company_GetCategoryItemCount(category)
{
    switch (category) {
        case E_COMPANY_CAT_ROUPAS:      return sizeof(g_CompanyItemsRoupas);
        case E_COMPANY_CAT_RESTAURANTE: return sizeof(g_CompanyItemsRestaurante);
        case E_COMPANY_CAT_24HORAS:     return sizeof(g_CompanyItems247);
        case E_COMPANY_CAT_ARMARIA:     return sizeof(g_CompanyItemsArmaria);
        default:                        return sizeof(g_CompanyItemsGenerica);
    }
}

stock Company_GetCategoryItemName(category, index, name[], size = sizeof(name))
{
    switch (category) {
        case E_COMPANY_CAT_ROUPAS: {
            if (!(0 <= index < sizeof(g_CompanyItemsRoupas))) return 0;
            format(name, size, "%s", g_CompanyItemsRoupas[index][E_ITEM_NAME]);
        }
        case E_COMPANY_CAT_RESTAURANTE: {
            if (!(0 <= index < sizeof(g_CompanyItemsRestaurante))) return 0;
            format(name, size, "%s", g_CompanyItemsRestaurante[index][E_ITEM_NAME]);
        }
        case E_COMPANY_CAT_24HORAS: {
            if (!(0 <= index < sizeof(g_CompanyItems247))) return 0;
            format(name, size, "%s", g_CompanyItems247[index][E_ITEM_NAME]);
        }
        case E_COMPANY_CAT_ARMARIA: {
            if (!(0 <= index < sizeof(g_CompanyItemsArmaria))) return 0;
            format(name, size, "%s", g_CompanyItemsArmaria[index][E_ITEM_NAME]);
        }
        default: {
            if (!(0 <= index < sizeof(g_CompanyItemsGenerica))) return 0;
            format(name, size, "%s", g_CompanyItemsGenerica[index][E_ITEM_NAME]);
        }
    }
    return 1;
}

stock Company_GetCategoryItemPrice(category, index)
{
    switch (category) {
        case E_COMPANY_CAT_ROUPAS: {
            if (!(0 <= index < sizeof(g_CompanyItemsRoupas))) return 0;
            return g_CompanyItemsRoupas[index][E_ITEM_PRICE];
        }
        case E_COMPANY_CAT_RESTAURANTE: {
            if (!(0 <= index < sizeof(g_CompanyItemsRestaurante))) return 0;
            return g_CompanyItemsRestaurante[index][E_ITEM_PRICE];
        }
        case E_COMPANY_CAT_24HORAS: {
            if (!(0 <= index < sizeof(g_CompanyItems247))) return 0;
            return g_CompanyItems247[index][E_ITEM_PRICE];
        }
        case E_COMPANY_CAT_ARMARIA: {
            if (!(0 <= index < sizeof(g_CompanyItemsArmaria))) return 0;
            return g_CompanyItemsArmaria[index][E_ITEM_PRICE];
        }
        default: return 0;
    }
}

stock Company_GetCategoryItemEffect(category, index)
{
    switch (category) {
        case E_COMPANY_CAT_ROUPAS: {
            if (!(0 <= index < sizeof(g_CompanyItemsRoupas))) return -1;
            return g_CompanyItemsRoupas[index][E_ITEM_EFFECT];
        }
        default: return -1;
    }
}

forward OnCompanyPurchase(playerid, companyid, const itemName[], price);

stock Company_ShowPurchaseDialog(playerid, companyid)
{
    if (!IsValidCompany(companyid)) {
        return 0;
    }

    new category = GetCompanyCategory(companyid);
    new count = Company_GetCategoryItemCount(category);

    new list[512];
    list[0] = '\0';

    new itemName[32];

    for (new i = 0; i < count; i++) {
        Company_GetCategoryItemName(category, i, itemName);
        format(list, sizeof(list), "%s%s - $%d\n", list, itemName, Company_GetCategoryItemPrice(category, i));
    }

    Dialog_Show(playerid, CompanyPurchaseMenu, DIALOG_STYLE_LIST,
        "Caixa - Selecione um item",
        list,
        "Comprar", "Cancelar"
    );

    /* guarda a empresa selecionada para o momento da resposta do dialog */
    Company_SetPurchaseContext(playerid, companyid);
    return 1;
}

static s_PurchaseCompany[MAX_PLAYERS];

static stock Company_SetPurchaseContext(playerid, companyid)
{
    s_PurchaseCompany[playerid] = companyid;
    return 1;
}

Dialog:CompanyPurchaseMenu(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_PurchaseCompany[playerid];

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao existe mais.");
        return 1;
    }

    if (!IsCompanyAvailable(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao esta disponivel para compras no momento.");
        return 1;
    }

    new category = GetCompanyCategory(companyid);

    if (!(0 <= listitem < Company_GetCategoryItemCount(category))) {
        return 1;
    }

    new itemName[32];
    Company_GetCategoryItemName(category, listitem, itemName);

    new price = Company_GetCategoryItemPrice(category, listitem);

    if (price <= 0) {
        SendClientMessage(playerid, -1, "Este item não está disponivel para compra.");
        return 1;
    }

    if (GetPlayerMoney(playerid) < price) {
        SendClientMessage(playerid, -1, "Você não tem dinheiro suficiente.");
        return 1;
    }

    GivePlayerMoney(playerid, -price);
    AddCompanyCash(companyid, price);

    new effect = Company_GetCategoryItemEffect(category, listitem);
    if (category == _:E_COMPANY_CAT_ROUPAS && effect > 0) {
        SetPlayerSkin(playerid, effect);
    }

    new msg[96];
    format(msg, sizeof(msg), "Voce comprou '%s' por $%d.", itemName, price);
    SendClientMessage(playerid, -1, msg);

    Company_LogPurchase(companyid, playerid, itemName, price);

    CallLocalFunction("OnCompanyPurchase", "iisi", playerid, companyid, itemName, price);
    return 1;
}

static stock Company_LogPurchase(companyid, buyerid, const itemName[], price)
{
    new sqlId = GetCompanySQLID(companyid);

    if (sqlId <= 0) {
        return 0;
    }

    new itemEscaped[65];
    mysql_escape_string(itemName, itemEscaped);

    new query[256];
    format(query, sizeof(query),
        "INSERT INTO `company_purchases` (`company_id`, `buyer_id`, `item_name`, `price`) VALUES (%d, %d, '%s', %d)",
        sqlId, buyerid, itemEscaped, price
    );

    mysql_tquery(GetDatabaseHandle(), query);
    return 1;
}
