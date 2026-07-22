stock ShowLoginDialog(playerid, const message[])
{
    Dialog_Show(playerid, "Login", DIALOG_STYLE_PASSWORD, "Login", message, "Entrar", "Sair");
    return 1;
}

stock ShowRegisterDialog(playerid, const message[])
{
    Dialog_Show(playerid, "Registro", DIALOG_STYLE_PASSWORD, "Criar conta", message, "Próximo", "Sair");
    return 1;
}

stock ShowRegisterConfirmDialog(playerid, const message[])
{
    Dialog_Show(playerid, "RegistroConfirmar", DIALOG_STYLE_PASSWORD, "Confirmar senha", message, "Confirmar", "Sair");
    return 1;
}

Dialog:Login(playerid, response, listitem, inputtext[])
{
    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    if (strlen(inputtext) == 0) {
        ShowLoginDialog(
            playerid, 
            "Voce precisa digitar uma senha.\nDigite sua senha para entrar:"
        );
        return 1;
    }

    ValidatePlayerPassword(playerid, inputtext);
    return 1;
}

Dialog:Registro(playerid, response, listitem, inputtext[])
{
    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    if (strlen(inputtext) < MIN_PASSWORD_LEN) {
        ShowRegisterDialog(
            playerid, 
            "A senha deve ter pelo menos 4 caracteres.\nDigite uma senha para criar sua conta:"
        );
        return 1;
    }

    HashPlayerPassword(playerid, inputtext, s_AccountPasswordHash[playerid]);
    ShowRegisterConfirmDialog(playerid, "Digite a mesma senha novamente para confirmar:");
    return 1;
}

Dialog:RegistroConfirmar(playerid, response, listitem, inputtext[])
{
    new hash[HASH_LEN];

    if (!response) {
        SetTimerEx("DelayedKick", 500, false, "d", playerid);
        return 1;
    }

    HashPlayerPassword(playerid, inputtext, hash);

    if (!strcmp(hash, s_AccountPasswordHash[playerid])) {
        CreatePlayerAccount(playerid);
    } else {
        ShowRegisterDialog(
            playerid, 
            "As senhas nao coincidem.\nDigite uma senha para criar sua conta:"
        );
    }
    return 1;
}
