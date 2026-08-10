#if defined _INC_company_dialogs
    #endinput
#endif
#define _INC_company_dialogs

static enum e_COMPANY_CREATION
{
    E_COMPANY_NAME[32],
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

flags:criarempresa(CMD_LOGGED | CMD_ADMIN | CMD_MODER | CMD_JR_MODER)
CMD:criarempresa(playerid, params[])
{
    if (IsCompanyLimitReached()) {
        SendClientMessage(playerid, -1, "O limite maximo de empresas ja foi atingido.");
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_ACTIVE] = true;
    s_CompanyCreation[playerid][E_COMPANY_NAME][0] = '\0';

    Dialog_Show(playerid, CompanyCreateName, DIALOG_STYLE_INPUT,
        "Criar empresa - Nome",
        "Digite o nome da nova empresa:",
        "Proximo", "Cancelar");
    return 1;
}

Dialog:CompanyCreateName(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    if (isnull(inputtext) || strlen(inputtext) >= 32) {
        SendClientMessage(playerid, -1, "Nome invalido. Digite um nome de ate 31 caracteres.");
        Dialog_Show(playerid, CompanyCreateName, DIALOG_STYLE_INPUT,
            "Criar empresa - Nome",
            "Digite o nome da nova empresa:",
            "Proximo", "Cancelar"
        );
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_NAME][0] = '\0';
    strcat(s_CompanyCreation[playerid][E_COMPANY_NAME], inputtext);

    Dialog_Show(playerid, CompanyCreateOpen, DIALOG_STYLE_INPUT,
        "Criar empresa - Abertura",
        "Digite o horario de abertura (0 a 23):",
        "Proximo", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateOpen(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new open;

    if (isnull(inputtext) || sscanf(inputtext, "i", open) || !(0 <= open <= 23)) {
        SendClientMessage(playerid, -1, "Horario invalido. Digite um numero de 0 a 23.");
        Dialog_Show(playerid, CompanyCreateOpen, DIALOG_STYLE_INPUT,
            "Criar empresa - Abertura",
            "Digite o horario de abertura (0 a 23):",
            "Proximo", "Cancelar");
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_OPEN] = open;

    Dialog_Show(playerid, CompanyCreateClose, DIALOG_STYLE_INPUT,
        "Criar empresa - Fechamento",
        "Digite o horario de fechamento (0 a 23):",
        "Proximo", "Cancelar");
    return 1;
}

Dialog:CompanyCreateClose(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new close;

    if (isnull(inputtext) || sscanf(inputtext, "i", close) || !(0 <= close <= 23)) {
        SendClientMessage(playerid, -1, "Horario invalido. Digite um numero de 0 a 23.");
        Dialog_Show(playerid, CompanyCreateClose, DIALOG_STYLE_INPUT,
            "Criar empresa - Fechamento",
            "Digite o horario de fechamento (0 a 23):",
            "Proximo", "Cancelar"
        );
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_CLOSE] = close;

    Dialog_Show(playerid, CompanyCreateEnter, DIALOG_STYLE_MSGBOX,
        "Criar empresa - Entrada",
        "Va ate o local desejado para a ENTRADA da empresa e confirme.\n\
        Sua posicao e interior atuais serao utilizados.",
        "Confirmar", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateEnter(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new Float:x;
    new Float:y;
    new Float:z;
    GetPlayerPos(playerid, x, y, z);

    s_CompanyCreation[playerid][E_COMPANY_ENTER_X] = x;
    s_CompanyCreation[playerid][E_COMPANY_ENTER_Y] = y;
    s_CompanyCreation[playerid][E_COMPANY_ENTER_Z] = z;
    s_CompanyCreation[playerid][E_COMPANY_ENTER_INTERIOR] = GetPlayerInterior(playerid);

    Dialog_Show(playerid, CompanyCreateExit, DIALOG_STYLE_MSGBOX,
        "Criar empresa - Saida",
        "Va ate o local desejado para a SAIDA da empresa e confirme.\n\
        Sua posicao e interior atuais serao utilizados.",
        "Confirmar", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateExit(playerid, response, listitem, inputtext[])
{
    if (!response) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    if (!s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    s_CompanyCreation[playerid][E_COMPANY_EXIT_X] = x;
    s_CompanyCreation[playerid][E_COMPANY_EXIT_Y] = y;
    s_CompanyCreation[playerid][E_COMPANY_EXIT_Z] = z;
    s_CompanyCreation[playerid][E_COMPANY_EXIT_INTERIOR] = GetPlayerInterior(playerid);

    new summary[400];
    format(summary, sizeof(summary),
        "Nome: %s\n\
        Abertura: %02d:00\n\
        Fechamento: %02d:00\n\n\
        Entrada:\n\
        %.2f, %.2f, %.2f (Interior %d)\n\n\
        Saida:\n\
        %.2f, %.2f, %.2f (Interior %d)\n\n\
        Deseja criar esta empresa?",
        s_CompanyCreation[playerid][E_COMPANY_NAME],
        s_CompanyCreation[playerid][E_COMPANY_OPEN],
        s_CompanyCreation[playerid][E_COMPANY_CLOSE],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_X],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_Y],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_Z],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_INTERIOR],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_X],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_Y],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_Z],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_INTERIOR]
    );

    Dialog_Show(playerid, CompanyCreateConfirm, DIALOG_STYLE_MSGBOX,
        "Criar empresa - Confirmacao",
        summary,
        "Criar", "Cancelar"
    );
    return 1;
}

Dialog:CompanyCreateConfirm(playerid, response, listitem, inputtext[])
{
    if (!response || !s_CompanyCreation[playerid][E_COMPANY_ACTIVE]) {
        ResetCompanyCreation(playerid);
        SendClientMessage(playerid, -1, "Criacao de empresa cancelada.");
        return 1;
    }

    s_CompanyCreation[playerid][E_COMPANY_ACTIVE] = false;

    CreateCompanyInDB(
        playerid,
        s_CompanyCreation[playerid][E_COMPANY_NAME],
        s_CompanyCreation[playerid][E_COMPANY_OPEN],
        s_CompanyCreation[playerid][E_COMPANY_CLOSE],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_X],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_Y],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_Z],
        s_CompanyCreation[playerid][E_COMPANY_ENTER_INTERIOR],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_X],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_Y],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_Z],
        s_CompanyCreation[playerid][E_COMPANY_EXIT_INTERIOR]
    );
    return 1;
}

// Chamado por company_core.inc quando o INSERT termina (sucesso ou falha).
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

    new msg[96];
    format(msg, sizeof(msg), "Empresa '%s' criada com sucesso (ID %d).", name, GetCompanySQLID(companySlot));
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
        SendClientMessage(playerid, -1, "Empresa invalida.");
        return 1;
    }

    s_EditingCompany[playerid] = companyid;

    new name[32];
    GetCompanyName(companyid, name);

    new open, close;
    GetCompanyBusinessHour(companyid, open, close);

    new menu[256];
    format(menu, sizeof(menu),
        "Nome (%s)\n\
        Horario de abertura (%02d:00)\n\
        Horario de fechamento (%02d:00)\n\
        Redefinir entrada\n\
        Redefinir saida\n\
        %s\n\
        Excluir empresa",
        name, open, close,
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
        case 0: {
            Dialog_Show(playerid, CompanyEditName, DIALOG_STYLE_INPUT,
                "Editar empresa - Nome",
                "Digite o novo nome da empresa:",
                "Salvar", "Cancelar");
        } case 1: {
            Dialog_Show(playerid, CompanyEditOpen, DIALOG_STYLE_INPUT,
                "Editar empresa - Abertura",
                "Digite o novo horario de abertura (0 a 23):",
                "Salvar", "Cancelar");
        } case 2: {
            Dialog_Show(playerid, CompanyEditClose, DIALOG_STYLE_INPUT,
                "Editar empresa - Fechamento",
                "Digite o novo horario de fechamento (0 a 23):",
                "Salvar", "Cancelar");
        } case 3: {
            Dialog_Show(playerid, CompanyEditEnter, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Entrada",
                "Va ate o novo local de ENTRADA e confirme.\n\
                Sua posicao e interior atuais serao utilizados.",
                "Confirmar", "Cancelar");
        } case 4: {
            Dialog_Show(playerid, CompanyEditExit, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Saida",
                "Va ate o novo local de SAIDA e confirme.\n\
                Sua posicao e interior atuais serao utilizados.",
                "Confirmar", "Cancelar");
        } case 5: {
            SetCompanyLocked(companyid, !IsCompanyLocked(companyid));

            SendClientMessage(playerid, -1, IsCompanyLocked(companyid)
                ? "Empresa trancada."
                : "Empresa destrancada.");
        } case 6: {
            Dialog_Show(playerid, CompanyEditDelete, DIALOG_STYLE_MSGBOX,
                "Editar empresa - Excluir",
                "Tem certeza que deseja excluir esta empresa?\nEsta acao nao pode ser desfeita.",
                "Excluir", "Cancelar");
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
        SendClientMessage(playerid, -1, "Horario invalido.");
        return 1;
    }

    GetCompanyBusinessHour(companyid, open, close);
    SetCompanyBusinessHour(companyid, newOpen, close);
    SendClientMessage(playerid, -1, "Horario de abertura atualizado.");
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
        SendClientMessage(playerid, -1, "Horario invalido.");
        return 1;
    }

    GetCompanyBusinessHour(companyid, open, close);
    SetCompanyBusinessHour(companyid, open, newClose);
    SendClientMessage(playerid, -1, "Horario de fechamento atualizado.");
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
    SendClientMessage(playerid, -1, "Saida da empresa atualizada.");
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
    SendClientMessage(playerid, -1, "Empresa excluida.");
    return 1;
}
