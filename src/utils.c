
#include <stdlib.h>
#include "utils.h"

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

I64Result make_i64_success(long number)
{
    I64Result result = {true, {number}};
    return result;
}

I64Result make_i64_failure(char *message)
{
    Error error = {message};
    I64Result result = {false};
    result.error = error;
    return result;
}
