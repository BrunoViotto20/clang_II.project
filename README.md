# Como compilar o código?

Segue abaixo instruções de como compilar o código do repositório

## Linux (Ubuntu)

Primeiro vamos precisar instalar o compilador e a ferramenta de build, para isso execute o seguinte comando:

`sudo apt install gcc make`

Caso não consiga executar o comando acima, tente executar o comando abaixo e depois tente executar o comanado acima novamente:

`sudo apt update && sudo apt upgrade`

Após instalar as ferramentas de compilação, vá até a raiz do projeto e execute o comando `make`

Isso irá gerar um executável na pasta `bin/` chamado `main`

Para executá-lo você pode executar um dos comandos abaixo:

`./bin/main`

ou

`make run`

# Exemplos de código

Segue abaixo alguns exemplos de como usar certos trechos de códigos

## Como usar a API de arquivo/database?

```
// Abrir uma conexão com o arquivo
DatabaseResult *connection_result;
db_open(connection_result);
// Se a operação não for um sucesso
if (!is_database_success(connection_result))
{
    Error error = get_database_error(connection_result);    // Pega o erro no resultado
    perror(error.message);                                  // Printa a mensagem como um erro
exit(-1);                                                   // Mata o programa com código -1
}
Database *connection = get_database_value(connection_result);

char cpf[CPF_LENGTH] = {0};
printf("Digite o cpf: ");
fgets(cpf, CPF_LENGTH, stdin);

// Deleta um usuário com determinado CPF
UnitResult *delete_result;
db_delete_user(connection, cpf, delete_result);
if (!is_unit_success(delete_result))
{
    perror("Usuário não encontrado");
}

UsersResult *users_result;
db_get_users(connection, users_result);
if (!is_users_success(users_result))
{
    Error error = get_database_error(connection_result);
    perror(error.message);
    exit(-1);
}
Users users = get_users_value(users_result);
free(users.values);                             // Desaloca o array dinamicamente alocado

UnitResult *close_result;
db_close(connection, close_result);
if (!is_unit_success(close_result))
{
    Error error = get_unit_error(close_result);
    perror(error.message);
    exit(-1);
}
```
