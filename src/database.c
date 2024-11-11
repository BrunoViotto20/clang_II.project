#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

#define USERS_PATH "database/users.txt"
#define ORDERS_PATH "database/orders.txt"

#define CHAR_SEPARATOR ';'
#define LINE_BUFFER_LENGTH 256
#define LONG_MAX_LENGTH 19

struct Database
{
    FILE *users;
    FILE *orders;
};

struct UnitResult
{
    bool is_success;
    Error error;
};

struct DatabaseResult
{
    bool is_success;
    union
    {
        Database *database;
        Error error;
    };
};

struct UsersResult
{
    bool is_success;
    union
    {
        Users users;
        Error error;
    };
};

struct UserResult
{
    bool is_success;
    union
    {
        User user;
        Error error;
    };
};

struct OrdersResult
{
    bool is_success;
    union
    {
        Orders orders;
        Error error;
    };
};

UnitResult make_unit_success();
UnitResult make_unit_failure(char *message);
DatabaseResult make_database_success(Database *database);
DatabaseResult make_database_failure(char *message);
UsersResult make_users_success(User *users, long length);
UsersResult make_users_failure(char *message);
UserResult make_user_success(User user);
UserResult make_user_failure(char *message);
OrdersResult make_orders_success(Order *orders, long length);
OrdersResult make_orders_failure(char *message);

/// @brief Tries to consume and parse a string, value surrounded by the character \" on both ends.
/// @param input The string to be consumed and parsed.
/// @param buffer The buffer to which the consumed string will be written to.
/// @param buffer_length The length of the buffer being written into.
/// @return Returns an integer representing the offset consumed by the method,
///         if the returned value is zero, the method failed to consume/parse the string.
int try_consume_string(char *input, char *buffer, int buffer_length);

/// @brief Tris to consume and parse a signed 64 bits number.
/// @param input The string to be consumed and parsed.
/// @param number The number parsed from the input string.
/// @return Returns an integer representing the offset consumed by the method,
///         if the returned value is zero, the method failed to consume/parse the string.
int try_consume_i64(char *input, long *number);

/// @brief Tries to parse a user.
/// @param string The string to be parsed.
/// @param user The user parsed from the string.
/// @return Returns true if the parse was successful, false otherwise.
bool try_parse_user(char *string, User *user);

void db_open(DatabaseResult *result)
{
    Database *db = (Database *)malloc(sizeof(Database));
    if (db == NULL)
    {
        *result = make_database_failure("Falha ao alocar o handler do banco de dados");
        return;
    }

    db->users = fopen(USERS_PATH, "a+");
    if (db->users == NULL)
    {
        *result = make_database_failure("Falha ao abrir arquivo");
        return;
    }

    db->orders = fopen(ORDERS_PATH, "a+");
    if (db->orders == NULL)
    {
        *result = make_database_failure("Falha ao abrir arquivo");
        return;
    }

    *result = make_database_success(db);
}

void db_close(Database *db, UnitResult *result)
{
    fclose(db->users);
    fclose(db->orders);
    free(db);

    *result = make_unit_success();
}

void db_get_users(Database *db, UsersResult *result)
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
            if (users == NULL)
            {
                free(users);
                *result = make_users_failure("No memory to reallocate");
                return;
            }
            users = temp;
        }

        // Tries to parse the user from the retrieved line and store it into the array
        bool successful_parse = try_parse_user(line_buffer, users + users_length);
        users_length++;
        if (!successful_parse)
        {
            free(users);
            *result = make_users_failure("The database is corrupted");
            return;
        }
    }

    // Shrinks the user buffer to its exact length
    if (users_capacity != users_length)
    {
        User *temp = realloc(users, sizeof(User) * users_length);
        if (users == NULL)
        {
            *result = make_users_failure("No memory to reallocate");
            return;
        }
        users = temp;
    }

    *result = make_users_success(users, users_length);
}

void db_get_user(Database *db, char cpf[CPF_LENGTH], UserResult *result)
{
    rewind(db->users);

    char buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        char *eof = fgets(buffer, LINE_BUFFER_LENGTH, db->users);
        if (eof == NULL)
        {
            *result = make_user_failure("Nenhum usuário encontrado!");
            return;
        }

        User user;
        bool successful_parse = try_parse_user(buffer, &user);
        if (!successful_parse)
        {
            *result = make_user_failure("Banco de dados corrompido");
            return;
        }

        if (strcmp(cpf, user.cpf) != 0)
        {
            continue;
        }

        *result = make_user_success(user);
        return;
    }
}

// void db_insert_user(Database *db, User *user);
// User db_disable_user(Database *db, User *user);
// User db_delete_user(Database *db, User *user);

// Order *db_get_orders(Database *db, User *user);
// void db_insert_order(Database *db, Order *order);

/* Private functions */

UnitResult make_unit_success()
{
    UnitResult result = {true, {NULL}};
    return result;
}

UnitResult make_unit_failure(char *message)
{
    Error error = {message};
    UnitResult result = {false};
    result.error = error;
    return result;
}

bool is_unit_success(UnitResult *result)
{
    return result->is_success;
}

Error get_unit_error(UnitResult *result)
{
    assert(result->is_success);

    return result->error;
}

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

bool is_database_success(DatabaseResult *result)
{
    return result->is_success;
}

Database *get_database_value(DatabaseResult *result)
{
    assert(result->is_success);

    return result->database;
}

Error get_database_error(DatabaseResult *result)
{
    assert(result->is_success);

    return result->error;
}

UsersResult make_users_success(User *users, long length)
{
    Users usrs = {users, length};
    UsersResult result = {true, {usrs}};
    return result;
}

UsersResult make_users_failure(char *message)
{
    Error error = {message};
    UsersResult result = {false};
    result.error = error;
    return result;
}

bool is_users_success(UsersResult *result)
{
    return result->is_success;
}

Users get_users_value(UsersResult *result)
{
    assert(result->is_success);

    return result->users;
}

Error get_users_error(UsersResult *result)
{
    assert(result->is_success);

    return result->error;
}

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

bool is_user_success(UserResult *result)
{
    return result->is_success;
}

User get_user_value(UserResult *result)
{
    assert(result->is_success);

    return result->user;
}

Error get_user_error(UserResult *result)
{
    assert(result->is_success);

    return result->error;
}

OrdersResult make_orders_success(Order *orders, long length)
{
    Orders ordrs = {orders, length};
    OrdersResult result = {true, {ordrs}};
    return result;
}

OrdersResult make_orders_failure(char *message)
{
    Error error = {message};
    OrdersResult result = {false};
    result.error = error;
    return result;
}

bool is_orders_success(OrdersResult *result)
{
    return result->is_success;
}

Orders get_orders_value(OrdersResult *result)
{
    assert(result->is_success);

    return result->orders;
}

Error get_orders_error(OrdersResult *result)
{
    assert(result->is_success);

    return result->error;
}

bool try_parse_user(char *string, User *user)
{
    // Parses the user id
    int offset = try_consume_i64(string, &user->id);
    string += offset;
    if (offset == 0 || *string != CHAR_SEPARATOR)
    {
        return false;
    }
    string++;

    // Parses the user name
    offset = try_consume_string(string, user->name, USER_NAME_LENGTH);
    string += offset;
    if (offset == 0 || *string != CHAR_SEPARATOR)
    {
        return false;
    }
    string++;

    // Parses the user cpf
    offset = try_consume_string(string, user->cpf, CPF_LENGTH);
    string += offset;
    if (offset == 0 || *string != '\0')
    {
        return false;
    }
    string++;

    return true;
}

int try_consume_string(char *input, char *buffer, int buffer_length)
{
    if (input[0] != '"')
    {
        return 0;
    }
    input++;

    int i;
    for (i = 0; input[i] != '\0' && i < buffer_length - 1 && (buffer[i] != '"' || input[i - 1] == '\\'); i++)
    {
        buffer[i] = input[i];
    }
    buffer[i] = '\0';

    if (input[i] != '"')
    {
        return 0;
    }

    return i;
}

int try_consume_i64(char *input, long *number)
{
    char buffer[LONG_MAX_LENGTH + 1] = {0};

    int i;
    for (i = 0; input[i] != '\0' && input[i] != ';' && i < LONG_MAX_LENGTH; i++)
    {
        buffer[i] = input[i];
    }
    buffer[i] = '\0';

    char *end;
    *number = strtol(buffer, &end, 10);

    if (buffer == end || *end != '\0')
    {
        return 0;
    }

    return i;
}
