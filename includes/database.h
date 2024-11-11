#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "lanchonete.h"

/// @brief Represents a connection to the database, required to access and manipulate it.
typedef struct Database Database;

/// @brief Represents a finite collection of users.
typedef struct Users
{
    /// @brief The users of the collection.
    User *values;

    /// @brief The length of the collection.
    long length;
} Users;

/// @brief Respresents a finite collection of orders.
typedef struct Orders
{
    /// @brief The orders of the collection.
    Order *values;

    /// @brief The length of the collection.
    long length;
} Orders;

/// @brief Represents an application error.
typedef struct Error
{
    /// @brief The message of the error.
    char *message;
} Error;

/// @brief Represents a result which may succeed or return an error.
typedef struct UnitResult UnitResult;

/// @brief Represents a result which may return a Database or an error.
typedef struct DatabaseResult DatabaseResult;

/// @brief Represents a result which may return a collection of users or an error.
typedef struct UsersResult UsersResult;

/// @brief Represents a result which may return a user or an error.
typedef struct UserResult UserResult;

/// @brief Represents a result which may return a collection of orders or an error.
typedef struct OrdersResult OrdersResult;

/// @brief Returns if the operation result was successful.
/// @param result The operation reuslt.
/// @return Returns true if the operation was successful, false otherwise.
bool is_unit_success(UnitResult *result);

/// @brief Returns if the operation result was successful.
/// @param result The operation reuslt.
/// @return Returns true if the operation was successful, false otherwise.
bool is_database_success(DatabaseResult *result);

/// @brief Returns if the operation result was successful.
/// @param result The operation reuslt.
/// @return Returns true if the operation was successful, false otherwise.
bool is_users_success(UsersResult *result);

/// @brief Returns if the operation result was successful.
/// @param result The operation reuslt.
/// @return Returns true if the operation was successful, false otherwise.
bool is_user_success(UserResult *result);

/// @brief Returns if the operation result was successful.
/// @param result The operation reuslt.
/// @return Returns true if the operation was successful, false otherwise.
bool is_orders_success(OrdersResult *result);

/// @brief Gets the Database from the result.
/// @param result The operation result.
/// @return Returns the Database.
Database *get_database_value(DatabaseResult *result);

/// @brief Gets the Users from the result.
/// @param result The operation result.
/// @return Returns the Users.
Users get_users_value(UsersResult *result);

/// @brief Gets the User from the result.
/// @param result The operation result.
/// @return Returns the User.
User get_user_value(UserResult *result);

/// @brief Gets the Orders from the result.
/// @param result The operation result.
/// @return Returns the Orders.
Orders get_orders_value(OrdersResult *result);

/// @brief Gets the result error.
/// @param result The operation result.
/// @return Returns the error.
Error get_unit_error(UnitResult *result);

/// @brief Gets the result error.
/// @param result The operation result.
/// @return Returns the error.
Error get_database_error(DatabaseResult *result);

/// @brief Gets the result error.
/// @param result The operation result.
/// @return Returns the error.
Error get_users_error(UsersResult *result);

/// @brief Gets the result error.
/// @param result The operation result.
/// @return Returns the error.
Error get_user_error(UserResult *result);

/// @brief Gets the result error.
/// @param result The operation result.
/// @return Returns the error.
Error get_orders_error(OrdersResult *result);

/// @brief Opens a connection to the database.
/// @param result The result of the database open operation.
void db_open(DatabaseResult *result);

/// @brief Closes the connection to the database.
/// @param db The database connection to be closed.
/// @param result The result of the database close operation.
void db_close(Database *db, UnitResult *result);

/// @brief Gets all users in the database.
/// @param db The database connection to be used.
/// @param result The result of the get users operation.
void db_get_users(Database *db, UsersResult *result);

/// @brief Gets a user in the database by their CPF.
/// @param db The database connection to be used.
/// @param cpf The user CPF to be queried for.
/// @param result The result of the get user operation.
void db_get_user(Database *db, char cpf[CPF_LENGTH], UserResult *result);

/// @brief Inserts a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be inserted.
/// @param result The result of the insert user operation.
void db_insert_user(Database *db, User *user, UnitResult *result);

/// @brief Disables a user in the database.
/// @param db The database connection to be used.
/// @param user The user to be disabled.
/// @param result The result of the disable user operation.
void db_disable_user(Database *db, User *user, UnitResult *result);

/// @brief Deletes a user from the database.
/// @param db The database connection to be used.
/// @param user The user to be deleted.
/// @param result The result of the delete user operation.
void db_delete_user(Database *db, User *user, UnitResult *result);

/// @brief Gets all orders from the user.
/// @param db The database connection to be used.
/// @param user The user to get the orders from.
/// @param result The result of the get orders operation.
void db_get_orders(Database *db, User *user, OrdersResult *result);

/// @brief Inserts an order for the given user.
/// @param db The database connection to be used.
/// @param user The user to be attributed the order.
/// @param order The order to be inserted.
/// @param result The result of the insert order operation.
void db_insert_order(Database *db, User *user, Order *order, UnitResult *result);

#endif
