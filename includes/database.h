#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "lanchonete.h"

/// @brief Represents a connection to the database, required to access and manipulate it.
typedef struct DatabaseConnection DatabaseConnection;

/// @brief Opens a connection to the database.
/// @return Returns a pointer to the database connection.
DatabaseConnection *db_open();

/// @brief Closes the connection to the database.
/// @param db The database connection to be closed.
void db_close(DatabaseConnection *db);

/// @brief Gets all users in the database.
/// @param db The database connection to be used.
/// @return Returns an array of users.
User *db_get_users(DatabaseConnection *db);

/// @brief Gets a user in the database by their CPF.
/// @param db The database connection to be used.
/// @param cpf The user CPF to be queried for.
/// @param user The user queried in the database.
/// @return Returns whether the operation was successful or not.
bool db_get_user(DatabaseConnection *db, char cpf[CPF_LENGTH], User *user);

/// @brief Inserts a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be inserted.
void db_insert_user(DatabaseConnection *db, User *user);

/// @brief Disables a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be disabled.
void db_disable_user(DatabaseConnection *db, User *user);

/// @brief Deletes a user from the database.
/// @param db The database connection to be used.
/// @param user The user to be deleted.
void db_delete_user(DatabaseConnection *db, User *user);

/// @brief Gets all orders from the user.
/// @param db The database connection to be used.
/// @param user The user to get the orders from.
/// @return Returns all the user's orders
Order *db_get_orders(DatabaseConnection *db, User *user);

/// @brief Inserts an order for the given user.
/// @param db The database connection to be used.
/// @param user The user to be attributed the order.
/// @param order The order to be inserted.
void db_insert_order(DatabaseConnection *db, User *user, Order *order);

#endif
