#ifndef __LANCHONETE_H__
#define __LANCHONETE_H__

#define USER_NAME_LENGTH 64
#define CPF_LENGTH 11

typedef struct User
{
    long id;
    char name[USER_NAME_LENGTH];
    char cpf[CPF_LENGTH];
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

Product products_menu(const Product *products, int length, int age);
PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length);

#endif
