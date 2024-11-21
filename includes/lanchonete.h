#ifndef __LANCHONETE_H__
#define __LANCHONETE_H__

#include <stdbool.h>
#include "utils.h"

#define USER_NAME_LENGTH 64
#define CPF_LENGTH 11

typedef struct Database Database;

typedef struct User
{
    long id;
    char name[USER_NAME_LENGTH + 1];
    char cpf[CPF_LENGTH + 1];
    int age;
    bool active;
} User;

typedef struct Product
{
    char name[32];
    double price;
    bool adult;
} Product;

typedef struct PaymentMethod
{
    char name[32];
    double fee;
} PaymentMethod;

typedef struct Order
{
    Product product;
    PaymentMethod payment_method;
} Order;

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

/// @brief Represents a result which may return a user or an error.
typedef struct
{
    bool is_success;
    union
    {
        User user;
        Error error;
    };
} UserResult;

/// @brief Creates a user success.
/// @param user The successfully created user.
/// @return Returns a successful user result.
UserResult make_user_success(User user);

/// @brief Creates a user failure.
/// @param message The error message.
/// @return Returns a failure result.
UserResult make_user_failure(char *message);

/// @brief Represents a result which may return a collection of users or an error.
typedef struct
{
    bool is_success;
    union
    {
        Users users;
        Error error;
    };
} UsersResult;

/// @brief Creates a users success.
/// @param users The successfully created users.
/// @return Returns a successful users result.
UsersResult make_users_success(Users users);

/// @brief Creates a users failure.
/// @param message The error message.
/// @return Returns a failure result.
UsersResult make_users_failure(char *message);

/// @brief Represents a result which may return a collection of orders or an error.
typedef struct
{
    bool is_success;
    union
    {
        Orders orders;
        Error error;
    };
} OrdersResult;

/// @brief Creates an orders success.
/// @param orders The successfully created orders.
/// @return Returns a successful orders result.
OrdersResult make_orders_success(Orders orders);

/// @brief Creates an orders failure.
/// @param message The error message.
/// @return Returns a failure result.
OrdersResult make_orders_failure(char *message);

bool menu_principal(Database *connection);

#endif
