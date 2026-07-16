#include "includes\mSelection.inc"

static skins = mS_INVALID_LISTID;

LoadSkins()
{
    skins  = LoadModelSelectionMenu("skins.txt");
}

BincoInit()
{
    Company_Create(
        BINCO_DOWNTOWN,
        "Binco",
        COMPANY_TYPE_CLOTHES
    );

    Company_AddMapIcon(
        BINCO_DOWNTOWN,
        45,
        2244.9133,
        -1664.3789,
        15.4766
    );

    Company_AddPoint(
        BINCO_DOWNTOWN,
        COMPANY_POINT_ENTRANCE,
        2244.9133,
        -1664.3789,
        15.4766,
        347.1557,
        0.5,
        1239,
        1,
        0,
        0
    );

    Company_AddPoint(
        BINCO_DOWNTOWN,
        COMPANY_POINT_CLOTHES,
        206.3019,
        -100.3332,
        1005.2578,
        0.0,
        0.5,
        1275,
        1,
        15,
        0
    );
    /*
    Company_AddPoint(
        BINCO_DOWNTOWN,
        COMPANY_POINT_BUY,
        204.3322,
        -108.7321,
        1005.1328,
        0.0,
        0.5,
        1274,
        1,
        15,
        0
    );
    */
    Company_AddPoint(
        BINCO_DOWNTOWN,
        COMPANY_POINT_EXIT,
        207.4098,
        -109.8123,
        1005.1328,
        358.6200,
        0.5,
        1239,
        1,
        15,
        0
    );

    return 1;
}

hook OnCompanyPoint(playerid, companyid, pointid)
{
    if (companyid != BINCO_DOWNTOWN) {
        return 1;
    }

    switch (Company_GetPointType(companyid, pointid)) {
        case COMPANY_POINT_ENTRANCE:
        {
            Binco_ShowEntranceDialog(playerid);
        }

        case COMPANY_POINT_EXIT:
        {
            Company_Exit(playerid, companyid, 0.0, 1.5, 0.0);
        }

        case COMPANY_POINT_CLOTHES:
        {
            Binco_ShowClothesMenu(playerid);
        }
    }

    return 1;
}

public OnPlayerModelSelection(playerid, response, listid, modelid)
{
	if (listid == skins)
    {
	    if (response)
        {
			new Grana;
			Grana = GetPlayerMoney(playerid);
			if (Grana > 999)
            {
		        SetPlayerSkin(playerid,modelid);
		        GameTextForPlayer(playerid, "~g~Skin comprada!", 3000, 3);
				GivePlayerMoney(playerid, -1000);
			}
            else 
            {
			    GameTextForPlayer(playerid, "~r~Dinheiro insuficiente!", 3000, 3);
			}
		}
		return 1;
	}
	return 1;
}

static enum {
    ENUMATOR_BINCO_ENTER,
    ENUMATOR_BINCO_CLOTHES
};

Dialog:ENUMATOR_BINCO_ENTER(playerid, response, listitem)
{
    if (!response) {
        return 1;
    }

    Company_Enter(playerid, BINCO_DOWNTOWN, 0, 1.5, 0);
    return 1;
}

Dialog:ENUMATOR_BINCO_CLOTHES(playerid, response, listitem)
{
    if (!response) {
        return 1;
    }

    switch (listitem) {
        case 0: { ShowModelSelectionMenu(playerid , skins, "Escolha sua skin", 0x000000FF,0x000000FF,0x00FF00FF); }
        case 1: { ShowModelSelectionMenu(playerid, skins, "Escolha sua skin", 0x000000FF,0x000000FF,0x00FF00FF);}
    }
    return 1;
}

Binco_ShowEntranceDialog(playerid)
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

    Dialog_Show(playerid, ENUMATOR_BINCO_ENTER,
        DIALOG_STYLE_MSGBOX,
        title,
        "Deseja entrar na loja?",
        "Entrar",
        "Cancelar"
    );

    return 1;
}

Binco_ShowClothesMenu(playerid)
{
    if (Dialog_Opened(playerid)) {
        return 0;
    }

    Dialog_Show(playerid, ENUMATOR_BINCO_CLOTHES,
        DIALOG_STYLE_LIST,
        "Binco",
        "Comprar roupas\nTrocar roupa",
        "Selecionar",
        "Fechar"
    );
    return 1;
}