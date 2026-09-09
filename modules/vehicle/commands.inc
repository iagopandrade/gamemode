#if defined _INC_vehicle_commands
	#endinput
#endif
#define _INC_vehicle_commands

flags:motor(CMD_LOGGED)
CMD:motor(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid)) {
		GameTextForPlayer(playerid,"~r~Você precisa estar dentro de um veículo.", 5000, 16);
		return 1;
	}

	/*
	if (GetPlayerVehicleSeat(playerid) != 0) {
		SendClientMessage(playerid, GENERAL_COLOR, "Você precisa estar no banco do motorista.");
		return 1;
	}
	*/

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

flags:farol(CMD_LOGGED)
CMD:farol(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid)) {
		GameTextForPlayer(playerid,"~r~Você precisa estar dentro de um veículo.", 5000, 16);
		return 1;
	}

	/*
	if (GetPlayerVehicleSeat(playerid) != 0) {
		SendClientMessage(playerid, GENERAL_COLOR, "Voce precisa estar no banco do motorista.");
		return 1;
	}
	*/

	new vehicleid = GetPlayerVehicleID(playerid);
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	lights = !lights;
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	if (lights)
		SendClientMessage(playerid, GENERAL_COLOR, "Voce ligou os farois do veiculo.");
	else
		SendClientMessage(playerid, GENERAL_COLOR, "Voce desligou os farois do veiculo.");
	return 1;
}

flags:vcolor(CMD_LOGGED)
CMD:vcolor(playerid, params[])
{
    new vehicleid;
    new colour1;
    new colour2;

    if (sscanf(params, "ddd", vehicleid, colour1, colour2)) {
        SendClientMessage(playerid, GENERAL_COLOR, "Uso: /vcolor [vehicleid] [cor1] [cor2]");
        return 1;
    }

    if (!IsValidVehicle(vehicleid)) {
        SendClientMessage(playerid, GENERAL_COLOR, "Erro: o ID do veiculo e invalido ou o veiculo nao existe.");
        return 1;
    }

    if (!IsValidVehicleColour(colour1)) {
        SendClientMessage( playerid, GENERAL_COLOR, "Erro: a primeira cor deve estar entre 0 e 255.");
        return 1;
    }

    if (!IsValidVehicleColour(colour2)) {
        SendClientMessage(playerid, GENERAL_COLOR, "Erro: a segunda cor deve estar entre 0 e 255.");
        return 1;
    }

    ChangeVehicleColours(vehicleid, colour1, colour2);

    SendClientMessage(playerid, GENERAL_COLOR, "Veículo %d alterado para as cores %d e %d.", vehicleid, colour1, colour2);
    return 1;
}

flags:vpaintjob(CMD_LOGGED)
CMD:vpaintjob(playerid, params[])
{
    new vehicleid;
    new paintjobid;

    if (sscanf(params, "dd", vehicleid, paintjobid)) {
        SendClientMessage(playerid, 0xFFFFFFFF, "Uso: /vpaintjob [vehicleid] [paintjobid]");
        return 1;
    }

    if (!IsValidVehicle(vehicleid)) {
        SendClientMessage(playerid, 0xFF4444FF, "Erro: o ID do veiculo e invalido ou o veiculo nao existe.");
        return 1;
    }

    new modelid = GetVehicleModel(vehicleid);
    new paintjobCount = GetVehiclePaintjobCount(modelid);

    if (paintjobCount == 0) {
        SendClientMessage(playerid, GENERAL_COLOR, "Erro: este modelo de veiculo nao suporta paintjob.");
        return 1;
    }

    if (!IsValidPaintjob(modelid, paintjobid)) {
        SendClientMessage(playerid, GENERAL_COLOR, "Erro: paintjob invalido. Este veiculo aceita IDs de 0 ate %d.", paintjobCount - 1);
        return 1;
    }

    // Branco para obter o melhor resultado visual do paintjob.
    // ChangeVehicleColor(vehicleid, 1, 1);

    ChangeVehiclePaintjob(vehicleid, paintjobid);

    SendClientMessage(playerid, GENERAL_COLOR, "Paintjob %d aplicado no veiculo %d (modelo %d).", paintjobid, vehicleid, modelid);
    return 1;
}

flags:idveh(CMD_LOGGED)
CMD:idveh(playerid, params[])
{
	SendClientMessage(playerid, GENERAL_COLOR, "%d", GetPlayerVehicleID(playerid));
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
