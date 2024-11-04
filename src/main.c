#include <stdio.h>
#include <stdlib.h>

typedef struct Produto Produto;

struct Produto
{
    char nome[32];
    double preco;
};

int main_menu();
int payment_menu();

int main(void)
{
    Produto produtos[] = {
        {"Cachorro Quente", 12},
        {"X-Salada", 15.5},
        {"X-Bacon", 18},
        {"Torrada Simples", 8},
        {"Refrigerante", 4.5},
        {"Cerveja Brahma", 6},
        {"Cigarro Malboro", 6}};

    int menu = main_menu();

    Produto produto = produtos[menu - 1];

    printf("Digite a quantidade do item :");
    int quantidade;
    scanf("%d", &quantidade);

    double preco_total = quantidade * produto.preco;

    int pagamento = payment_menu();

    double porcentagem;
    double soma;
    switch (pagamento)
    {
    case 1:
        porcentagem = (preco_total * 2) / 100;
        soma = (porcentagem + preco_total);
        printf("taxa de 2%%, sobre o valor da compra :\n");
        printf("O valor total é: %.2f Reais\n", soma);
        break;

    case 2:
        porcentagem = (preco_total * 2) / 100;
        soma = (porcentagem + preco_total);
        printf("taxa de 2%%, sobre o valor da compra :\n");
        printf("O valor total é: %.2f Reais\n", soma);
        break;

    case 3:
        printf("O valor a pagar sera de : %.2f Reais\n", preco_total);
        break;

    default:
        printf("Digite um codigo valido!");
        break;
    }

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
