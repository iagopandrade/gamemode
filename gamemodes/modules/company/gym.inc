GymGantonInit()
{
    Company_Create(
        GYM_GANTON,
        "Gym Ganton",
        COMPANY_TYPE_GYM
    );

    Company_AddMapIcon(
        GYM_GANTON,
        54,
        2229.8274, -1721.4088, 13.5627
    );

    Company_AddPoint(
        GYM_GANTON,
        COMPANY_POINT_ENTRANCE,
        2229.8274, -1721.4088, 13.5627, 129.3870,
        0.5,
        1239,
        1,
        0,
        0
    );

    Company_AddPoint(
        GYM_GANTON,
        COMPANY_POINT_EXIT,
        772.4016, -5.5164, 1000.7287, 1.3774,
        0.5,
        1239,
        1,
        5,
        0
    );

    return 1;
}

hook OnCompanyPoint@1(playerid, companyid, pointid)
{
    if (companyid != GYM_GANTON) {
        return 1;
    }

    switch (Company_GetPointType(companyid, pointid))
    {
        case COMPANY_POINT_ENTRANCE:
        {
            Gym_ShowEntranceDialog(playerid);
        }

        case COMPANY_POINT_EXIT:
        {
            Company_Exit(playerid, companyid, 0.0, 1.5, 0.1);
        }
    }

    return 1;
}

static enum {
    ENUMATOR_GYM_ENTER
};

Dialog:ENUMATOR_GYM_ENTER(playerid, response, listitem)
{
    if (!response) {
        return 1;
    }

    Company_Enter(playerid, GYM_GANTON, 0.0, 1.5, 0.0);
    return 1;
}

Gym_ShowEntranceDialog(playerid)
{
    if (Dialog_Opened(playerid)) {
        return 0;
    }

    new title[MAX_COMPANY_NAME];
    new name[MAX_COMPANY_NAME];

    Company_GetName(
        GYM_GANTON,
        name
    );

    format(title, sizeof(title), "%s", name);

    Dialog_Show(playerid, ENUMATOR_GYM_ENTER,
        DIALOG_STYLE_MSGBOX,
        title,
        "Deseja entrar na loja?",
        "Entrar",
        "Cancelar"
    );
    return 1;
}

/**
⣿⣿⣿⣿⠟⠉⠉⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠉⠉⠙⣿⣿
⣿⣿⣿⣿⡀⠀⠀⠀⣿⣿⣿⡿⠿⠟⠛⠻⣿⡀⠀⠀⠀⣸⣿
⣿⣿⣿⣿⣷⠶⠖⠚⠉⠉⠀⠀⠀⢀⣀⣴⠿⠿⠶⠶⢾⣿⣿
⣿⣿⠟⠁⠀⠀⠀⠀⢠⣤⣴⣶⣿⠟⠋⠀⠀⠀⠀⠀⠀⠘⣿
⠋⠁⠀⠀⠀⠀⠀⠀⡾⠛⠉⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⣿
⣄⣀⠀⠀⠀⠀⠀⣸⣷⣄⣠⣴⣶⣾⡏⠀⠀⠀⠀⠀⠀⢀⣽
⣿⡟⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⢰⣿⣿⣿⣿
⣿⠇⠀⠀⣠⠀⠀⠘⣿⣿⣿⣿⠏⠀⠀⢀⠀⠀⠈⣿⣿⣿⣿
⣿⠀⠀⢠⣿⣧⠀⠀⢸⣿⣿⡏⠀⠀⣠⣿⣇⠀⠀⢻⣿⣿⣿
⠏⠀⠀⣼⣿⣿⠀⠀⢸⣿⣿⡇⠀⠀⣿⣿⣿⡀⠀⠘⣿⣿⣿
⠀⠀⣸⣿⣿⣿⠀⠀⢸⣿⣿⡇⠀⠀⣿⣿⣿⣧⠀⠀⢹⣿⣿
⣶⣶⣿⣿⣿⣿⣷⣶⣿⣿⣿⣿⣶⣶⣿⣿⣿⣿⣷⣶⣿⣿⣿
 */