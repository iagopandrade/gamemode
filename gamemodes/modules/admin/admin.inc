forward AtualizarFomeSede();

///////////////////////////////////////////////////ESTABELECIMENTOS////////////
new Float:PizzariaLSPos[3] = {2104.9644,-1806.5123,13.5547};   
new Float:PizzariaLSSaidaPos[3] = {372.5565, -131.3607, 1001.4922};
new bool:DentroPizzariaLS[MAX_PLAYERS];   

new AgenciaPickup;
new Float:AgenciaSaidaPos[3] = {-2029.4583, -119.0372, 1035.1719};
new Float:AgenciaMaletaInternaPos[3] = {-2033.1250, -117.3180, 1035.1719};

//////////////////////////////////////////////////
// SISTEMA DE BANCO
//////////////////////////////////////////////////

#define DIALOG_BANCO 107
#define DIALOG_BANCO_DEPOSITAR 108
#define DIALOG_BANCO_SACAR 109
#define DIALOG_BANCO_SALDO 110

new Float:BancoEntradaPos[3] = {1467.2704, -1011.1876, 26.8438};
new Float:BancoInternoPos[3] = {2306.3826, -15.2365, 26.7496};
new bool:DentroBanco[MAX_PLAYERS];

new Float:BalcaoBancoPos[3] = {2315.8311, -10.0434, 26.7422};
new Float:SaidaBancoPos[3] = {2306.3047, -16.0743, 26.7496};

//////////////////////////////////////////////////
// SISTEMA DE EMPREGO - PIZZAIOLO
//////////////////////////////////////////////////

#define DIALOG_AJUDAEMPREGO 106

new bool:Trabalhando[MAX_PLAYERS];
new SkinAntesTrabalho[MAX_PLAYERS];
new bool:CarregandoPizza[MAX_PLAYERS];
new PizzasNaMoto[MAX_PLAYERS];
new FaggioVeiculo[MAX_PLAYERS];
new bool:EmRota[MAX_PLAYERS];
new EntregaAtual[MAX_PLAYERS];
new EntregasRestantes[MAX_PLAYERS];
new RotaOrdem[MAX_PLAYERS][5];