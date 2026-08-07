#if defined _INC_character_commands_cash_payment
	#endinput
#endif
#define _INC_character_commands_cash_payment

#include <pp-hooks>

static PlayerText:s_PaymentTextDraw[MAX_PLAYERS][2];

stock CreatePaymentTextDraw(playerid) 
{
    s_PaymentTextDraw[playerid][0] = CreatePlayerTextDraw(playerid, -1.1, 230.0, "Aceitar");
    PlayerTextDrawFont(playerid, s_PaymentTextDraw[playerid][0], TEXT_DRAW_FONT_0);
    PlayerTextDrawLetterSize(playerid, s_PaymentTextDraw[playerid][0], 0.600, 2.300);
    PlayerTextDrawColour(playerid, s_PaymentTextDraw[playerid][0], 0xFFFFFFFF);
    PlayerTextDrawAlignment(playerid, s_PaymentTextDraw[playerid][0], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawUseBox(playerid, s_PaymentTextDraw[playerid][0], true);
    PlayerTextDrawBoxColour(playerid, s_PaymentTextDraw[playerid][0], 0x000000AA);
    PlayerTextDrawTextSize(playerid, s_PaymentTextDraw[playerid][0], 58.4, 25.0);
    PlayerTextDrawBackgroundColour(playerid, s_PaymentTextDraw[playerid][0], 0x000000FF);
    PlayerTextDrawSetProportional(playerid, s_PaymentTextDraw[playerid][0], true);
    PlayerTextDrawSetSelectable(playerid, s_PaymentTextDraw[playerid][0], true);

    s_PaymentTextDraw[playerid][1] = CreatePlayerTextDraw(playerid, -1.1, 264.0, "Recusar");
    PlayerTextDrawFont(playerid, s_PaymentTextDraw[playerid][1], TEXT_DRAW_FONT_0);
    PlayerTextDrawLetterSize(playerid, s_PaymentTextDraw[playerid][1], 0.600, 2.300);
    PlayerTextDrawColour(playerid, s_PaymentTextDraw[playerid][1], 0xFFFFFFFF);
    PlayerTextDrawAlignment(playerid, s_PaymentTextDraw[playerid][1], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawUseBox(playerid, s_PaymentTextDraw[playerid][1], true);
    PlayerTextDrawBoxColour(playerid, s_PaymentTextDraw[playerid][1], 0x000000AA);
    PlayerTextDrawTextSize(playerid, s_PaymentTextDraw[playerid][1], 58.4, 25.0);
    PlayerTextDrawBackgroundColour(playerid, s_PaymentTextDraw[playerid][1], 0x000000FF);
    PlayerTextDrawSetProportional(playerid, s_PaymentTextDraw[playerid][1], true);
    PlayerTextDrawSetSelectable(playerid, s_PaymentTextDraw[playerid][1], true);
}

static enum e_PENDING_PAYMENT
{
    bool:E_PAYMENT_ACTIVE,
    E_PAYMENT_FROM,
    E_PAYMENT_VALUE
};

static s_PendingPayment[MAX_PLAYERS][e_PENDING_PAYMENT];

static s_PaymentTDTimer[MAX_PLAYERS];

stock ShowPaymentTextDraw(sender, payee, amount)
{
    if (s_PaymentTDTimer[payee]) {
        KillTimer(s_PaymentTDTimer[payee]);
        s_PaymentTDTimer[payee] = 0;
    }

    s_PendingPayment[payee][E_PAYMENT_ACTIVE] = true;
    s_PendingPayment[payee][E_PAYMENT_FROM] = sender;
    s_PendingPayment[payee][E_PAYMENT_VALUE] = amount;

    SendPlayerPaymentMessage(sender, payee, amount);

    SelectTextDraw(payee, 0xFFFFFFFF);

    PlayerTextDrawShow(payee, s_PaymentTextDraw[payee][0]);
    PlayerTextDrawShow(payee, s_PaymentTextDraw[payee][1]);

    s_PaymentTDTimer[payee] = SetTimerEx("HidePaymentTextDraw", 5000, false,  "i", payee);
    return 1;
}

stock SendPlayerPaymentMessage(sender, payee, amount)
{
    new nameSender[MAX_PLAYER_NAME];
    GetPlayerName(sender, nameSender, sizeof(nameSender));

    new namePayee[MAX_PLAYER_NAME];
    GetPlayerName(payee, namePayee, sizeof(namePayee));

    new Float:floatMoney = amount;

    SendClientMessage(sender, COLOR_ME, "> %s oferece R$%.2f a %s.", nameSender, floatMoney, namePayee);
    
    SendClientMessage(payee, COLOR_ME, "* %s esta lhe oferecendo R$%.2f.", nameSender, floatMoney);
    return 1;
}

forward HidePaymentTextDraw(playerid);
public HidePaymentTextDraw(playerid)
{
    s_PaymentTDTimer[playerid] = 0;

    CancelSelectTextDraw(playerid);

    PlayerTextDrawHide(playerid, s_PaymentTextDraw[playerid][0]);
    PlayerTextDrawHide(playerid, s_PaymentTextDraw[playerid][1]);

    s_PendingPayment[playerid][E_PAYMENT_ACTIVE] = false;
    s_PendingPayment[playerid][E_PAYMENT_FROM] = INVALID_PLAYER_ID;
    s_PendingPayment[playerid][E_PAYMENT_VALUE] = 0;
    return 1;
}

flags:reset(CMD_LOGGED)
CMD:reset(playerid, params[])
{
    ResetPlayerMoney(playerid);
    return 1;
}

flags:pagar(CMD_LOGGED)
CMD:pagar(playerid, params[])
{
    new targetid;
    new money;

    if (sscanf(params, "ui", targetid, money)) {
        GameTextForPlayer(
            playerid,"~r~SINTAXE~n~~w~Use: /pagar <id> <valor>",
             5000, 16
        );
        return 1;
    }

    if (money <= 0) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~O valor deve ser maior que zero",
            5000, 16
        );
        return 1;
    }

    if (targetid == playerid) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Voce nao pode pagar para voce mesmo",
            5000, 16
        );
        return 1;
    }

    if (!IsPlayerConnected(targetid)) {
		GameTextForPlayer(
			playerid,
			"~r~ERRO~n~~w~Esse jogador nao esta conectado", 
			5000, 16
		);
		return 1;
	}

    if (GetPlayerMoney(playerid) < money) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Voce nao tem dinheiro suficiente",
            5000, 16
        );
        return 1;
    }

    if (s_PendingPayment[targetid][E_PAYMENT_ACTIVE]) {
        GameTextForPlayer(
            playerid,
            "~r~ERRO~n~~w~Aguarde um instante",
            5000,
            16
        );
        return 1;
    }

    ShowPaymentTextDraw(playerid, targetid, money);
    return 1;
}

hook OnPlayerConnect(playerid)
{
    CreatePaymentTextDraw(playerid);
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (s_PaymentTDTimer[playerid]) {
        KillTimer(s_PaymentTDTimer[playerid]);
        s_PaymentTDTimer[playerid] = 0;
    }

    s_PendingPayment[playerid][E_PAYMENT_ACTIVE] = false;
    s_PendingPayment[playerid][E_PAYMENT_FROM] = INVALID_PLAYER_ID;
    s_PendingPayment[playerid][E_PAYMENT_VALUE] = 0;
    return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (playertextid == s_PaymentTextDraw[playerid][0]) {
        if (!s_PendingPayment[playerid][E_PAYMENT_ACTIVE]) {
            return 1;
        }

        new senderid = s_PendingPayment[playerid][E_PAYMENT_FROM];
        new value  = s_PendingPayment[playerid][E_PAYMENT_VALUE];

        if (!IsPlayerConnected(senderid)) {
            GameTextForPlayer(
                playerid,
                "~r~ERRO~n~~w~Esse jogador nao esta mais conectado", 
                5000, 16
            );
            return 1;
        }

        if (GetPlayerMoney(senderid) < value) {
            HidePaymentTextDraw(playerid);

            GameTextForPlayer(
                playerid,
                "~r~ERRO~n~~w~O pagador nao possui mais dinheiro suficiente",
                5000,
                16
            );
            return 1;
        }

        FinalizePayment(senderid, playerid, value);

        HidePaymentTextDraw(playerid);
        return 1;
    } else if (playertextid == s_PaymentTextDraw[playerid][1]) {
        HidePaymentTextDraw(playerid);

        ApplyAnimation(playerid, "GANGS", "Invite_No", 4.1, false, false, false, false, 0, SYNC_ALL);

        new payeeName[MAX_PLAYER_NAME];

        GetPlayerName(playerid, payeeName, sizeof(payeeName));

        new string[128];

        format(string, sizeof(string), "* %s recusou a oferta.", payeeName);

        SetPlayerChatBubble(playerid, string, COLOR_ME, 15.0, 5000);

        SendClientMessage(playerid, COLOR_ME, "> %s recusou a oferta.", payeeName);
        return 1;
    }
    return 0;
}

stock FinalizePayment(senderid, payeeid, amount)
{
    new slot = 0;

    SetPlayerAttachedObject(senderid, slot, 1212, 5, 0.0900, 0.0459, -0.0419, -8.8999, 88.1999, 3.9999, 1.0000, 1.0000, 0.1560, 0xFFFFFFFF, 0xFFFFFFFF);
    SetTimerEx("RemovePlayerMoneyAttached", 5000, false,  "ii", senderid, slot);

    ApplyAnimation(senderid, "INT_SHOP", "shop_pay", 4.1, false, false, false, false, 0, SYNC_ALL);
    ApplyAnimation(payeeid, "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0, SYNC_ALL);
    
    GivePlayerMoney(senderid, -amount);
    GivePlayerMoney(payeeid, amount);

    new senderName[MAX_PLAYER_NAME];
    new payeeName[MAX_PLAYER_NAME];

    GetPlayerName(senderid, senderName, sizeof(senderName));
    GetPlayerName(payeeid, payeeName, sizeof(payeeName));

    new string[128];

    format(string, sizeof(string), "* %s entregou uma quantia em dinheiro a %s", senderName, payeeName);

    SetPlayerChatBubble(senderid, string, COLOR_ME, 15.0, 5000);

    SendClientMessage(senderid, COLOR_ME, "> %s entregou uma quantia em dinheiro a %s", senderName, payeeName);

    new Float:x;
    new Float:y;
    new Float:z;

	GetPlayerPos(senderid, x, y, z);

    PlayerPlaySound(senderid, 5405, x, y, z);
	PlayerPlaySound(payeeid, 5405, x, y, z);

    new str[16];
    
    format(str, sizeof(str), "R$%.2f", Float:amount);

    GameTextForPlayer(senderid, "-%s", 3000, 12, str);
    GameTextForPlayer(payeeid, "+%s", 3000, 11, str);
	return 1;
}

forward RemovePlayerMoneyAttached(playerid, slot);
public RemovePlayerMoneyAttached(playerid, slot)
{
    if (!IsPlayerAttachedObjectSlotUsed(playerid, slot)) {
        return 0;
    }
    RemovePlayerAttachedObject(playerid, slot);
    return 1;
}
