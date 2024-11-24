#ifndef __LANCHONETE_H__
#define __LANCHONETE_H__

#include <stdbool.h>
#include "constants.h"

typedef struct Database Database;

typedef struct User User;
typedef struct Product Product;
typedef struct PaymentMethod PaymentMethod;
typedef struct Order Order;

typedef struct Users Users;
typedef struct Orders Orders;

struct User
{
    long id;
    char name[USER_NAME_LENGTH + 1];
    char cpf[CPF_LENGTH + 1];
    int age;
    bool active;
};

struct Product
{
    char name[PRODUCT_NAME_LENGTH + 1];
    double price;
    bool is_adult;
};

struct PaymentMethod
{
    char name[PAYMENT_METHOD_NAME_LENGTH + 1];
    double fee;
};

struct Order
{
    Product product;
    PaymentMethod payment_method;
    long user_id;
};

/// @brief Represents a finite collection of users.
struct Users
{
    /// @brief The users of the collection.
    User *values;

    /// @brief The length of the collection.
    long length;
};

/// @brief Respresents a finite collection of orders.
struct Orders
{
    /// @brief The orders of the collection.
    Order *values;

    /// @brief The length of the collection.
    long length;
};

bool menu_principal(Database *connection);

#endif
