#define PEDAGIO_PRECO 100

new PedagioCancela;

enum E_PEDAGIO
{
    Float:pX,
    Float:pY,
    Float:pZ
};

new Pedagio[][E_PEDAGIO] =
{
    {1710.91907, 410.76010, 30.67420}
};

CMD:pedagio(playerid, params[])
{
    if (GetPlayerMoney(playerid) >= PEDAGIO_PRECO)
    {
        GivePlayerMoney(playerid, -PEDAGIO_PRECO);
        AbrirCancela();
        SendClientMessage(playerid, 0x00FF00FF, "Pedágio pago com sucesso!");
    }
    else
    {
        SendClientMessage(playerid, 0xFF0000FF, "Você não tem dinheiro suficiente para pagar o pedágio.");
    }
    return 1;
}

CMD:test(playerid, params[])
{
    GivePlayerMoney(playerid, 10000);
    SetPlayerPos(playerid, 1710.91907, 410.76010, 30.67420);
    return 1;
}

forward FecharCancela();

stock AbrirCancela()
{
    MoveObject(
        PedagioCancela,
        1710.91907,
        410.76010,
        30.67420,
        2.0,
        -280.0,
        -40.0,
        4.0
    );

    SetTimer("FecharCancela", 5000, false);
}

public FecharCancela()
{
    MoveObject(
        PedagioCancela,
        1710.91907,
        410.76010,
        30.67420,
        2.0,
        -280.0,
        40.0,
        4.0
    );
}