#include <stdio.h>
#include <stdlib.h>

#include "console.h"

void clear_console()
{
    int result;

#if _WIN32
    result = system("cls");
#else
    result = system("clear");
#endif

    if (result != 0)
    {
        perror("ERRO: Falha ao limpar o console");
    }
}

void wait_key_press()
{
    printf("Pressione ENTER para continuar...\n");
    getchar();
    getchar();
}
