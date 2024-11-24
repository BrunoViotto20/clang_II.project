#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "console.h"
#include "database.h"
#include "lanchonete.h"
#include "result.h"

PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length);
Product products_menu(const Product *products, int length, int age);
void get_cpf(char *cpf);
int get_age();
int get_amount();
void fazer_pedido();
void cadastrar_usuario(Database *connection);
void delete_user(Database *connection);
void disable_user(Database *connection);
bool is_cpf(char *string);

bool menu_principal(Database *connection)
{
    int op;
    printf("[ 1 ] Cadastrar\n");
    printf("[ 2 ] Fazer Pedido\n");
    printf("[ 3 ] Desabilitar Usuário\n");
    printf("[ 4 ] Deletar Usuário\n");
    printf("[ 5 ] Listar Usuários\n");
    printf("[ 6 ] Listar Pedidos do Usuário\n");
    printf("[ 7 ] Sair\n");

    printf("Digite a opção desejada: ");
    int result = scanf("%d", &op);
    if (result != 1)
    {
        printf("Opção inválida! tente novamente...");
        return false;
    }

    switch (op)
    {
    case 7:
        return true;
    case 1:
        cadastrar_usuario(connection);
        break;
    case 2:
        fazer_pedido();
        break;
    case 3:
        disable_user(connection);
        break;
    case 4:
        delete_user(connection);
        break;
    }

    return false;
}

void cadastrar_usuario(Database *connection)
{
    User user;
    user.active = true;

    printf("\n");
    get_cpf(user.cpf);
    user.age = get_age();

    // This scanf clears the stdin buffer
    int _ = scanf("%*c");
    (void)_;

    printf("Digite o seu nome: ");
    char *buffer = fgets(user.name, USER_NAME_LENGTH + 1, stdin);
    if (buffer == NULL)
    {
        perror("Falha ao ler o nome do usuario\n");
        return;
    }
    int name_length = strlen(user.name);
    if (user.name[name_length - 1] == '\n')
    {
        user.name[name_length - 1] = '\0';
    }

    UnitResult user_insert = db_insert_user(connection, &user);
    if (!user_insert.is_success)
    {
        perror(user_insert.error.message);
        return;
    }
}
void fazer_pedido()
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
    // pedir cpf
    // db.get.user

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

        if (product.is_adult && age < 18)
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

void get_cpf(char *cpf)
{
    while (true)
    {
        int _ = scanf("%*c");
        (void)_;

        printf("Digite o seu cpf: ");
        char *buffer = fgets(cpf, CPF_LENGTH + 1, stdin);

        if (buffer == NULL || !is_cpf(cpf))
        {
            clear_console();
            printf("cpf inválido! Tente novamente\n\n");
            wait_key_press();
            clear_console();
            continue;
        }
        break;
    }
}
bool is_cpf(char *string)
{

    return true;
}
int get_age()
{
    while (true)
    {
        int age;
        printf("Digite a sua idade: ");
        int result = scanf("%d", &age);

        if (result != 1 || age <= 0)
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
        int result = scanf("%d", &amount);

        if (result != 1 || amount <= 0)
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

void delete_user(Database *connection)
{
    char cpf[CPF_LENGTH + 1] = {0};
    get_cpf(cpf);

    UnitResult user_delete_result = db_delete_user(connection, cpf);

    if (!user_delete_result.is_success)
    {
        perror(user_delete_result.error.message);
        return;
    }
}

void disable_user(Database *connection)
{
    char cpf[CPF_LENGTH + 1] = {0};
    get_cpf(cpf);

    UnitResult user_desable_result = db_disable_user(connection, cpf);

    if (!user_desable_result.is_success)
    {
        perror(user_desable_result.error.message);
        return;
    }
}
