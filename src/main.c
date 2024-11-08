#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "console.h"
#include "lanchonete.h"

int get_age();
int get_amount();

int main(void)
{
    const Product products[] = {
        {"Cachorro Quente", 12, false},
        {"X-Salada", 15.5, false},
        {"X-Bacon", 18, false},
        {"Torrada Simples", 8, false},
        {"Refrigerante", 4.5, false},
        {"Cerveja Brahma", 6, true},
        {"Cigarro Malboro", 6, true}};
    const int products_length = sizeof(products) / sizeof(Product);

    const PaymentMethod payment_methods[] = {
        {"Crédito", .02},
        {"Débito", .02},
        {"Dinheiro", 0}};
    const int payment_methods_length = sizeof(payment_methods) / sizeof(PaymentMethod);

    int age = get_age();

    Product product = products_menu(products, products_length, age);

    int amount = get_amount();

    PaymentMethod payment_method = payment_menu(payment_methods, payment_methods_length);

    if (payment_method.fee != 0)
    {
        printf("Taxa de %.1lf%%, sobre o valor da compra:\n", payment_method.fee * 100);
    }

    double total_price = amount * product.price;
    double final_price = total_price * (1 + payment_method.fee);
    printf("O valor total é: R$%.2f\n", final_price);

    return 0;
}

int get_age()
{
    while (true)
    {
        int age;
        printf("Digite a sua idade: ");
        scanf("%d", &age);
        if (age <= 0)
        {
            clear_console();
            printf("Idade inválida! Tente novamente\n\n");
            wait_key_press();
            clear_console();
            continue;
        }

        return age;
    }
}

int get_amount()
{
    while (true)
    {
        int amount;
        printf("Digite a quantidade do item: ");
        scanf("%d", &amount);

        if (amount <= 0)
        {
            clear_console();
            printf("Quantidade inválida! Tente novamente\n\n");
            wait_key_press();
            clear_console();
            continue;
        }

        return amount;
    }
}
