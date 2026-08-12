#if defined _INC_core
	#endinput
#endif
#define _INC_time_core

#define TIMER_INTERVAL      1000
#define MINUTES_PER_TICK    1

static s_WorldHour = 12;   
static s_WorldMinute = 0;     
 
static Text:txtTimeDisp = Text:INVALID_TEXT_DRAW;

public OnGameModeInit()
{
    SetCityWeather(E_CITY_LS, E_WEATHER_EXTRASUNNY_SMOG);
    SetCityWeather(E_CITY_LV, E_WEATHER_EXTRASUNNY_MILD);   // Las Venturas com clima extra ensolarado ameno
    SetCityWeather(E_CITY_SF, E_WEATHER_SANDSTORM);         // San Fierro com tempestade de areia

    txtTimeDisp = TextDrawCreate(605.0, 25.0, "00:00");
    TextDrawUseBox(txtTimeDisp, false);
    TextDrawFont(txtTimeDisp, TEXT_DRAW_FONT_PRICEDOWN);
    TextDrawSetShadow(txtTimeDisp, false);
    TextDrawSetOutline(txtTimeDisp, 2);
    TextDrawBackgroundColour(txtTimeDisp, 0x000000FF);
    TextDrawColour(txtTimeDisp, 0xFFFFFFFF);
    TextDrawAlignment(txtTimeDisp, TEXT_DRAW_ALIGN_RIGHT);
    TextDrawLetterSize(txtTimeDisp, 0.5, 1.5);

    SetTimer("UpdateWorldTime", TIMER_INTERVAL, true);
    
    UpdateWorldTime();

    #if defined TimeCore_OnGameModeInit
        return TimeCore_OnGameModeInit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit TimeCore_OnGameModeInit

#if defined TimeCore_OnGameModeInit
    forward TimeCore_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    SetPlayerTime(playerid, s_WorldHour, s_WorldMinute);

    if (txtTimeDisp != Text:INVALID_TEXT_DRAW) {
        TextDrawShowForPlayer(playerid, txtTimeDisp);
    }

    #if defined TimeCore_OnPlayerConnect
        return TimeCore_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect TimeCore_OnPlayerConnect

#if defined TimeCore_OnPlayerConnect
    forward TimeCore_OnPlayerConnect(playerid);
#endif

public OnPlayerSpawn(playerid)
{
    new cidade = GetPlayerCity(playerid);
    new Clima  = GetCityWeather(cidade);
    
    printf("Seu clima e %s sua cidade e %s", GetClimaName(Clima), GetCityName(cidade));

    #if defined TimeCore_OnPlayerSpawn
        return TimeCore_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn TimeCore_OnPlayerSpawn

#if defined TimeCore_OnPlayerSpawn
    forward TimeCore_OnPlayerSpawn(playerid);
#endif

stock GetServerTime(&hour, &minute = 0)
{
    hour = s_WorldHour;
    minute = s_WorldMinute;
    return 1;
}

stock SetServerTime(hour, minute)
{
    s_WorldHour = hour;
    s_WorldMinute = minute;
    return 1;
}

forward UpdateWorldTime();
public UpdateWorldTime()
{
    s_WorldMinute += MINUTES_PER_TICK;

    if (s_WorldMinute >= 60) {
        s_WorldMinute -= 60;
        s_WorldHour++;

        if (s_WorldHour >= 24) {
            s_WorldHour = 0;
        }
    }

    foreach (new i : Player) {
        SetPlayerTime(i, s_WorldHour, s_WorldMinute);
    }

    new timeString[6];
    format(timeString, sizeof(timeString), "%02d:%02d", s_WorldHour, s_WorldMinute);
    TextDrawSetString(txtTimeDisp, timeString);
    return 1;
}
