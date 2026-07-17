BankInit()
{
    Company_Create(
        BANK_LOS_SANTOS,
        "Banco de Los Santos",
        COMPANY_TYPE_BANK
    );

    Company_AddMapIcon(
        BANK_LOS_SANTOS,
        52,
        1467.2704, -1011.1876, 26.8438
    );

    Company_AddPoint(
        PIZZERIA,
        COMPANY_POINT_ENTRANCE,
        1467.2704, -1011.1876, 26.8438, 129.3870,
        0.5,
        1239,
        1,
        0,
        0
    );

    Company_AddPoint(
        PIZZERIA,
        COMPANY_POINT_EXIT,
        2306.3826, -15.2365, 26.7496, 0.0,
        0.5,
        1239,
        1,
        5,
        0
    );

    return 1;
}

hook OnCompanyPoint@4(playerid, companyid, pointid)
{
    if (companyid != BANK_LOS_SANTOS) {
        return 1;
    }

    switch (Company_GetPointType(companyid, pointid))
    {
        case COMPANY_POINT_ENTRANCE:
        {
            Bank_ShowEntranceDialog(playerid);
        }

        case COMPANY_POINT_EXIT:
        {
            Company_Exit(playerid, companyid, 0.0, 1.5, 0.0);
        }
    }

    return 1;
}

static enum {
    ENUMATOR_BANK_ENTER
};

Dialog:ENUMATOR_BANK_ENTER(playerid, response, listitem)
{
    if (!response) {
        return 1;
    }

    Company_Enter(playerid, BANK_LOS_SANTOS, 0.0, 1.5, 0.0);
    return 1;
}

Bank_ShowEntranceDialog(playerid)
{
    if (Dialog_Opened(playerid)) {
        return 0;
    }

    new title[MAX_COMPANY_NAME];
    new name[MAX_COMPANY_NAME];

    Company_GetName(
        BANK_LOS_SANTOS,
        name
    );

    format(title, sizeof(title), "%s", name);

    Dialog_Show(playerid, ENUMATOR_BANK_ENTER,
        DIALOG_STYLE_MSGBOX,
        title,
        "Deseja entrar no Banco?",
        "Entrar",
        "Cancelar"
    );
    return 1;
}
