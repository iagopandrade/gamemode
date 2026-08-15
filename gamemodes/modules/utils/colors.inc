#define ACTION_COLOR            (0xC2A2DAAA)
#define ADMIN_ACTION_COLOR      (0xDAA2ACAA)
#define CMD_USAGE_COLOR         (0xBFC0C2FF)

#define COLOR_EMERALD           (0x2ECC71FF)
#define COLOR_GRAY              (0xB9C9BFFF)
#define COLOR_GREEN             (0x00FF00FF)
#define COLOR_LIGHTRED          (0xFF6347FF)
#define COLOR_ME                (0xC2A2DAAA)
#define COLOR_NEPHRITE          (0x27AE60FF)
#define COLOR_NORMAL_PLAYER     (0xEEEEEEFF)
#define COLOR_ORANGE            (0xFFA500FF)
#define COLOR_PINK              (0xFFAEC9FF)
#define COLOR_PURPLE            (0xC2A2DAAA)
#define COLOR_RED               (0xFF0000FF)
#define COLOR_SOFT_YELLOW       (0xF7D358FF)
#define COLOR_SPRINGGREEN       (0x00FF7FFF)
#define COLOR_SUNFLOWER         (0xF1C40FFF)
#define COLOR_WHITE             (0xFFFFFFFF)
#define COLOR_YELLOW            (0xFFFF00FF)

#define EMBED_CANARY            "{FFEA03}"
#define EMBED_CELTIC_BLUE       "{1170CF}"
#define EMBED_EMERALD           "{2ECC71}"
#define EMBED_FOREST_GREEN      "{0C9E05}"
#define EMBED_GRAY              "{B9C9BF}"
#define EMBED_GREEN             "{00FF00}"
#define EMBED_HARLEQUIN         "{4CFA1B}"
#define EMBED_NEPHRITE          "{27AE60}"
#define EMBED_OFF_RED           "{FA0505}"
#define EMBED_ORANGE            "{F39C12}"
#define EMBED_PINK              "{FFAEC9}"
#define EMBED_RED               "{FF0000}"
#define EMBED_ROBIN_EGG_BLUE    "{11CFCF}"
#define EMBED_SPRINGGREEN       "{00FF7F}"
#define EMBED_SUNFLOWER         "{F1C40F}"
#define EMBED_WHITE             "{FFFFFF}"
#define EMBED_YELLOW            "{FFFF00}"

#define GENERAL_COLOR           (0xEEEEEEFF)

#define LOCAL_TALK_COLOR        (0xD0D0D0FF)
#define MEGAPHONE_COLOR         (0xFFFF00AA)
#define OOC_COLOR               (0xE0FFFFAA)
#define SPEECH_BUBBLE_COLOR     (0xEEEEEEFF)
#define WHISPER_COLOR           (0xFFFF00AA)
/*
static const ColorNames[][] =
{
    "WHITE",
    "OFFWHITE",
    "SNOW",
    "BLACK",
    "JETBLACK",
    "GREY",
    "DARKGREY",
    "LIGHTGREY",
    "SILVER",
    "GAINSBORO",
    "SLATEGREY",
    "DARKSLATEGREY",

    "RED",
    "DARKRED",
    "LIGHTRED",
    "CRIMSON",
    "FIREBRICK",
    "MAROON",
    "INDIANRED",
    "SALMON",
    "DARKSALMON",
    "CORAL",
    "TOMATO",

    "ORANGE",
    "DARKORANGE",
    "LIGHTORANGE",
    "PEACH",
    "APRICOT",
    "AMBER",

    "YELLOW",
    "DARKYELLOW",
    "LIGHTYELLOW",
    "GOLD",
    "KHAKI",
    "MUSTARD",
    "LEMON",

    "GREEN",
    "DARKGREEN",
    "LIGHTGREEN",
    "FORESTGREEN",
    "LIME",
    "LIMEGREEN",
    "OLIVE",
    "DARKOLIVE",
    "SEAGREEN",
    "MINTGREEN",
    "SPRINGGREEN",
    "EMERALD",
    "JADE",
    "TEAGREEN",

    "CYAN",
    "DARKCYAN",
    "LIGHTCYAN",
    "TEAL",
    "TURQUOISE",
    "AQUAMARINE",
    "AQUA",

    "BLUE",
    "DARKBLUE",
    "LIGHTBLUE",
    "NAVY",
    "ROYALBLUE",
    "SKYBLUE",
    "DODGERBLUE",
    "STEELBLUE",
    "CORNFLOWERBLUE",
    "POWDERBLUE",
    "BABYBLUE",
    "ICEBLUE",

    "PURPLE",
    "DARKPURPLE",
    "LIGHTPURPLE",
    "VIOLET",
    "INDIGO",
    "LAVENDER",
    "ORCHID",
    "PLUM",
    "AMETHYST",

    "PINK",
    "HOTPINK",
    "DEEPPINK",
    "LIGHTPINK",
    "MAGENTA",
    "DARKMAGENTA",
    "FUCHSIA",
    "ROSE",

    "BROWN",
    "DARKBROWN",
    "LIGHTBROWN",
    "CHOCOLATE",
    "SIENNA",
    "BEIGE",
    "TAN",
    "WHEAT",
    "BRONZE",
    "COPPER",
    "GOLDMETAL",
    "SILVERMETAL",
    "PLATINUM"
};

static const ColorValues[] =
{
    COLOR_WHITE,
    COLOR_OFFWHITE,
    COLOR_SNOW,
    COLOR_BLACK,
    COLOR_JETBLACK,
    COLOR_GREY,
    COLOR_DARKGREY,
    COLOR_LIGHTGREY,
    COLOR_SILVER,
    COLOR_GAINSBORO,
    COLOR_SLATEGREY,
    COLOR_DARKSLATEGREY,

    COLOR_RED,
    COLOR_DARKRED,
    COLOR_LIGHTRED,
    COLOR_CRIMSON,
    COLOR_FIREBRICK,
    COLOR_MAROON,
    COLOR_INDIANRED,
    COLOR_SALMON,
    COLOR_DARKSALMON,
    COLOR_CORAL,
    COLOR_TOMATO,

    COLOR_ORANGE,
    COLOR_DARKORANGE,
    COLOR_LIGHTORANGE,
    COLOR_PEACH,
    COLOR_APRICOT,
    COLOR_AMBER,

    COLOR_YELLOW,
    COLOR_DARKYELLOW,
    COLOR_LIGHTYELLOW,
    COLOR_GOLD,
    COLOR_KHAKI,
    COLOR_MUSTARD,
    COLOR_LEMON,

    COLOR_GREEN,
    COLOR_DARKGREEN,
    COLOR_LIGHTGREEN,
    COLOR_FORESTGREEN,
    COLOR_LIME,
    COLOR_LIMEGREEN,
    COLOR_OLIVE,
    COLOR_DARKOLIVE,
    COLOR_SEAGREEN,
    COLOR_MINTGREEN,
    COLOR_SPRINGGREEN,
    COLOR_EMERALD,
    COLOR_JADE,
    COLOR_TEAGREEN,

    COLOR_CYAN,
    COLOR_DARKCYAN,
    COLOR_LIGHTCYAN,
    COLOR_TEAL,
    COLOR_TURQUOISE,
    COLOR_AQUAMARINE,
    COLOR_AQUA,

    COLOR_BLUE,
    COLOR_DARKBLUE,
    COLOR_LIGHTBLUE,
    COLOR_NAVY,
    COLOR_ROYALBLUE,
    COLOR_SKYBLUE,
    COLOR_DODGERBLUE,
    COLOR_STEELBLUE,
    COLOR_CORNFLOWERBLUE,
    COLOR_POWDERBLUE,
    COLOR_BABYBLUE,
    COLOR_ICEBLUE,

    COLOR_PURPLE,
    COLOR_DARKPURPLE,
    COLOR_LIGHTPURPLE,
    COLOR_VIOLET,
    COLOR_INDIGO,
    COLOR_LAVENDER,
    COLOR_ORCHID,
    COLOR_PLUM,
    COLOR_AMETHYST,

    COLOR_PINK,
    COLOR_HOTPINK,
    COLOR_DEEPPINK,
    COLOR_LIGHTPINK,
    COLOR_MAGENTA,
    COLOR_DARKMAGENTA,
    COLOR_FUCHSIA,
    COLOR_ROSE,

    COLOR_BROWN,
    COLOR_DARKBROWN,
    COLOR_LIGHTBROWN,
    COLOR_CHOCOLATE,
    COLOR_SIENNA,
    COLOR_BEIGE,
    COLOR_TAN,
    COLOR_WHEAT,
    COLOR_BRONZE,
    COLOR_COPPER,
    COLOR_GOLDMETAL,
    COLOR_SILVERMETAL,
    COLOR_PLATINUM
};

stock ShowColorList(playerid)
{
    SendClientMessage(playerid, COLOR_WHITE, "========== COLOR LIST ==========");

    for(new i = 0; i < sizeof(ColorNames); i++) {
        SendClientMessage(playerid, "Hey, this is the color %s!", ColorValues[i]);
    }

    SendClientMessage(playerid, COLOR_WHITE, "========== END OF LIST ==========");
    return 1;
}

flags:testcolor(CMD_LOGGED)
CMD:testcolor(playerid, params[])
{	
    ShowColorList(playerid);
    return 1;
}
*/