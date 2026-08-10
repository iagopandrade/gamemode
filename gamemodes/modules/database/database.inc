#if defined _INC_database
	#endinput
#endif
#define _INC_database

#define MYSQL_HOST              "127.0.0.1"
#define MYSQL_USER              "root"
#define MYSQL_PASS              ""
#define MYSQL_DB                "servidor"

static MySQL:s_DatabaseHandle;

stock MySQL:GetDatabaseHandle()
{
    return s_DatabaseHandle;
}

stock bool:IsDatabaseConnected()
{
    return bool:(_:s_DatabaseHandle != 0 && mysql_errno(s_DatabaseHandle) == 0);
}

stock ConnectToDatabase()
{
    new MySQLOpt:options = mysql_init_options();

    mysql_set_option(options, AUTO_RECONNECT, true);
    mysql_set_option(options, POOL_SIZE, 2);

    s_DatabaseHandle = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB, options);

    if (mysql_errno(s_DatabaseHandle) != 0) {
        print("[Database] Nao foi possivel conectar ao banco de dados.");
        return 0;
    }

    print("[Database] Conexao estabelecida com sucesso.");
    return 1;
}

stock DisconnectFromDatabase()
{
    mysql_close(s_DatabaseHandle);
    return 1;
}
