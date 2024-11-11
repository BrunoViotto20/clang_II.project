#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "lanchonete.h"

/// @brief Represents a connection to the database, required to access and manipulate it.
typedef struct Database Database;

typedef struct Users
{
    User *values;
    long length;
} Users;

typedef struct Orders
{
    Order *values;
    long length;
} Orders;

typedef struct Error
{
    char *value;
} Error;

typedef struct UnitResult UnitResult;

typedef struct DatabaseResult DatabaseResult;

typedef struct UsersResult UsersResult;

typedef struct UserResult UserResult;

typedef struct OrdersResult OrdersResult;

bool is_unit_success(UnitResult *result);
bool is_database_success(DatabaseResult *result);
bool is_users_success(UsersResult *result);
bool is_user_success(UserResult *result);
bool is_orders_success(OrdersResult *result);

Database *get_database_value(DatabaseResult *result);
Users get_users_value(UsersResult *result);
User get_user_value(UserResult *result);
Orders get_orders_value(OrdersResult *result);

Error get_unit_error(UnitResult *result);
Error get_database_error(DatabaseResult *result);
Error get_users_error(UsersResult *result);
Error get_user_error(UserResult *result);
Error get_orders_error(OrdersResult *result);

/// @brief Opens a connection to the database.
/// @return Returns a pointer to the database connection.
void db_open(DatabaseResult *result);

/// @brief Closes the connection to the database.
/// @param db The database connection to be closed.
void db_close(Database *db, UnitResult *result);

/// @brief Gets all users in the database.
/// @param db The database connection to be used.
/// @return Returns an array of users.
void db_get_users(Database *db, UsersResult *result);

/// @brief Gets a user in the database by their CPF.
/// @param db The database connection to be used.
/// @param cpf The user CPF to be queried for.
/// @param user The user queried in the database.
/// @return Returns whether the operation was successful or not.
void db_get_user(Database *db, char cpf[CPF_LENGTH], UserResult *result);

/// @brief Inserts a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be inserted.
void db_insert_user(Database *db, User *user, UnitResult *result);

/// @brief Disables a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be disabled.
void db_disable_user(Database *db, User *user, UnitResult *result);

/// @brief Deletes a user from the database.
/// @param db The database connection to be used.
/// @param user The user to be deleted.
void db_delete_user(Database *db, User *user, UnitResult *result);

/// @brief Gets all orders from the user.
/// @param db The database connection to be used.
/// @param user The user to get the orders from.
/// @return Returns all the user's orders
void db_get_orders(Database *db, User *user, OrdersResult *result);

/// @brief Inserts an order for the given user.
/// @param db The database connection to be used.
/// @param user The user to be attributed the order.
/// @param order The order to be inserted.
void db_insert_order(Database *db, User *user, Order *order, UnitResult *result);

#endif
