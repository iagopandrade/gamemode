/*
    FilterScript Sistema de casas C-Casas.
    Criado por Cleyson Stein - StNN
    Não retire os créditos.
    Todos os direitos reservados. 02/05/2018*
    Link: https://sampforum.blast.hk/showthread.php?tid=653371
*/
#if defined _INC_house_commands
	#endinput
#endif
#define _INC_house_commands

flags:criarcasa(CMD_LOGGED | CMD_ADMIN)
CMD:criarcasa(playerid, params[])
{
    new houseid;
    new houseprice;
    new interiorid;
    if (sscanf(params, "iii", houseid, houseprice, interiorid)) {
        SendClientMessage(playerid, COLOR_GRAY, "Use /criarcasa <id> <preco> <interior 0-10>");
        return 1;
    }
    Criar_Casa(playerid, houseid, houseprice, interiorid);
    return 1;
}

flags:ircasa(CMD_LOGGED | CMD_ADMIN)
CMD:ircasa(playerid, params[])
{
    if (sscanf(params, "i", params[0])) {
        SendClientMessage(playerid, COLOR_GRAY, "Use /ircasa [id]");
        return 1;
    }
    Ir_Casa(playerid, params[0]);
    return 1;
}

flags:editarcasa(CMD_LOGGED | CMD_ADMIN)
CMD:editarcasa(playerid, params[])
{
    if (sscanf(params, "iii", params[0], params[1], params[2])) {
        SendClientMessage(playerid, COLOR_GRAY, "Use /editarcasa [id] [preco] [int 0a10]");
        return 1;
    }
    Editar_Casa(playerid, params[0], params[1], params[2]);
    return 1;
}

flags:deletarcasa(CMD_LOGGED | CMD_ADMIN)
CMD:deletarcasa(playerid, params[])
{
    if (sscanf(params, "i", params[0])) {
        SendClientMessage(playerid, COLOR_GRAY, "Use /deletarcasa [id]");
        return 1;
    }
    Deletar_Casa(playerid, params[0]);
    return 1;
}

flags:comprarcasa(CMD_LOGGED)
CMD:comprarcasa(playerid)
{
    for (new i = 0; i < MAX_HOUSES; i++)
    {
        if (IsPlayerInRangeOfPoint(playerid, 1, c[i][cX], c[i][cY], c[i][cZ]))
        {
            if(p[playerid][TemCasa] > 0) return SendClientMessage(playerid, COLOR_RED, "Vocк ja possue uma casa.");
            if(c[i][cTemDono] == 1) return SendClientMessage(playerid, COLOR_RED, "Essa casa ja possue dono.");
            if(GetPlayerMoney(playerid) < c[i][cPreco]) return SendClientMessage(playerid, COLOR_RED, "Dinheiro insuficiente.");
            format(c[i][cDono], 24, p[playerid][Nome]);
            c[i][cTemDono] = 1;
            p[playerid][TemCasa] = i;
            SendClientMessage(playerid, COLOR_ORANGE, "Casa adiquirida com sucesso.");
            GivePlayerMoney(playerid, -c[i][cPreco]);
            DestroyDynamicPickup(c[i][cPickup]);
            DestroyDynamicMapIcon(c[i][cIcone]);
            DestroyDynamic3DTextLabel(c[i][cText]);
            new texto[200];
            c[i][cIcone] = CreateDynamicMapIcon(c[i][cX], c[i][cY], c[i][cZ], 32, -1);
            c[i][cPickup] = CreateDynamicPickup(19522, 21, c[i][cX], c[i][cY], c[i][cZ]);//1272
            
            format(texto, sizeof(texto), "{B22222}CASA ID: {FFFFFF}%d\n{CD5C5C}Dono: {FFFFFF}%s\n{CD5C5C}Preзo: {FFFFFF}$ %d{CD5C5C}\n/ajudacasa", i, c[i][cDono], c[i][cPreco]);
            c[i][cText] = CreateDynamic3DTextLabel(texto, 0xFFFFFFAA, c[i][cX], c[i][cY], c[i][cZ]+1, 20);
            new Query[80];
            format(Query, sizeof(Query), "UPDATE CASAS SET DONO ='%s', TEMDONO = 1 WHERE ID =%d", c[i][cDono], i);
            DB_ExecuteQuery(Database, Query);
            break;
        }
    }
    return 1;
}

flags:vendercasa(CMD_LOGGED)
CMD:vendercasa(playerid)
{
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 1, c[i][cX], c[i][cY], c[i][cZ]))
        {
            if(p[playerid][TemCasa] == 0) return SendClientMessage(playerid, COLOR_RED, "Vocк nгo possue uma casa.");
            if(c[i][cTemDono] == 0) return SendClientMessage(playerid, COLOR_RED, "Essa casa nгo possue dono.");
            if(p[playerid][TemCasa] == i)
            {
                SendClientMessage(playerid, COLOR_ORANGE, "Casa vendida com sucesso.");
                GivePlayerMoney(playerid, c[i][cPreco]/2);
                format(c[i][cDono], 24, "Nenhum");
                c[i][cTemDono] = 0;
                p[playerid][TemCasa] = 0;
                DestroyDynamicPickup(c[i][cPickup]);
                DestroyDynamicMapIcon(c[i][cIcone]);
                DestroyDynamic3DTextLabel(c[i][cText]);
                new texto[200];
                c[i][cIcone] = CreateDynamicMapIcon(c[i][cX], c[i][cY], c[i][cZ], 31, -1);
                c[i][cPickup] = CreateDynamicPickup(1273, 21, c[i][cX], c[i][cY], c[i][cZ]);
                format(texto, sizeof(texto), "{2E8B57}CASA ID: {FFFFFF}%d\n{3CB371}Dono: {FFFFFF}%s\n{3CB371}Preзo: {FFFFFF}$ %d\n{3CB371}Aperte 'F' para entrar\n/ajudacasa", i, c[i][cDono], c[i][cPreco]);
                c[i][cText] = CreateDynamic3DTextLabel(texto, 0xFFFFFFAA, c[i][cX], c[i][cY], c[i][cZ]+1, 20);
                new Query[80];
                format(Query, sizeof(Query), "UPDATE CASAS SET DONO ='%s', TEMDONO = 0 WHERE ID =%d", c[i][cDono], i);
                DB_ExecuteQuery(Database, Query);
                break;
                } else {
                SendClientMessage(playerid, COLOR_RED, "Essa casa nгo й sua.");
                return 1;
               }
        }
    }
    return 1;
}

flags:trancar(CMD_LOGGED)
CMD:trancar(playerid)
{
    for (new i = 0; i < MAX_HOUSES; i++)
    {
        if (IsPlayerInRangeOfPoint(playerid, 1, c[i][cX], c[i][cY], c[i][cZ]))
        {
            if(p[playerid][TemCasa] == 0) return SendClientMessage(playerid, COLOR_RED, "Vocк nгo possue uma casa.");
            if(c[i][cTemDono] == 0) return SendClientMessage(playerid, COLOR_RED, "Essa casa nгo possue dono.");
            if(p[playerid][TemCasa] == i)
            {
                new Query[80];
                if(c[i][cTrancada] == 0)
                {
                    SendClientMessage(playerid, COLOR_ORANGE, "Casa trancada.");
                    c[i][cTrancada] = 1;
                    
                    format(Query, sizeof(Query), "UPDATE CASAS SET TRANCADA = 1 WHERE ID =%d", i);
                    DB_ExecuteQuery(Database, Query);
                    break;
                    
                    } else {
                    
                    SendClientMessage(playerid, COLOR_ORANGE, "Casa destrancada.");
                    c[i][cTrancada] = 0;
                    
                    format(Query, sizeof(Query), "UPDATE CASAS SET TRANCADA = 0 WHERE ID =%d", i);
                    DB_ExecuteQuery(Database, Query);
                    break;
                }
                } else {
                SendClientMessage(playerid, COLOR_RED, "Essa casa nгo й sua.");
                return 1;
               }
        }
    }
    return 1;
}

flags:ajudacasa(CMD_LOGGED)
CMD:ajudacasa(playerid)
{
    Dialog_Show(playerid, "Comandos Casas",
        DIALOG_STYLE_MSGBOX, 
        "Informacao | "EMBED_RED" Comandos Casas",    
        "{CECECE}/comprarcasa {FFFFFF}- Comprar uma casa.\
        \n{CECECE}/vendercasa {FFFFFF}- Vender sua casa.\
        \n{CECECE}/trancar {FFFFFF}- trancar | destrancar casa.\n",
        "", ""
    );
    return 1;
}
alias:ajudacasa("acasa")
