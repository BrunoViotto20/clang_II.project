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

struct DatabaseConnection
{
    FILE *users;
    FILE *orders;
};

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

DatabaseConnection *db_open()
{
    DatabaseConnection *db = (DatabaseConnection *)malloc(sizeof(DatabaseConnection));

    if (db == NULL)
    {
        perror("Falha ao abrir o banco de dados\n");
        return NULL;
    }

    db->users = fopen(USERS_PATH, "a+");
    db->orders = fopen(ORDERS_PATH, "a+");

    return db;
}

void db_close(DatabaseConnection *db)
{
    fclose(db->users);
    fclose(db->orders);
    free(db);
}

User *db_get_users(DatabaseConnection *db)
{
    rewind(db->users);

    // Allocates a buffer to retrieve the users
    int users_capacity = 8;
    int users_length = 0;
    User *users = (User *)malloc(sizeof(User) * users_capacity);

    // Loop to get all users
    char line_buffer[LINE_BUFFER_LENGTH] = {0};
    while (true)
    {
        // Gets a line from the database
        char *eof = fgets(line_buffer, LINE_BUFFER_LENGTH, db->users);

        // Grows the user buffer if it is full
        if (users_capacity <= users_length)
        {
            users_capacity *= 2;
            users = realloc(users, sizeof(User) * users_capacity);

            if (users == NULL)
            {
                perror("ERROR: Out of memory to reallocate");
                return NULL;
            }
        }

        // Tries to parse the user from the retrieved line and store it into the array
        bool successful_parse = try_parse_user(line_buffer, users + users_length);
        users_length++;

        // If fails to parse the user, fails the whole function
        if (!successful_parse)
        {
            perror("ERROR: The database is corrupted");
            return NULL;
        }

        if (eof == NULL)
        {
            break;
        }
    }

    // Shrinks the user buffer to its exact length
    if (users_capacity != users_length)
    {
        users = realloc(users, sizeof(User) * users_length);

        if (users == NULL)
        {
            perror("ERROR: Out of memory to reallocate");
            return NULL;
        }
    }

    return users;
}

// User db_get_user(DatabaseConnection *db, char cpf[CPF_LENGTH]);
// void db_insert_user(DatabaseConnection *db, User *user);
// User db_disable_user(DatabaseConnection *db, User *user);
// User db_delete_user(DatabaseConnection *db, User *user);

// Order *db_get_orders(DatabaseConnection *db, User *user);
// void db_insert_order(DatabaseConnection *db, Order *order);

/* Private functions */

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
