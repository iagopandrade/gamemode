CMD:motor(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid))
	{
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce precisa estar dentro de um veiculo.");
		return 1;
	}

	if (GetPlayerVehicleSeat(playerid) != 0)
	{
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce precisa estar no banco do motorista.");
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	engine = !engine;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	new name[MAX_PLAYER_NAME], string[144];
	GetPlayerName(playerid, name, sizeof(name));

	if (engine)
		format(string, sizeof(string), "* %s ligou o motor do veiculo.", name);
	else
		format(string, sizeof(string), "* %s desligou o motor do veiculo.", name);

	SendClientMessageToAll(0xC2A2DAFF, string);
	return 1;
}

CMD:farol(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid))
	{
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce precisa estar dentro de um veiculo.");
		return 1;
	}

	if (GetPlayerVehicleSeat(playerid) != 0)
	{
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce precisa estar no banco do motorista.");
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	lights = !lights;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	if (lights)
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce ligou os farois do veiculo.");
	else
		SendClientMessage(playerid, GENERAL_COLOR, "[BC:RP] Voce desligou os farois do veiculo.");
	return 1;
}

/*
	T
	 E
	  S
	   T
	    E
	     D
		 A
		S
	   I
	  L
	 V
	A
*/

flags:portamalascolocar(CMD_LOGGED)
CMD:portamalascolocar(playerid, params[])
{
    new targetid;
    
    if (sscanf(params, "u", targetid)) {
        SendClientMessage(playerid, -1, "Uso: /portamalascolocar [playerid]");
        return 1;
    }

    if (!IsPlayerConnected(targetid)) {
        SendClientMessage(playerid, -1, "Jogador inválido.");
        return 1;
    }
	/*
    if (targetid == playerid) {
        SendClientMessage(playerid, -1, "Você não pode colocar a si mesmo.");
		return 1;
	}
	*/
    new vehicleid = GetPlayerVehicleID(playerid);

    if (vehicleid == INVALID_VEHICLE_ID) {
        SendClientMessage(playerid, -1, "Você precisa estar em um veículo.");
        return 1;
    }

    new Float:px, Float:py, Float:pz;
    new Float:tx, Float:ty, Float:tz;

    GetPlayerPos(playerid, px, py, pz);
    GetPlayerPos(targetid, tx, ty, tz);
	/*
    if (GetPlayerDistanceFromPoint(targetid, px, py, pz) > 3.0) {
        SendClientMessage(playerid, -1, "Esse jogador está muito longe.");
		return 1;
	}
	*/
    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(vehicleid, vx, vy, vz);

    if (GetPlayerDistanceFromPoint(playerid, vx, vy, vz) > 4.0) {
        SendClientMessage(playerid, -1, "Você está muito longe do veículo.");
		return 1;
	}

    SetPlayerInVehicleTrunk(targetid, vehicleid, true);

    SendClientMessage(playerid, -1, "Jogador colocado no porta-malas.");
    SendClientMessage(targetid, -1, "Você foi colocado no porta-malas.");
    return 1;
}
