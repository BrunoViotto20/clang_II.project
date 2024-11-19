#include <stdbool.h>
#include <stdio.h>

#include "console.h"
#include "lanchonete.h"

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

Product products_menu(const Product *products, int length, int age)
{
    while (true)
    {
        printf("MENU DE PEDIDOS:\n\n");
        for (int i = 0; i < length; i++)
        {
            Product product = products[i];
            printf("[ %d ] %s R$%.2lf\n", i + 1, product.name, product.price);
        }
        printf("\n");

        int option;
        printf("Selecione o produto: ");
        int result = scanf("%d", &option);

        if (result != 1 || option <= 0 || option > length)
        {
            clear_console();
            printf("Produto inválido! Tente novamente\n\n");
            wait_key_press();
            clear_console();
            continue;
        }

        Product product = products[option - 1];

        if (product.adult && age < 18)
        {
            clear_console();
            printf("Venda proibida para menores de 18 anos!\n");
            wait_key_press();
            clear_console();
            continue;
        }

        return product;
    }
}

PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length)
{
    while (true)
    {
        printf("MENU DE PAGAMENTO:\n\n");
        for (int i = 0; i < length; i++)
        {
            PaymentMethod payment_method = payment_methods[i];
            printf("[ %d ] %s", i + 1, payment_method.name);

            if (payment_method.fee != 0)
            {
                printf(" (+%.1lf%%)\n", payment_method.fee * 100);
            }
        }
        printf("\n");

        int option;
        printf("Selecione a forma de pagamento: ");
        int result = scanf("%d", &option);

        if (result != 1 || option <= 0 || option > length)
        {
            clear_console();
            printf("Forma de pagamento inválida! Tente novamente\n\n");
            wait_key_press();
            clear_console();
            continue;
        }

        return payment_methods[option - 1];
    }
}
