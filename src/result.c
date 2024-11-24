#include <stdlib.h>

#include "result.h"

UnitResult make_unit_success()
{
    UnitResult result = {true, {NULL}};
    return result;
}

UnitResult make_unit_failure(char *message)
{
    Error error = {message};
    UnitResult result = {false};
    result.error = error;
    return result;
}

I64Result make_i64_success(long number)
{
    I64Result result = {true, {number}};
    return result;
}

I64Result make_i64_failure(char *message)
{
    Error error = {message};
    I64Result result = {false};
    result.error = error;
    return result;
}

DatabaseResult make_database_success(Database *database)
{
    DatabaseResult result = {true, {database}};
    return result;
}

DatabaseResult make_database_failure(char *message)
{
    Error error = {message};
    DatabaseResult result = {false};
    result.error = error;
    return result;
}

UserResult make_user_success(User user)
{
    UserResult result = {true, {user}};
    return result;
}

UserResult make_user_failure(char *message)
{
    Error error = {message};
    UserResult result = {false};
    result.error = error;
    return result;
}

UsersResult make_users_success(Users users)
{
    UsersResult result = {true, {users}};
    return result;
}

UsersResult make_users_failure(char *message)
{
    Error error = {message};
    UsersResult result = {false};
    result.error = error;
    return result;
}

OrderResult make_order_success(Order order)
{
    OrderResult result = {true, {order}};
    return result;
}

OrderResult make_order_failure(char *message)
{
    Error error = {message};
    OrderResult result = {false};
    result.error = error;
    return result;
}

OrdersResult make_orders_success(Orders orders)
{
    OrdersResult result = {true, {orders}};
    return result;
}

OrdersResult make_orders_failure(char *message)
{
    Error error = {message};
    OrdersResult result = {false};
    result.error = error;
    return result;
}
