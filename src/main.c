#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Product Product;
typedef struct PaymentMethod PaymentMethod;

struct Product
{
    char name[32];
    double price;
    bool adult;
};

struct PaymentMethod
{
    char name[32];
    double fee;
};

void clear_console();
void wait_key_press();
int get_age();
Product products_menu(const Product *products, int length, int age);
PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length);

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

    printf("Digite a quantidade do item: ");
    int amount;
    scanf("%d", &amount);

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

void clear_console()
{
#if _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void wait_key_press()
{
    printf("Press any key to Continue\n");
    getchar();
    getchar();
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
        scanf("%d", &option);

        if (option <= 0 || option > length)
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
        scanf("%d", &option);

        if (option <= 0 || option > length)
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
