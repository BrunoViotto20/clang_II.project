#ifndef __RESULT_H__
#define __RESULT_H__

#include "lanchonete.h"

struct Database;

typedef struct Error Error;
typedef struct UnitResult UnitResult;
typedef struct I64Result I64Result;
typedef struct DatabaseResult DatabaseResult;
typedef struct UserResult UserResult;
typedef struct UsersResult UsersResult;
typedef struct OrderResult OrderResult;
typedef struct OrdersResult OrdersResult;

/// @brief Represents an application error.
struct Error
{
    /// @brief The message of the error.
    char *message;
};

/// @brief Represents a result which may succeed or return an error.
struct UnitResult
{
    bool is_success;
    Error error;
};

/// @brief Represents a result which may succeed with an i64 value or fail with an error.
struct I64Result
{
    bool is_success;
    union
    {
        long value;
        Error error;
    };
};

/// @brief Represents a result which may return a Database or an error.
struct DatabaseResult
{
    bool is_success;
    union
    {
        Database *database;
        Error error;
    };
};

/// @brief Represents a result which may return a user or an error.
struct UserResult
{
    bool is_success;
    union
    {
        User user;
        Error error;
    };
};

/// @brief Represents a result which may return a collection of users or an error.
struct UsersResult
{
    bool is_success;
    union
    {
        Users users;
        Error error;
    };
};

/// @brief Represents a result which may return a collection of orders or an error.
struct OrderResult
{
    bool is_success;
    union
    {
        Order order;
        Error error;
    };
};

/// @brief Represents a result which may return a collection of orders or an error.
struct OrdersResult
{
    bool is_success;
    union
    {
        Orders orders;
        Error error;
    };
};

UnitResult make_unit_success();
UnitResult make_unit_failure(char *message);

I64Result make_i64_success(long number);
I64Result make_i64_failure(char *message);

DatabaseResult make_database_success(Database *database);
DatabaseResult make_database_failure(char *message);

UserResult make_user_success(User user);
UserResult make_user_failure(char *message);

UsersResult make_users_success(Users users);
UsersResult make_users_failure(char *message);

OrderResult make_order_success(Order order);
OrderResult make_order_failure(char *message);

OrdersResult make_orders_success(Orders orders);
OrdersResult make_orders_failure(char *message);

#endif
