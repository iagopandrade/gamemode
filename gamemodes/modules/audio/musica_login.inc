#if defined _musica_login_included
	#endinput
#endif
#define _musica_login_included

#define URL_MUSICA_LOGIN "https://ia800501.us.archive.org/16/items/brasil-city/Brasil%20City.mp3"

stock TocarMusicaLogin(playerid)
{
	if (IsPlayerConnected(playerid))
	{
		PlayAudioStreamForPlayer(playerid, URL_MUSICA_LOGIN);
	}
	return 1;
}

stock PararMusicaLogin(playerid)
{
	if (IsPlayerConnected(playerid))
	{
		StopAudioStreamForPlayer(playerid);
	}
	return 1;
}
