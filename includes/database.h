#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "lanchonete.h"

typedef struct DatabaseConnection DatabaseConnection;

/// @brief Opens a connection to the database.
/// @return Returns a pointer to the database connection.
DatabaseConnection *db_open();

/// @brief Closes the connection to the database.
/// @param db The database connection to be closed.
void db_close(DatabaseConnection *db);

User *db_get_users(DatabaseConnection *db);
User db_get_user(DatabaseConnection *db, char cpf[CPF_LENGTH]);
void db_insert_user(DatabaseConnection *db, User *user);
User db_disable_user(DatabaseConnection *db, User *user);
User db_delete_user(DatabaseConnection *db, User *user);

Order *db_get_orders(DatabaseConnection *db, User *user);
void db_insert_order(DatabaseConnection *db, Order *order);

#endif
