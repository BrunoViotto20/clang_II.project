#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdbool.h>

#include "lanchonete.h"
#include "utils.h"

/// @brief Represents a connection to the database, required to access and manipulate it.
typedef struct Database Database;

/// @brief Represents a result which may return a Database or an error.
typedef struct
{
    bool is_success;
    union
    {
        Database *database;
        Error error;
    };
} DatabaseResult;

/// @brief Creates a database success.
/// @param database The successfully created database.
/// @return Returns a successful database result.
DatabaseResult make_database_success(Database *database);

/// @brief Creates a database failure.
/// @param message The error message.
/// @return Returns a failure result.
DatabaseResult make_database_failure(char *message);

/// @brief Opens a connection to the database.
/// @param result The result of the database open operation.
DatabaseResult db_open();

/// @brief Closes the connection to the database.
/// @param db The database connection to be closed.
/// @param result The result of the database close operation.
UnitResult db_close(Database *db);

/// @brief Gets all users in the database.
/// @param db The database connection to be used.
/// @param result The result of the get users operation.
UsersResult db_get_users(Database *db);

/// @brief Gets a user in the database by their CPF.
/// @param db The database connection to be used.
/// @param cpf The user CPF to be queried for.
/// @param result The result of the get user operation.
UserResult db_get_user(Database *db, char cpf[CPF_LENGTH + 1]);

/// @brief Inserts a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be inserted.
/// @param result The result of the insert user operation.
UnitResult db_insert_user(Database *db, User *user);

/// @brief Disables a user in the database.
/// @param db The database connection to be used.
/// @param cpf The cpf of the user to be disabled.
/// @param result The result of the disable user operation.
UnitResult db_disable_user(Database *db, char cpf[CPF_LENGTH]);

/// @brief Deletes a user from the database.
/// @param db The database connection to be used.
/// @param cpf The cpf of the user to be deleted.
/// @param result The result of the delete user operation.
UnitResult db_delete_user(Database *db, char cpf[CPF_LENGTH]);

/// @brief Gets all orders from the user.
/// @param db The database connection to be used.
/// @param user The user to get the orders from.
/// @param result The result of the get orders operation.
OrdersResult db_get_orders(Database *db, char cpf[CPF_LENGTH + 1]);

/// @brief Inserts an order for the given user.
/// @param db The database connection to be used.
/// @param user The user to be attributed the order.
/// @param order The order to be inserted.
/// @param result The result of the insert order operation.
UnitResult db_insert_order(Database *db, char cpf[CPF_LENGTH + 1], Order *order);

#endif
