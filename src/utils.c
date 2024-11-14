
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
