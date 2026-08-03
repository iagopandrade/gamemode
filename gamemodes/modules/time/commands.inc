#if defined _INC_time_commands
	#endinput
#endif
#define _INC_time_commands

flags:setartempo(CMD_ADMIN)
CMD:setartempo(playerid, params[])
{
    new hour, minute;
    GetServerTime(hour, minute);

    if (sscanf(params, "dd", hour, minute)) {
        SendClientMessage(playerid, -1, "Uso: /setartempo <hour> <minute>");
        return 1;
    }
    
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        SendClientMessage(playerid, -1, "hour invalida! (0-23 h / 0-59 min)");
        return 1;
    }

    SetServerTime(hour, minute);
    
    foreach (new i : Player) {
        SetPlayerTime(i, hour, minute);
    }
    return 1;
}
alias:setartempo("st")

flags:setarclima(CMD_ADMIN)
CMD:setarclima(playerid, params[])
{
    new clima;
    if (sscanf(params, "d", clima)) {
        SendClientMessage(playerid, -1, "Uso: /setarclima <clima>");
        return 1;
    }
    
    if (clima < 0 || clima > 21) {
        SendClientMessage(playerid, -1, "clima inv");
        return 1;
    }

    SetWeather(clima);
    // foreach (new i : Player) {
    //     SetPlayerWeather(playerid, clima);
    // }
    return 1;
}
alias:setarclima("sc")
