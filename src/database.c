#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

#define USERS_PATH "database/users.txt"
#define ORDERS_PATH "database/orders.txt"

#define CHAR_SEPARATOR ;
#define LINE_BUFFER_LENGTH 256

struct DatabaseConnection
{
    FILE *users;
    FILE *orders;
};

DatabaseConnection *db_open()
{
    DatabaseConnection *db = (DatabaseConnection *)malloc(sizeof(DatabaseConnection));

    if (db == NULL)
    {
        perror("Falha ao abrir o banco de dados\n");
        return NULL;
    }

    db->users = fopen(USERS_PATH, "a+");
    db->orders = fopen(ORDERS_PATH, "a+");

    return db;
}

void db_close(DatabaseConnection *db)
{
    fclose(db->users);
    fclose(db->orders);
    free(db);
}

// User *db_get_users(DatabaseConnection *db);
// User db_get_user(DatabaseConnection *db, char cpf[CPF_LENGTH]);
// void db_insert_user(DatabaseConnection *db, User *user);
// User db_disable_user(DatabaseConnection *db, User *user);
// User db_delete_user(DatabaseConnection *db, User *user);

// Order *db_get_orders(DatabaseConnection *db, User *user);
// void db_insert_order(DatabaseConnection *db, Order *order);

