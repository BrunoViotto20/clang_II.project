#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

/// @brief Represents an application error.
typedef struct Error
{
    /// @brief The message of the error.
    char *message;
} Error;

/// @brief Represents a result which may succeed or return an error.
typedef struct
{
    bool is_success;
    Error error;
} UnitResult;

/// @brief Creates a unit success.
/// @return Returns a successful result.
UnitResult make_unit_success();

/// @brief Creates a unit failure.
/// @param message The error message.
/// @return Returns a failure result.
UnitResult make_unit_failure(char *message);

/// @brief Represents a result which may succeed with an i64 value or fail with an error.
typedef struct
{
    bool is_success;
    union
    {
        long value;
        Error error;
    };
} I64Result;

/// @brief Creates an i64 success.
/// @param number The successfully obtained number
/// @return Returns a successful result.
I64Result make_i64_success(long number);

/// @brief Creates an i64 failure.
/// @param message The error message.
/// @return Returns a failure result.
I64Result make_i64_failure(char *message);

#endif
