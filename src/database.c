#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"
#include "database.h"

// File paths
#define USERS_PATH "database/users.txt"
#define ORDERS_PATH "database/orders.txt"

#define USERS_INDEX_PATH "database/users_index.txt"

#define FIELD_SEPARATOR ';'
#define RECORD_SEPARATOR '\n'
#define LINE_BUFFER_LENGTH 256
#define LONG_MAX_LENGTH 19

struct Database
{
    FILE *users;
    FILE *users_index;
    FILE *orders;
};

char *try_consume_string(char *input, char *buffer, int buffer_length);
char *try_consume_i64(char *input, long *number);
char *try_consume_bool(char *buffer, bool *boolean);
char *try_consume_true(char *buffer);
char *try_consume_false(char *buffer);

bool try_parse_user(char *string, User *user);

void write_separator(FILE *fd);
bool write_user(Database *db, User *user);
UnitResult write_order(Database *db, Order *order);
void write_string(FILE *fd, char *string, int length);
void write_bool(FILE *fd, bool value);

UnitResult ensure_directory_exists(char *directory_path);

I64Result get_next_user_id(Database *db);

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
                return make_users_failure("No memory to reallocate");
            }
            users = temp;
        }

        // Tries to parse the user from the retrieved line and store it into the array
        bool successful_parse = try_parse_user(line_buffer, users + users_length);
        users_length++;
        if (!successful_parse)
        {
            free(users);
            return make_users_failure("The database is corrupted");
        }

        if (!users[users_length - 1].active)
        {
            users_length--;
        }
    }

    // Shrinks the user buffer to its exact length
    if (users_capacity != users_length)
    {
        User *temp = realloc(users, sizeof(User) * users_length);
        if (users == NULL)
        {
            return make_users_failure("No memory to reallocate");
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

        User user;
        bool successful_parse = try_parse_user(buffer, &user);
        if (!successful_parse)
        {
            return make_user_failure("Banco de dados corrompido");
        }

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

    bool success_insert = write_user(db, user);
    if (!success_insert)
    {
        return make_unit_failure("Falha ao inserir o usuário\n");
    }

    return make_unit_success();
}

UnitResult db_disable_user(Database *db, char cpf[CPF_LENGTH])
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

        User user;
        bool successful_parse = try_parse_user(buffer, &user);
        if (!successful_parse)
        {
            return make_unit_failure("Banco de dados corrompido");
        }

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
            return make_unit_failure("Falha ao atualizar arquivo");
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
                return make_unit_failure("Falha ao diminuir o tamanho do arquivo");
            }
        }

        return make_unit_success();
    }
}

UnitResult db_delete_user(Database *db, char cpf[CPF_LENGTH])
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

        User user;
        bool successful_parse = try_parse_user(buffer, &user);
        if (!successful_parse)
        {
            return make_unit_failure("Banco de dados corrompido");
        }

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

// Order *db_get_orders(Database *db, User *user);

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

bool try_parse_user(char *string, User *user)
{
    // Parses the user id
    string = try_consume_i64(string, &user->id);
    if (string == NULL || *string != FIELD_SEPARATOR)
    {
        return false;
    }
    string++;

    // Parses the user name
    string = try_consume_string(string, user->name, USER_NAME_LENGTH + 1);
    if (string == NULL || *string != FIELD_SEPARATOR)
    {
        return false;
    }
    string++;

    // Parses the user cpf
    string = try_consume_string(string, user->cpf, CPF_LENGTH + 1);
    if (string == NULL || *string != FIELD_SEPARATOR)
    {
        return false;
    }
    string++;

    // Parses the user active status
    string = try_consume_bool(string, &user->active);
    if (string == NULL || *string != '\n')
    {
        return false;
    }

    return true;
}

char *try_consume_string(char *input, char *buffer, int buffer_length)
{
    if (input[0] != '"')
    {
        return NULL;
    }
    input++;

    int i;
    for (i = 0; input[i] != '\0' && i < buffer_length - 1 && (input[i] != '"' || input[i - 1] == '\\'); i++)
    {
        buffer[i] = input[i];
    }
    buffer[i] = '\0';

    if (input[i] != '"')
    {
        return NULL;
    }

    return input + i + 1;
}

char *try_consume_i64(char *input, long *number)
{
    char buffer[LONG_MAX_LENGTH + 1] = {0};

    int i;
    for (i = 0; input[i] != '\0' && input[i] != FIELD_SEPARATOR && input[i] != RECORD_SEPARATOR && i < LONG_MAX_LENGTH; i++)
    {
        buffer[i] = input[i];
    }
    buffer[i] = '\0';

    char *end;
    *number = strtol(buffer, &end, 10);

    if (buffer == end || *end != '\0')
    {
        return NULL;
    }

    return input + strlen(buffer);
}

char *try_consume_bool(char *buffer, bool *boolean)
{
    if (*buffer == 't')
    {
        *boolean = true;
        return try_consume_true(buffer);
    }

    if (*buffer == 'f')
    {
        *boolean = false;
        return try_consume_false(buffer);
    }

    return NULL;
}

char *try_consume_true(char *buffer)
{
    if (buffer[0] != 't' || buffer[1] != 'r' || buffer[2] != 'u' || buffer[3] != 'e')
    {
        return NULL;
    }

    return buffer + 4;
}

char *try_consume_false(char *buffer)
{
    if (buffer[0] != 'f' || buffer[1] != 'a' || buffer[2] != 'l' || buffer[3] != 's' || buffer[4] != 'e')
    {
        return NULL;
    }

    return buffer + 5;
}

void write_separator(FILE *fd)
{
    const char buffer[1] = {FIELD_SEPARATOR};
    fwrite(buffer, sizeof(char), 1, fd);
    fflush(fd);
}

bool write_user(Database *db, User *user)
{
    char id_buffer[LONG_MAX_LENGTH + 1] = {0};
    snprintf(id_buffer, sizeof(id_buffer), "%ld", user->id);
    int id_length = strlen(id_buffer);

    int name_length = strlen(user->name);

    FILE *fd = db->users;
    fwrite(id_buffer, sizeof(char), id_length, fd);
    write_separator(fd);
    write_string(fd, user->name, name_length);
    write_separator(fd);
    write_string(fd, user->cpf, CPF_LENGTH);
    write_separator(fd);
    write_bool(fd, user->active);
    const char buffer[1] = {RECORD_SEPARATOR};
    fwrite(buffer, sizeof(char), 1, fd);

    fflush(fd);

    return true;
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

    return make_unit_success();
}

void write_string(FILE *fd, char *string, int length)
{
    const char quote[1] = {'\"'};
    fwrite(quote, sizeof(char), 1, fd);
    fwrite(string, sizeof(char), length, fd);
    fwrite(quote, sizeof(char), 1, fd);

    fflush(fd);
}

void write_bool(FILE *fd, bool value)
{
    if (value)
    {
        fwrite("true", sizeof(char), 4, fd);
    }
    else
    {
        fwrite("false", sizeof(char), 5, fd);
    }

    fflush(fd);
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
