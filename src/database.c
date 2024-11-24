#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "database.h"
#include "lanchonete.h"
#include "result.h"

#define USERS_PATH "database/users.txt"
#define ORDERS_PATH "database/orders.txt"
#define USERS_INDEX_PATH "database/users_index.txt"

#define LINE_BUFFER_LENGTH 256

struct Database
{
    FILE *users;
    FILE *users_index;
    FILE *orders;
};

UserResult parse_user(char *string);
OrderResult parse_order(char *string);

UnitResult write_user(Database *db, User *user);
UnitResult write_order(Database *db, Order *order);

UnitResult ensure_directory_exists(char *directory_path);

I64Result get_next_user_id(Database *db);

DatabaseResult db_open()
{
    Database *db = (Database *)malloc(sizeof(Database));
    if (db == NULL)
    {
        return make_database_failure("Falha ao alocar o handler do banco de dados\n");
    }

    UnitResult dir_result = ensure_directory_exists("database");
    if (!dir_result.is_success)
    {
        free(db);
        return make_database_failure("Falha ao criar o diretório do arquivo\n");
    }

    db->users = fopen(USERS_PATH, "a");
    fclose(db->users);
    db->users = fopen(USERS_PATH, "r+");
    if (db->users == NULL)
    {
        free(db);
        return make_database_failure("Falha ao abrir arquivo\n");
    }

    db->orders = fopen(ORDERS_PATH, "a");
    fclose(db->orders);
    db->orders = fopen(ORDERS_PATH, "r+");
    if (db->orders == NULL)
    {
        free(db->users);
        free(db);
        return make_database_failure("Falha ao abrir arquivo\n");
    }

    db->users_index = fopen(USERS_INDEX_PATH, "a");
    fclose(db->users_index);
    db->users_index = fopen(USERS_INDEX_PATH, "r+");
    if (db->users_index == NULL)
    {
        free(db->orders);
        free(db->users);
        free(db);
        return make_database_failure("Falha ao abrir arquivo\n");
    }

    return make_database_success(db);
}

UnitResult db_close(Database *db)
{
    fclose(db->users);
    fclose(db->orders);
    fclose(db->users_index);
    free(db);

    return make_unit_success();
}

UsersResult db_get_users(Database *db)
{
    rewind(db->users);

    // Allocates a buffer to retrieve the users
    int users_capacity = 8;
    long users_length = 0;
    User *users = (User *)malloc(sizeof(User) * users_capacity);
    if (users == NULL)
    {
        return make_users_failure("ERRO: Sem memória para alocar");
    }

    // Loop to get all users
    char line_buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        // Gets a line from the database
        char *eof = fgets(line_buffer, LINE_BUFFER_LENGTH, db->users);
        if (eof == NULL)
        {
            break;
        }

        // Grows the user buffer if it is full
        if (users_capacity <= users_length)
        {
            users_capacity *= 2;
            User *temp = realloc(users, sizeof(User) * users_capacity);
            if (temp == NULL)
            {
                free(users);
                return make_users_failure("ERRO: Sem memória para realocar");
            }
            users = temp;
        }

        // Tries to parse the user from the retrieved line and store it into the array
        UserResult parse_user_result = parse_user(line_buffer);
        if (!parse_user_result.is_success)
        {
            free(users);
            return make_users_failure(parse_user_result.error.message);
        }
        User user = parse_user_result.user;

        if (!user.active)
        {
            continue;
        }

        users[users_length] = user;
        users_length++;
    }

    // Shrinks the user buffer to its exact length
    if (users_capacity != users_length)
    {
        User *temp = realloc(users, sizeof(User) * users_length);
        if (users == NULL)
        {
            free(users);
            return make_users_failure("ERRO: Sem memória para realocar");
        }
        users = temp;
    }

    Users usrs = {users, users_length};
    return make_users_success(usrs);
}

UserResult db_get_user(Database *db, char cpf[CPF_LENGTH + 1])
{
    rewind(db->users);

    char buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        char *eof = fgets(buffer, LINE_BUFFER_LENGTH, db->users);
        if (eof == NULL)
        {
            return make_user_failure("Nenhum usuário encontrado!");
        }

        UserResult parse_user_result = parse_user(buffer);
        if (!parse_user_result.is_success)
        {
            return parse_user_result;
        }
        User user = parse_user_result.user;

        if (!user.active)
        {
            continue;
        }

        if (strcmp(cpf, user.cpf) != 0)
        {
            continue;
        }

        return make_user_success(user);
    }
}

UnitResult db_insert_user(Database *db, User *user)
{
    fseek(db->users, 0, SEEK_END);

    int pos = ftell(db->users);
    if (pos != 0)
    {
        fseek(db->users, -1, SEEK_END);
        char last = fgetc(db->users);
        if (last != '\n')
        {
            fwrite("\n", sizeof(char), 1, db->users);
        }
    }

    UserResult user_result = db_get_user(db, user->cpf);
    if (user_result.is_success)
    {
        return make_unit_failure("CPF já cadastrado no banco de dados\n");
    }

    I64Result nextIdResult = get_next_user_id(db);
    if (!nextIdResult.is_success)
    {
        return make_unit_failure(nextIdResult.error.message);
    }
    user->id = nextIdResult.value;

    UnitResult write_user_result = write_user(db, user);
    if (!write_user_result.is_success)
    {
        return write_user_result;
    }

    return make_unit_success();
}

UnitResult db_disable_user(Database *db, char cpf[CPF_LENGTH + 1])
{
    rewind(db->users);

    char buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        int record_start = ftell(db->users);
        char *eof = fgets(buffer, LINE_BUFFER_LENGTH, db->users);
        if (eof == NULL)
        {
            return make_unit_failure("Usuário não encontrado");
        }

        UserResult parse_user_result = parse_user(buffer);
        if (!parse_user_result.is_success)
        {
            return make_unit_failure(parse_user_result.error.message);
        }
        User user = parse_user_result.user;

        if (!user.active || strcmp(user.cpf, cpf) != 0)
        {
            continue;
        }

        user.active = false;

        int record_end = ftell(db->users);
        fseek(db->users, 0, SEEK_END);
        int file_size = ftell(db->users);

        fseek(db->users, record_end, SEEK_SET);
        int leftover = file_size - record_end;
        char *file_buffer = malloc(sizeof(char) * leftover);
        int read = fread(file_buffer, sizeof(char), leftover, db->users);
        if (read != leftover)
        {
            free(file_buffer);
            return make_unit_failure("ERRO: Falha ao atualizar arquivo");
        }
        fseek(db->users, record_start, SEEK_SET);

        write_user(db, &user);
        fwrite(file_buffer, sizeof(char), leftover, db->users);
        fflush(db->users);
        int current_size = ftell(db->users);
        free(file_buffer);

        if (current_size < file_size)
        {
            int shrink_result = ftruncate(fileno(db->users), current_size);

            if (shrink_result == -1)
            {
                return make_unit_failure("ERRO: Falha ao diminuir o tamanho do arquivo");
            }
        }

        return make_unit_success();
    }
}

UnitResult db_delete_user(Database *db, char cpf[CPF_LENGTH + 1])
{
    rewind(db->users);

    char buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        int record_start = ftell(db->users);
        char *eof = fgets(buffer, LINE_BUFFER_LENGTH, db->users);
        if (eof == NULL)
        {
            return make_unit_failure("Usuário não encontrado");
        }

        UserResult parse_user_result = parse_user(buffer);
        if (!parse_user_result.is_success)
        {
            return make_unit_failure(parse_user_result.error.message);
        }
        User user = parse_user_result.user;

        if (!user.active || strcmp(user.cpf, cpf) != 0)
        {
            continue;
        }

        int record_end = ftell(db->users);
        fseek(db->users, 0, SEEK_END);
        int file_size = ftell(db->users);

        fseek(db->users, record_end, SEEK_SET);
        int leftover = file_size - record_end;
        char *file_buffer = malloc(sizeof(char) * leftover);
        int read = fread(file_buffer, sizeof(char), leftover, db->users);
        if (read != leftover)
        {
            free(file_buffer);
            return make_unit_failure("Falha ao atualizar arquivo");
        }
        fseek(db->users, record_start, SEEK_SET);

        fwrite(file_buffer, sizeof(char), leftover, db->users);
        fflush(db->users);
        int current_size = ftell(db->users);
        free(file_buffer);

        if (current_size < file_size)
        {
            int shrink_result = ftruncate(fileno(db->users), current_size);

            if (shrink_result == -1)
            {
                return make_unit_failure("Falha ao diminuir o tamanho do arquivo");
            }
        }

        return make_unit_success();
    }
}

OrdersResult db_get_orders(Database *db, char cpf[CPF_LENGTH + 1])
{
    rewind(db->orders);

    UserResult get_user_result = db_get_user(db, cpf);
    if (!get_user_result.is_success)
    {
        return make_orders_failure(get_user_result.error.message);
    }
    User user = get_user_result.user;

    // Allocates a buffer to retrieve the orders
    int orders_capacity = 8;
    long orders_length = 0;
    Order *orders = (Order *)malloc(sizeof(Order) * orders_capacity);
    if (orders == NULL)
    {
        return make_orders_failure("ERRO: Sem memória para alocar");
    }

    // Loop to get all orders
    char line_buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        // Gets a line from the database
        char *eof = fgets(line_buffer, LINE_BUFFER_LENGTH, db->orders);
        if (eof == NULL)
        {
            break;
        }

        // Grows the orders buffer if it is full
        if (orders_capacity == orders_length)
        {
            orders_capacity *= 2;
            Order *temp = (Order *)realloc(orders, sizeof(Order) * orders_capacity);
            if (temp == NULL)
            {
                free(orders);
                return make_orders_failure("ERRO: Sem memória para realocar");
            }
            orders = temp;
        }

        // Tries to parse the user from the retrieved line and store it into the array
        OrderResult order_result = parse_order(line_buffer);
        if (!order_result.is_success)
        {
            free(orders);
            return make_orders_failure(order_result.error.message);
        }
        Order order = order_result.order;

        if (order.user_id != user.id)
        {
            continue;
        }

        orders[orders_length] = order;
        orders_length++;
    }

    // Shrinks the user buffer to its exact length
    if (orders_capacity != orders_length)
    {
        Order *temp = (Order *)realloc(orders, sizeof(Order) * orders_length);
        if (orders == NULL)
        {
            free(orders);
            return make_orders_failure("ERRO: Sem memória para realocar");
        }
        orders = temp;
    }

    Orders ordrs = {orders, orders_length};
    return make_orders_success(ordrs);
}

UnitResult db_insert_order(Database *db, char cpf[CPF_LENGTH + 1], Order *order)
{
    fseek(db->orders, 0, SEEK_END);

    int pos = ftell(db->orders);
    if (pos != 0)
    {
        fseek(db->orders, -1, SEEK_END);
        char last = fgetc(db->orders);
        if (last != '\n')
        {
            fwrite("\n", sizeof(char), 1, db->orders);
        }
    }

    UserResult get_user_result = db_get_user(db, cpf);
    if (!get_user_result.is_success)
    {
        return make_unit_failure(get_user_result.error.message);
    }

    User user = get_user_result.user;
    order->user_id = user.id;

    UnitResult write_order_result = write_order(db, order);
    if (!write_order_result.is_success)
    {
        return make_unit_failure(write_order_result.error.message);
    }

    return make_unit_success();
}

/* Private functions */

UserResult parse_user(char *string)
{
    User user;
    char is_active_buffer[6];

    int matches = sscanf(
        string,
        "%ld;\"%[^\"]\";\"%[^\"]\";%d;%[^\n]",
        &user.id,
        user.name,
        user.cpf,
        &user.age,
        is_active_buffer);

    if (matches != 5)
    {
        return make_user_failure("ERRO: Banco de dados corrompido");
    }

    if (strcmp(is_active_buffer, "true") == 0)
    {
        user.active = true;
    }
    else if (strcmp(is_active_buffer, "false") == 0)
    {
        user.active = false;
    }
    else
    {
        return make_user_failure("ERRO: Banco de dados corrompido");
    }

    return make_user_success(user);
}

OrderResult parse_order(char *string)
{
    char is_adult_buffer[6];

    Order order;
    int matches = sscanf(
        string,
        "\"%[^\"]\";%lf;\"%[^\"]\";%lf;%[^;];%ld",
        order.payment_method.name,
        &order.payment_method.fee,
        order.product.name,
        &order.product.price,
        is_adult_buffer,
        &order.user_id);

    if (matches != 6)
    {
        return make_order_failure("ERRO: Banco de dados corrompido");
    }

    if (strcmp(is_adult_buffer, "true") == 0)
    {
        order.product.is_adult = true;
    }
    else if (strcmp(is_adult_buffer, "false") == 0)
    {
        order.product.is_adult = false;
    }
    else
    {
        return make_order_failure("ERRO: Banco de dados corrompido");
    }

    return make_order_success(order);
}

UnitResult write_user(Database *db, User *user)
{
    fprintf(
        db->users,
        "%ld;\"%s\";\"%s\";%d;%s\n",
        user->id,
        user->name,
        user->cpf,
        user->age,
        user->active ? "true" : "false");

    fflush(db->users);

    return make_unit_success();
}

UnitResult write_order(Database *db, Order *order)
{
    fprintf(db->orders, "\"%s\";%lf;\"%s\";%lf;%s;%ld\n",
            order->payment_method.name,
            order->payment_method.fee,
            order->product.name,
            order->product.price,
            order->product.is_adult ? "true" : "false",
            order->user_id);

    fflush(db->orders);

    return make_unit_success();
}

UnitResult ensure_directory_exists(char *directory_path)
{
    // If directory exists, return
    if (access(directory_path, F_OK) != -1)
    {
        return make_unit_success();
    }

    int mkdir_result = mkdir(directory_path, S_IRWXG | S_IRWXO | S_IRWXU);
    if (mkdir_result == -1)
    {
        return make_unit_failure("Falha ao criar o diretório do arquivo\n");
    }

    return make_unit_success();
}

I64Result get_next_user_id(Database *db)
{
    rewind(db->users_index);
    char buffer[LONG_MAX_LENGTH + 1] = {0};

    char *eof = fgets(buffer, LONG_MAX_LENGTH + 1, db->users_index);

    if (eof == NULL)
    {
        rewind(db->users_index);
        fwrite("1", sizeof(char), 1, db->users_index);
        buffer[0] = '1';
        buffer[1] = '\0';
    }

    char *end;
    long number = strtol(buffer, &end, 10);

    if (*end != '\0')
    {
        return make_i64_failure("Falha ao ler o index do usuário");
    }

    rewind(db->users_index);
    fprintf(db->users_index, "%ld", number + 1);

    return make_i64_success(number);
}
