#if defined _INC_character_commands
	#endinput
#endif
#define _INC_character_commands

#include "modules/character/commands/cash_payment.inc"

flags:rotateme(CMD_LOGGED)
CMD:rotateme(playerid, params[])
{
	TogglePlayerSpectating(playerid, true);
	InterpolateCameraLookAt(playerid, 50.0, 50.0, 10.0, -50.0, 50.0, 10.0, 10000, CAMERA_MOVE);
	return 1;
}
