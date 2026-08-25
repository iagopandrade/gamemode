#if defined _INC_company_dialogs
    #endinput
#endif
#define _INC_company_dialogs

static enum e_COMPANY_CREATION
{
    E_COMPANY_NAME[32],
    E_COMPANY_OPEN,
    E_COMPANY_CLOSE,
    E_COMPANY_CATEGORY,

    bool:E_COMPANY_ACTIVE
};

static s_CompanyCreation[MAX_PLAYERS][e_COMPANY_CREATION];

static s_EditingCompany[MAX_PLAYERS];

static stock ResetCompanyCreation(playerid)
{
    s_CompanyCreation[playerid][E_COMPANY_ACTIVE] = false;
    s_CompanyCreation[playerid][E_COMPANY_NAME][0] = '\0';
    return 1;
}

static enum {
    ENUMATOR_NAME = 0,
    ENUMATOR_OPEN,
    ENUMATOR_CLOSE,
    ENUMATOR_ENTRANCE,
    ENUMATOR_EXIT,
    ENUMATOR_SALES,
    ENUMATOR_MAPICON,
    ENUMATOR_CATEGORY,
    ENUMATOR_OWNER,
    ENUMATOR_LOCKED,
    ENUMATOR_DELETE
}

flags:criarempresa(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:criarempresa(playerid, params[])
{
    if (IsCompanyLimitReached()) {
        SendClientMessage(playerid, -1, "O limite maximo de empresas ja foi atingido.");
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_ACTIVE] = true;
    s_CompanyCreation[playerid][E_COMPANY_NAME][0] = '\0';
    s_CompanyCreation[playerid][E_COMPANY_CATEGORY] = E_COMPANY_CAT_GENERICA;

    Dialog_Show(playerid, CompanyCreateName, DIALOG_STYLE_INPUT,
        "Criar empresa - Nome",
        "Digite o nome da nova empresa:",
        "Próximo", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateName(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criação de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    if (isnull(inputtext) || strlen(inputtext) >= 32) {
        SendClientMessage(playerid, -1, "Nome invalido. Digite um nome de até 31 caracteres.");
        Dialog_Show(playerid, CompanyCreateName, DIALOG_STYLE_INPUT,
            "Criar empresa - Nome",
            "Digite o nome da nova empresa:",
            "Próximo", "Cancelar"
        );
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_NAME][0] = '\0';
    strcat(s_CompanyCreation[playerid][E_COMPANY_NAME], inputtext);

    Dialog_Show(playerid, CompanyCreateOpen, DIALOG_STYLE_INPUT,
        "Criar empresa - Abertura",
        "Digite o horário de abertura (0 a 23):",
        "Próximo", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateOpen(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criação de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new open;

    if (isnull(inputtext) || sscanf(inputtext, "i", open) || !(0 <= open <= 23)) {
        SendClientMessage(playerid, -1, "Horário inválido. Digite um número de 0 a 23.");
        Dialog_Show(playerid, CompanyCreateOpen, DIALOG_STYLE_INPUT,
            "Criar empresa - Abertura",
            "Digite o horário de abertura (0 a 23):",
            "Próximo", "Cancelar"
        );
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_OPEN] = open;

    Dialog_Show(playerid, CompanyCreateClose, DIALOG_STYLE_INPUT,
        "Criar empresa - Fechamento",
        "Digite o horário de fechamento (0 a 23):",
        "Próximo", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateClose(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criação de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new close;

    if (isnull(inputtext) || sscanf(inputtext, "i", close) || !(0 <= close <= 23)) {
        SendClientMessage(playerid, -1, "Horário invalido. Digite um número de 0 a 23.");
        Dialog_Show(playerid, CompanyCreateClose, DIALOG_STYLE_INPUT,
            "Criar empresa - Fechamento",
            "Digite o horário de fechamento (0 a 23):",
            "Próximo", "Cancelar"
        );
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_CLOSE] = close;

    Dialog_Show(playerid, CompanyCreateCategory, DIALOG_STYLE_LIST,
        "Criar empresa - Categoria",
        "Generica\nLoja de Roupas\n24 Horas\nRestaurante\nArmaria",
        "Próximo", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateCategory(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criação de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    if (!(0 <= listitem < _:E_COMPANY_CAT_MAX)) {
        listitem = E_COMPANY_CAT_GENERICA;
    }

    s_CompanyCreation[playerid][E_COMPANY_CATEGORY] = listitem;

    new categoryName[24];
    GetCompanyCategoryName(listitem, categoryName);

    new summary[400];
    format(summary, sizeof(summary),
        "Nome: %s\n\
        Abertura: %02d:00\n\
        Fechamento: %02d:00\n\
        Categoria: %s\n\n\
        A empresa sera criada com a ENTRADA e a SAIDA na sua posicao\n\
        atual. Use /empresasetentrada e /empresasetsaida (ou o menu\n\
        de /editarempresa) para ajustar depois.\n\n\
        Deseja criar esta empresa?",
        s_CompanyCreation[playerid][E_COMPANY_NAME],
        s_CompanyCreation[playerid][E_COMPANY_OPEN],
        s_CompanyCreation[playerid][E_COMPANY_CLOSE],
        categoryName
    );

    Dialog_Show(playerid, CompanyCreateConfirm, DIALOG_STYLE_MSGBOX,
        "Criar empresa - Confirmação",
        summary,
        "Criar", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateConfirm(playerid, response, listitem, inputtext[])
{
    if (!response || !s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criação de empresa cancelada.");
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_ACTIVE] = false;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new interior = GetPlayerInterior(playerid);

    CreateCompanyInDB(
        playerid,
        s_CompanyCreation[playerid][E_COMPANY_NAME],
        s_CompanyCreation[playerid][E_COMPANY_OPEN],
        s_CompanyCreation[playerid][E_COMPANY_CLOSE],
        x, y, z, interior,
        x, y, z, interior,
        s_CompanyCreation[playerid][E_COMPANY_CATEGORY]
    );
    return 1;
}

public OnCompanyInserted(playerid, companySlot)
{
    if (!(0 <= playerid < MAX_PLAYERS)) {
        return 1;
    }

    if (companySlot == COMPANY_INVALID_ID) {
        SendClientMessage(playerid, -1, "Falha ao criar a empresa. Verifique o console para mais detalhes.");
        return 1;
    }

    new name[32];
    GetCompanyName(companySlot, name);

    new msg[160];
    format(msg, sizeof(msg),
        "Empresa '%s' criada com sucesso (ID %d). Use /empresasetentrada %d e /empresasetsaida %d para ajustar a entrada e a saida.",
        name, GetCompanySQLID(companySlot), GetCompanySQLID(companySlot), GetCompanySQLID(companySlot)
    );
    SendClientMessage(playerid, -1, msg);
    return 1;
}

flags:editarempresa(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:editarempresa(playerid, params[])
{
    if (Iter_Count(Companies) == 0) {
        SendClientMessage(playerid, -1, "Nao ha nenhuma empresa carregada no momento.");
        return 1;
    }

    new list[1200];
    new name[32];

    foreach (new i : Companies) {
        GetCompanyName(i, name);
        format(list, sizeof(list), "%s%s (ID %d)\n", list, name, GetCompanySQLID(i));
    }

    Dialog_Show(playerid, CompanyEditList, DIALOG_STYLE_LIST,
        "Editar empresa - Selecione",
        list,
        "Selecionar", "Cancelar"
    );
    return 1;
}

Dialog:CompanyEditList(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = COMPANY_INVALID_ID, current;

    /* o "listitem" do easyDialog corresponde a ordem de insercao no foreach acima */
    foreach (new i : Companies) {
        if (current == listitem) {
            companyid = i;
            break;
        }
        current++;
    }

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Empresa inválida.");
        return 1;
    }

    s_EditingCompany[playerid] = companyid;

    new name[32];
    GetCompanyName(companyid, name);

    new open, close;
    GetCompanyBusinessHour(companyid, open, close);

    new categoryName[24];
    GetCompanyCategoryName(GetCompanyCategory(companyid), categoryName);

    new ownerLine[48];
    if (CompanyHasOwner(companyid)) {
        format(ownerLine, sizeof(ownerLine), "Dono (definido) - remover dono");
    } else {
        format(ownerLine, sizeof(ownerLine), "Dono (sem dono) - definir dono");
    }

    new menu[440];
    format(menu, sizeof(menu),
        "Nome (%s)\n\
        Horário de abertura (%02d:00)\n\
        Horário de fechamento (%02d:00)\n\
        Redefinir entrada\n\
        Redefinir saída\n\
        Checkpoint de vendas (%s)\n\
        Ícone\n\
        Categoria (%s)\n\
        %s\n\
        %s\n\
        Excluir empresa",
        name,
        open,
        close,
        CompanyHasSalesCheckpoint(companyid) ? "definido" : "nao definido",
        categoryName,
        ownerLine,
        IsCompanyLocked(companyid) ? "Destrancar empresa" : "Trancar empresa"
    );

    Dialog_Show(playerid, CompanyEditMenu, DIALOG_STYLE_LIST,
        "Editar empresa - Opcoes",
        menu,
        "Selecionar", "Voltar"
    );
    return 1;
}

Dialog:CompanyEditMenu(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao existe mais.");
        return 1;
    }

    switch (listitem) {
        case ENUMATOR_NAME: {
            Dialog_Show(playerid, CompanyEditName, DIALOG_STYLE_INPUT,
                "Editar empresa - Nome",
                "Digite o novo nome da empresa:",
                "Salvar", "Cancelar"
            );
        } case ENUMATOR_OPEN: {
            Dialog_Show(playerid, CompanyEditOpen, DIALOG_STYLE_INPUT,
                "Editar empresa - Abertura",
                "Digite o novo horário de abertura (0 a 23):",
                "Salvar", "Cancelar"
            );
        } case ENUMATOR_CLOSE: {
            Dialog_Show(playerid, CompanyEditClose, DIALOG_STYLE_INPUT,
                "Editar empresa - Fechamento",
                "Digite o novo horário de fechamento (0 a 23):",
                "Salvar", "Cancelar"
            );
        } case ENUMATOR_ENTRANCE: {
            Dialog_Show(playerid, CompanyEditEnter, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Entrada",
                "Vá até o novo local de ENTRADA e confirme.\n\
                Sua posição e interior atuais serão utilizados.",
                "Confirmar", "Cancelar"
            );
        } case ENUMATOR_EXIT: {
            Dialog_Show(playerid, CompanyEditExit, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Saída",
                "Vá até o novo local de SAIDA e confirme.\n\
                Sua posição e interior atuais serão utilizados.",
                "Confirmar", "Cancelar"
            );
        } case ENUMATOR_SALES: {
            if (CompanyHasSalesCheckpoint(companyid)) {
                Dialog_Show(playerid, CompanyEditSalesMenu, DIALOG_STYLE_LIST,
                    "Editar empresa - Checkpoint de vendas",
                    "Editar (reposicionar)\n\
                    Remover",
                    "Selecionar", "Voltar"
                );
            } else {
                Dialog_Show(playerid, CompanyEditSalesMenu, DIALOG_STYLE_LIST,
                    "Editar empresa - Checkpoint de vendas",
                    "Adicionar",
                    "Selecionar", "Voltar"
                );
            }
        } case ENUMATOR_MAPICON: {
            Dialog_Show(playerid, CompanyEditIcon, DIALOG_STYLE_LIST,
                "Editar empresa - Icone",
                "Ativar/desativar\n\
                Alterar icone\n\
                Alterar cor",
                "Selecionar",
                "Voltar"
            );
        } case ENUMATOR_CATEGORY: {
            Dialog_Show(playerid, CompanyEditCategory, DIALOG_STYLE_LIST,
                "Editar empresa - Categoria",
                "Generica\nLoja de Roupas\n24 Horas\nRestaurante\nArmaria",
                "Selecionar",
                "Voltar"
            );
        } case ENUMATOR_OWNER: {
            if (CompanyHasOwner(companyid)) {
                RemoveCompanyOwner(companyid);
                SendClientMessage(playerid, -1, "A empresa nao possui mais dono.");
            } else {
                Dialog_Show(playerid, CompanyEditOwner, DIALOG_STYLE_INPUT,
                    "Editar empresa - Dono",
                    "Digite o ID (playerid) do jogador que sera o dono:",
                    "Definir",
                    "Voltar"
                );
            }
        } case ENUMATOR_LOCKED: {
            SetCompanyLocked(companyid, !IsCompanyLocked(companyid));

            SendClientMessage(playerid, -1, IsCompanyLocked(companyid)
                ? "Empresa trancada."
                : "Empresa destrancada."
            );
        } case ENUMATOR_DELETE: {
            Dialog_Show(playerid, CompanyEditDelete, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Excluir",
                "Tem certeza que deseja excluir esta empresa?\nEsta acao nao pode ser desfeita.",
                "Excluir", "Cancelar"
            );
        }
    }
    return 1;
}

Dialog:CompanyEditName(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    if (isnull(inputtext) || strlen(inputtext) >= 32) {
        SendClientMessage(playerid, -1, "Nome invalido.");
        return 1;
    }

    SetCompanyName(companyid, inputtext);
    SendClientMessage(playerid, -1, "Nome da empresa atualizado.");
    return 1;
}

Dialog:CompanyEditOpen(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new open, close, newOpen;

    if (isnull(inputtext) || sscanf(inputtext, "i", newOpen) || !(0 <= newOpen <= 23)) {
        SendClientMessage(playerid, -1, "Horário invalido.");
        return 1;
    }

    GetCompanyBusinessHour(companyid, open, close);
    SetCompanyBusinessHour(companyid, newOpen, close);
    SendClientMessage(playerid, -1, "Horário de abertura atualizado.");
    return 1;
}

Dialog:CompanyEditClose(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new open, close, newClose;

    if (isnull(inputtext) || sscanf(inputtext, "i", newClose) || !(0 <= newClose <= 23)) {
        SendClientMessage(playerid, -1, "Horário invalido.");
        return 1;
    }

    GetCompanyBusinessHour(companyid, open, close);
    SetCompanyBusinessHour(companyid, open, newClose);
    SendClientMessage(playerid, -1, "Horário de fechamento atualizado.");
    return 1;
}

Dialog:CompanyEditIcon(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    switch (listitem) {
        case 0: {
            SetCompanyMapIconEnabled(
                companyid,
                !s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED]
            );

            SendClientMessage(
                playerid,
                -1,
                s_CompanyData[companyid][E_COMPANY_MAP_ICON_ENABLED]
                    ? "Icone da empresa ativado."
                    : "Icone da empresa desativado."
            );
        } case 1: {
            Dialog_Show(playerid, CompanyEditIconType, DIALOG_STYLE_INPUT,
                "Editar empresa - Icone",
                "Digite o ID do icone do mapa:",
                "Salvar",
                "Voltar"
            );
        } case 2: {
            Dialog_Show(playerid, CompanyEditIconColor, DIALOG_STYLE_INPUT,
                "Editar empresa - Cor",
                "Digite a cor RGBA em hexadecimal.\n\
                Exemplo: FFFFFFFF",
                "Salvar",
                "Voltar"
            );
        }
    }

    return 1;
}

Dialog:CompanyEditIconType(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new icon;

    if (isnull(inputtext) || sscanf(inputtext, "i", icon)) {
        SendClientMessage(
            playerid,
            -1,
            "Digite um ID de icone valido."
        );
        return 1;
    }

    if (!(0 <= icon <= 63)) {
        SendClientMessage(
            playerid,
            -1,
            "O ID do icone deve estar entre 0 e 63."
        );
        return 1;
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON_TYPE] = icon;

    CreateCompanyMapIcon(companyid);
    SaveCompany(companyid);

    SendClientMessage(
        playerid,
        -1,
        "Icone da empresa atualizado."
    );

    return 1;
}

stock Dialog:CompanyEditIconColor(
    playerid,
    response,
    listitem,
    inputtext[]
)
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new color;

    if (isnull(inputtext)) {
        SendClientMessage(
            playerid,
            -1,
            "Digite uma cor."
        );
        return 1;
    }

    if (sscanf(inputtext, "h", color)) {
        SendClientMessage(
            playerid,
            -1,
            "Cor invalida. Exemplo: FFFFFFFF"
        );
        return 1;
    }

    s_CompanyData[companyid][E_COMPANY_MAP_ICON_COLOR] = color;

    CreateCompanyMapIcon(companyid);
    SaveCompany(companyid);

    SendClientMessage(
        playerid,
        -1,
        "Cor do icone atualizada."
    );

    return 1;
}

Dialog:CompanyEditEnter(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    SetCompanyEnterPos(companyid, x, y, z, GetPlayerInterior(playerid));
    SendClientMessage(playerid, -1, "Entrada da empresa atualizada.");
    return 1;
}

Dialog:CompanyEditExit(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    SetCompanyExitPos(companyid, x, y, z, GetPlayerInterior(playerid));
    SendClientMessage(playerid, -1, "Saída da empresa atualizada.");
    return 1;
}

Dialog:CompanyEditSalesMenu(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    if (CompanyHasSalesCheckpoint(companyid)) {
        switch (listitem) {
            case 0: {
                Dialog_Show(playerid, CompanyEditSalesEdit, DIALOG_STYLE_MSGBOX,
                    "Editar empresa - Editar checkpoint",
                    "Vá até o novo local do CHECKPOINT DE VENDAS e confirme.\n\
                    Sua posição e interior atuais serão utilizados.",
                    "Confirmar", "Cancelar"
                );
            } case 1: {
                Dialog_Show(playerid, CompanyEditSalesRemove, DIALOG_STYLE_MSGBOX,
                    "Editar empresa - Remover checkpoint",
                    "Tem certeza que deseja remover o checkpoint de vendas desta empresa?\n\
                    O /caixa deixará de funcionar até que um novo seja adicionado.",
                    "Remover", "Cancelar"
                );
            }
        }
    } else {
        Dialog_Show(playerid, CompanyEditSalesAdd, DIALOG_STYLE_MSGBOX,
            "Editar empresa - Adicionar checkpoint",
            "Vá até o local desejado para o CHECKPOINT DE VENDAS e confirme.\n\
            E ali que o /caixa fica disponivel. Sua posição e interior\n\
            atuais serão utilizados.",
            "Confirmar", "Cancelar"
        );
    }
    return 1;
}

Dialog:CompanyEditSalesAdd(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (!AddCompanySalesCheckpoint(companyid, x, y, z, GetPlayerInterior(playerid))) {
        SendClientMessage(playerid, -1, "Esta empresa ja possui um checkpoint de vendas.");
        return 1;
    }

    SendClientMessage(playerid, -1, "Checkpoint de vendas adicionado.");
    return 1;
}

Dialog:CompanyEditSalesEdit(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (!EditCompanySalesCheckpoint(companyid, x, y, z, GetPlayerInterior(playerid))) {
        SendClientMessage(playerid, -1, "Esta empresa ainda nao possui um checkpoint de vendas.");
        return 1;
    }

    SendClientMessage(playerid, -1, "Checkpoint de vendas atualizado.");
    return 1;
}

Dialog:CompanyEditSalesRemove(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    if (!RemoveCompanySalesCheckpoint(companyid)) {
        SendClientMessage(playerid, -1, "Esta empresa nao possui um checkpoint de vendas para remover.");
        return 1;
    }

    SendClientMessage(playerid, -1, "Checkpoint de vendas removido.");
    return 1;
}

Dialog:CompanyEditCategory(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    if (!(0 <= listitem < _:E_COMPANY_CAT_MAX)) {
        return 1;
    }

    SetCompanyCategory(companyid, listitem);

    new categoryName[24];
    GetCompanyCategoryName(listitem, categoryName);

    new msg[80];
    format(msg, sizeof(msg), "Categoria da empresa atualizada para: %s", categoryName);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

Dialog:CompanyEditOwner(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    new targetid;

    if (isnull(inputtext) || sscanf(inputtext, "d", targetid) || !IsPlayerConnected(targetid)) {
        SendClientMessage(playerid, -1, "ID de jogador invalido.");
        return 1;
    }

    SetCompanyOwner(companyid, Company_GetPlayerOwnerID(targetid));

    new targetName[MAX_PLAYER_NAME];
    GetPlayerName(targetid, targetName, sizeof(targetName));

    new msg[96];
    format(msg, sizeof(msg), "%s agora e o dono desta empresa.", targetName);
    SendClientMessage(playerid, -1, msg);
    SendClientMessage(targetid, -1, msg);
    return 1;
}

Dialog:CompanyEditDelete(playerid, response, listitem, inputtext[])
{
    if (!response) {
        return 1;
    }

    new companyid = s_EditingCompany[playerid];

    if (!IsValidCompany(companyid)) {
        return 1;
    }

    DeleteCompany(companyid);
    SendClientMessage(playerid, -1, "Empresa excluída.");
    return 1;
}
