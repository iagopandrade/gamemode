CMD:pararmusica(playerid, params[])
{
	StopAudioStreamForPlayer(playerid);
	SendClientMessage(playerid, -1, "Musica parada.");
	return 1;
}