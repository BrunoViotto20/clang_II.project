#ifndef __LANCHONETE_H__
#define __LANCHONETE_H__

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

Product products_menu(const Product *products, int length, int age);
PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length);

#endif
