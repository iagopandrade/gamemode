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