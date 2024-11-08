#include <stdio.h>
#include <stdlib.h>

#include "console.h"

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
    printf("Pressione ENTER para continuar...\n");
    getchar();
    getchar();
}
