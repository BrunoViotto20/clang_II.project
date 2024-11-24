#include <stdbool.h>
#include <stdio.h>
#include <locale.h>

#include "database.h"
#include "lanchonete.h"
#include "result.h"

int main(void)
{
    setlocale(LC_ALL, "pt-BR");

    DatabaseResult open_result = db_open();
    if (!open_result.is_success)
    {
        perror(open_result.error.message);
        return -1;
    }
    Database *connection = open_result.database;

    bool exit;
    do
    {
        exit = menu_principal(connection);
    } while (!exit);

    UnitResult close_result = db_close(connection);
    if (!close_result.is_success)
    {
        perror(close_result.error.message);
        return -1;
    }

    return 0;
}
