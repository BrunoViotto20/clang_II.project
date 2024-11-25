#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console.h"
#include "database.h"
#include "lanchonete.h"
#include "result.h"

PaymentMethod payment_menu(const PaymentMethod *payment_methods, int length);
Product products_menu(const Product *products, int length, User *user);
void get_cpf(char cpf[CPF_LENGTH + 1]);
int get_age();
int get_amount();
void fazer_pedido(Database *connection);
void cadastrar_usuario(Database *connection);
void delete_user(Database *connection);
void disable_user(Database *connection);
void list_users(Database *connection);
void list_orders(Database *connection);
bool is_cpf(char string[CPF_LENGTH + 1]);
long sort_user_age(void *buffer, long length);
long sort_user_name(void *buffer, long length);
long sort_product_name(void *buffer, long length);
long sort_product_price(void *buffer, long length);
long sort_product_amount(void *buffer, long length);
void quick_sort(void *array, int size, long length, long (*f)(void *, long length));
void print_user(User user);
void print_order(Order order);

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
        fazer_pedido(connection);
        break;
    case 3:
        disable_user(connection);
        break;
    case 4:
        delete_user(connection);
        break;
    case 5:
        list_users(connection);
        break;
    case 6:
        list_orders(connection);
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

void fazer_pedido(Database *connection)
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

    char cpf[CPF_LENGTH + 1];
    get_cpf(cpf);

    UserResult get_user_result = db_get_user(connection, cpf);
    if (!get_user_result.is_success)
    {
        perror(get_user_result.error.message);
        return;
    }
    User user = get_user_result.user;

    Product product = products_menu(products, products_length, &user);

    int amount = get_amount();

    PaymentMethod payment_method = payment_menu(payment_methods, payment_methods_length);

    if (payment_method.fee != 0)
    {
        printf("Taxa de %.1lf%%, sobre o valor da compra\n", payment_method.fee * 100);
    }

    double total_price = amount * product.price;
    double final_price = total_price * (1 + payment_method.fee);
    printf("O valor total é de R$%.2f\n", final_price);

    int _ = scanf("%*c");
    (void)_;

    printf("Confirmar pedido? [S/N] ");
    char confirmation;
    int matches = scanf("%c", &confirmation);

    if (matches != 1)
    {
        printf("Confirmação inválida! Cancelando pedido...\n");
        wait_key_press();
        return;
    }

    Order order = {
        amount,
        product,
        payment_method,
        user.id};

    if (confirmation == 'S' || confirmation == 's')
    {
        UnitResult insert_order_result = db_insert_order(connection, cpf, &order);
        if (!insert_order_result.is_success)
        {
            perror(insert_order_result.error.message);
            return;
        }
        printf("Pedido realizado com sucesso!\n");
        wait_key_press();
        return;
    }

    if (confirmation == 'N' || confirmation == 'n')
    {
        printf("Pedido cancelado\n");
        wait_key_press();
        return;
    }

    printf("Confirmação inválida! Cancelando pedido...\n");
    wait_key_press();
}

void list_users(Database *connection)
{
    UsersResult get_users_result = db_get_users(connection);
    if (!get_users_result.is_success)
    {
        perror(get_users_result.error.message);
        return;
    }
    Users users = get_users_result.users;

    printf("[ 1 ] Nome\n");
    printf("[ 2 ] Idade\n");
    printf("Escolha o método de ordenação: ");
    int option;
    int matches = scanf("%d", &option);
    if (matches != 1)
    {
        free(users.values);
        printf("Método de ordenação inválido! Tente novamente");
        return;
    }

    switch (option)
    {
    case 1:
        quick_sort(users.values, sizeof(User), users.length, sort_user_name);
        break;
    case 2:
        quick_sort(users.values, sizeof(User), users.length, sort_user_age);
        break;
    default:
        printf("Método de ordenação inválido!");
        break;
    }

    printf("\n");
    for (int i = 0; i < users.length; i++)
    {
        printf("CLIENTE %d\n", i + 1);
        print_user(users.values[i]);
        printf("\n");
    }

    free(users.values);
    wait_key_press();
}

void list_orders(Database *connection)
{
    char cpf[CPF_LENGTH + 1] = {0};
    get_cpf(cpf);

    OrdersResult get_orders_result = db_get_orders(connection, cpf);
    if (!get_orders_result.is_success)
    {
        perror(get_orders_result.error.message);
        return;
    }
    Orders orders = get_orders_result.orders;

    printf("[ 1 ] Produto\n");
    printf("[ 2 ] Preço\n");
    printf("[ 3 ] Quantidade\n");
    printf("Escolha o método de ordenação: ");
    int option;
    int matches = scanf("%d", &option);
    if (matches != 1)
    {
        free(orders.values);
        printf("Método de ordenação inválido! Tente novamente");
        return;
    }

    switch (option)
    {
    case 1:
        quick_sort(orders.values, sizeof(Order), orders.length, sort_product_name);
        break;
    case 2:
        quick_sort(orders.values, sizeof(Order), orders.length, sort_product_price);
        break;
    case 3:
        quick_sort(orders.values, sizeof(Order), orders.length, sort_product_amount);
        break;
    default:
        printf("Método de ordenação inválido!");
        break;
    }

    printf("\n");
    for (int i = 0; i < orders.length; i++)
    {
        printf("CLIENTE %d\n", i + 1);
        print_order(orders.values[i]);
        printf("\n");
    }

    free(orders.values);
    wait_key_press();
}

Product products_menu(const Product *products, int length, User *user)
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

        if (product.is_adult && user->age < 18)
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

void get_cpf(char cpf[CPF_LENGTH + 1])
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

bool is_cpf(char string[CPF_LENGTH + 1])
{
#if DEBUG
    return true;
#endif

    int soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (10 - i) * (string[i] - '0');
    }

    int resto = soma % 11;
    if (resto < 2)
    {
        resto = 0;
    }
    else
    {
        resto = 11 - resto;
    }

    if (resto != string[9] - '0')
    {
        return false;
    }

    soma = 0;
    for (int i = 0; i < 10; i++)
    {
        soma += (11 - i) * (string[i] - '0');
    }

    resto = soma % 11;
    if (resto < 2)
    {
        resto = 0;
    }
    else
    {
        resto = 11 - resto;
    }

    if (resto != string[10] - '0')
    {
        return false;
    }

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

long sort_user_name(void *buffer, long length)
{
    User *array = (User *)buffer;
    User pivot = array[length - 1];
    long low = 0;

    for (long i = 0; i < length - 1; i++)
    {
        if (strcmp(array[i].name, pivot.name) >= 0)
        {
            continue;
        }

        User temp = array[i];
        array[i] = array[low];
        array[low] = temp;
        low++;
    }

    User temp = array[low];
    array[low] = pivot;
    array[length - 1] = temp;
    return low;
}

long sort_user_age(void *buffer, long length)
{
    User *array = (User *)buffer;
    User pivot = array[length - 1];
    long low = 0;

    for (long i = 0; i < length - 1; i++)
    {
        if (array[i].age >= pivot.age)
        {
            continue;
        }

        User temp = array[i];
        array[i] = array[low];
        array[low] = temp;
        low++;
    }

    User temp = array[low];
    array[low] = pivot;
    array[length - 1] = temp;
    return low;
}

long sort_product_amount(void *buffer, long length)
{
    Order *array = (Order *)buffer;
    Order pivot = array[length - 1];
    long low = 0;

    for (long i = 0; i < length - 1; i++)
    {
        if (array[i].amount >= pivot.amount)
        {
            continue;
        }

        Order temp = array[i];
        array[i] = array[low];
        array[low] = temp;
        low++;
    }

    Order temp = array[low];
    array[low] = pivot;
    array[length - 1] = temp;
    return low;
}

long sort_product_price(void *buffer, long length)
{
    Order *array = (Order *)buffer;
    Order pivot = array[length - 1];
    long low = 0;

    for (long i = 0; i < length - 1; i++)
    {
        if (array[i].product.price >= pivot.product.price)
        {
            continue;
        }

        Order temp = array[i];
        array[i] = array[low];
        array[low] = temp;
        low++;
    }

    Order temp = array[low];
    array[low] = pivot;
    array[length - 1] = temp;
    return low;
}

long sort_product_name(void *buffer, long length)
{
    Order *array = (Order *)buffer;
    Order pivot = array[length - 1];
    long low = 0;

    for (long i = 0; i < length - 1; i++)
    {
        if (strcmp(array[i].product.name, pivot.product.name) >= 0)
        {
            continue;
        }

        Order temp = array[i];
        array[i] = array[low];
        array[low] = temp;
        low++;
    }

    Order temp = array[low];
    array[low] = pivot;
    array[length - 1] = temp;
    return low;
}

void quick_sort(void *array, int size, long length, long (*partition_sort)(void *, long length))
{
    if (length <= 1)
    {
        return;
    }

    long index = partition_sort(array, length);

    quick_sort(array, size, index, partition_sort);
    quick_sort(array + (index + 1) * size, size, length - index - 1, partition_sort);
}

void print_user(User user)
{
    printf("Nome: %s\n", user.name);
    printf("CPF: %s\n", user.cpf);
    printf("Idade: %d\n", user.age);
}

void print_order(Order order)
{
    printf("Produto: %s\n", order.product.name);
    printf("Quantidade: %d\n", order.amount);
    printf("Preço: %lf\n", order.product.price);
    printf("Maior idade? %s\n", order.product.is_adult ? "sim" : "não");
    printf("Pagamento: %s\n", order.payment_method.name);
    printf("Taxa: %lf\n", order.payment_method.fee * 100);
}
