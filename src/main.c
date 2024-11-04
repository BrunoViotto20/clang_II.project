#include <stdio.h>
#include <stdlib.h>

typedef struct Produto Produto;
typedef struct FormaPagamento FormaPagamento;

struct Produto
{
    char nome[32];
    double preco;
};

struct FormaPagamento
{
    char nome[32];
    double custo;
};

int main_menu();
int payment_menu();

int main(void)
{
    const Produto produtos[] = {
        {"Cachorro Quente", 12},
        {"X-Salada", 15.5},
        {"X-Bacon", 18},
        {"Torrada Simples", 8},
        {"Refrigerante", 4.5},
        {"Cerveja Brahma", 6},
        {"Cigarro Malboro", 6}};

    const FormaPagamento formasPagamento[] = {
        {"Crédito", .02},
        {"Débito", .02},
        {"Dinheiro", 0}};

    int menu = main_menu();

    Produto produto = produtos[menu - 1];

    printf("Digite a quantidade do item: ");
    int quantidade;
    scanf("%d", &quantidade);

    double preco_total = quantidade * produto.preco;

    int opcaoPagamento = payment_menu();

    FormaPagamento formaPagamento = formasPagamento[opcaoPagamento - 1];

    if (formaPagamento.custo != 0)
    {
        printf("Taxa de %.1lf%%, sobre o valor da compra:\n", formaPagamento.custo * 100);
    }
    double preco_final = preco_total * (1 + formaPagamento.custo);
    printf("O valor total é: R$%.2f\n", preco_final);

    return 0;
}

int main_menu()
{
    printf("MENU DE PEDIDOS:\n\n");
    printf("1 - para Cachorro-quente R$ 12,00\n");
    printf("2 - para X-salada R$ 15,50\n");
    printf("3 - para X-bacon R$ 18,00\n");
    printf("4 - para Torrada simples R$ 8,00\n");
    printf("5 - para Refrigerante  R$ 4,50\n");
    printf("6 - para Cerveja Brahma R$ 6,00\n");
    printf("7 - para Cigarro Malboro R$ 6.00\n");
    printf("\n");
    printf("Digite a opção do produto: ");
    int opcao;
    scanf("%d", &opcao);

    return opcao;
}

int payment_menu()
{
    printf("MENU DE PAGAMENTO :\n\n");
    printf("1 - para Cartão de Débito +2%%\n");
    printf("2 - para Cartão de Credito +2%%\n");
    printf("3 - para Dinheiro em Especie\n");
    printf("\n");
    printf("Digite a opção da forma de pagamento: ");
    int opcao;
    scanf("%d", &opcao);

    return opcao;
}
