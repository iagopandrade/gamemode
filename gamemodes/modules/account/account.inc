///////////////////////////////////////////////////AGENCIA DE EMPREGO//////////
#define PROFISSAO_DESEMPREGADO 0
#define PROFISSAO_PIZZAIOLO    1

new Profissao[MAX_PLAYERS];
new SaldoBanco[MAX_PLAYERS];
new Fome[MAX_PLAYERS];
new Sede[MAX_PLAYERS];


#define DIALOG_ACMDS 102
#define LEVEL_HELPER      1
#define LEVEL_GAMEADMIN   2
#define LEVEL_ENCARREGADO 3
#define LEVEL_FUNDADOR    4
#define DIALOG_STATS 104
#define DIALOG_EMPREGOS 105

new AdminLevel[MAX_PLAYERS];
new bool:AdminDuty[MAX_PLAYERS];
new Warnings[MAX_PLAYERS];
new bool:Muted[MAX_PLAYERS];
new bool:Jailed[MAX_PLAYERS];
new Float:PosAntesEspectar[MAX_PLAYERS][3];
new bool:Espectando[MAX_PLAYERS];
new Nivel[MAX_PLAYERS];
new TempoJogado[MAX_PLAYERS]; // em segundos, desde o ultimo level up
new MinutosPayday[MAX_PLAYERS]; // contador ate completar 60 minutos
new Float:AgenciaPos[3] = {1412.3226, -1700.2135, 13.5395};
new bool:DentroAgencia[MAX_PLAYERS];
new Text:LogoServidor;
new Text:LogoLinha;
new Text:LogoEstrelaEsquerda;
new Text:LogoEstrelaDireita;
new SkinSalva[MAX_PLAYERS];
new Float:UltimaPosX[MAX_PLAYERS];
new Float:UltimaPosY[MAX_PLAYERS];
new Float:UltimaPosZ[MAX_PLAYERS];
new UltimoInterior[MAX_PLAYERS];

#define DIALOG_LOGIN    100
#define DIALOG_REGISTER 101
#define DIALOG_SKIN 103

new bool:IsLoggedIn[MAX_PLAYERS];
new SenhaHash[MAX_PLAYERS];
new SkinAtual[MAX_PLAYERS];

stock UserPath(playerid)
{
	new name[MAX_PLAYER_NAME], path[64];
	GetPlayerName(playerid, name, sizeof(name));
	format(path, sizeof(path), "Users/%s.ini", name);
	return path;
}

stock HashSenha(senha[])
{
	new hash = 0;
	for (new i = 0; i < strlen(senha); i++) hash = (hash * 31) + senha[i];
	return hash;
}

stock SalvarConta(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new File:conta = fopen(UserPath(playerid), io_write);
	new linha[350];
	format(linha, sizeof(linha), "%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%f\r\n%f\r\n%f\r\n%d\r\n%d\r\n%d\r\n",
		SenhaHash[playerid], AdminLevel[playerid], GetPlayerSkin(playerid), GetPlayerMoney(playerid),
		Nivel[playerid], TempoJogado[playerid], MinutosPayday[playerid], Profissao[playerid], SaldoBanco[playerid],
		x, y, z, GetPlayerInterior(playerid), Fome[playerid], Sede[playerid]);
	fwrite(conta, linha);
	fclose(conta);
	return 1;
}

stock MostrarSkin(playerid)
{
	new titulo[64];
	format(titulo, sizeof(titulo), "Skin atual: ID %d", SkinAtual[playerid]);
	ShowPlayerDialog(playerid, DIALOG_SKIN, DIALOG_STYLE_LIST, titulo, "Proxima Skin >>\n<< Skin Anterior\nConfirmar Skin", "Selecionar", "");
	return 1;
}

stock bool:SkinDePolicia(skinid)
{
	if (skinid >= 265 && skinid <= 267) return true; // Tenpenny, Pulaski, Hernandez
	if (skinid >= 280 && skinid <= 288) return true; // LAPD, SFPD, LVPD, Sheriff, SWAT, FBI, Army
	if (skinid >= 301 && skinid <= 307) return true; // variantes de policia sem colete
	return false;
}