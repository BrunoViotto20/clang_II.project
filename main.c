#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    float mult;
    float qtd;
    float porcentagem;
    float soma;
    int opcao;
    int opcao2;
    int idade;
    float *p;

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
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        printf("Digite a quantidade do item :");
        scanf("%f", &qtd);

        p = (float *)malloc(qtd * sizeof(float));

        mult = (qtd * 12.00);
        printf("O valor a pagar é: %.2f Reais\n", mult);

        printf("MENU DE PAGAMENTO :\n\n");
        printf("1 - para Cartão de Débito +2%%\n");
        printf("2 - para Cartão de Credito +2%%\n");
        printf("3 - para Dinheiro em Especie\n");
        printf("\n");
        printf("Digite a opção da forma de pagamento: ");
        scanf("%d", &opcao2);

        switch (opcao2)
        {
        case 1:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 2:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 3:
            printf("O valor a pagar sera de : %.2f Reais", mult);
            break;

        default:
            printf("Digite um codigo valido!");
            break;
        }
        break;

    case 2:
        printf("Digite a quantidade do item :");
        scanf("%f", &qtd);

        p = (float *)malloc(qtd * sizeof(float));

        mult = (qtd * 15.50);
        printf("O valor a pagar sera de : %.2f Reais\n\n", mult);

        printf("MENU DE PAGAMENTO :\n\n");
        printf("1 - para Cartão de Débito +2%%\n");
        printf("2 - para Cartão de Credito +2%%\n");
        printf("3 - para Dinheiro em Especie\n");
        printf("\n");
        printf("Digite a opção da forma de pagamento: ");
        scanf("%d", &opcao2);

        switch (opcao2)
        {
        case 1:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 2:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 3:
            printf("O valor a pagar sera de : %.2f Reais", mult);
            break;

        default:
            printf("Digite um codigo valido!");
            break;
        }
        break;

    case 3:
        printf("Digite a quantidade do item :");
        scanf("%f", &qtd);
        p = (float *)malloc(qtd * sizeof(float));
        mult = (qtd * 18.00);
        printf("O valor a pagar é: %.2f Reais\n\n", mult);

        printf("MENU DE PAGAMENTO :\n\n");
        printf("1 - para Cartão de Débito +2%%\n");
        printf("2 - para Cartão de Credito +2%%\n");
        printf("3 - para Dinheiro em Especie\n");
        printf("\n");
        printf("Digite a opção da forma de pagamento: ");
        scanf("%d", &opcao2);

        switch (opcao2)
        {
        case 1:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 2:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 3:
            printf("O valor a pagar sera de : %.2f Reais", mult);
            break;

        default:
            printf("Digite um codigo valido!");
            break;
        }
        break;

    case 4:
        printf("Digite a quantidade do item :");
        scanf("%f", &qtd);
        p = (float *)malloc(qtd * sizeof(float));
        mult = (qtd * 8.00);
        printf("O valor a pagar é: %.2f Reais\n\n", mult);

        printf("MENU DE PAGAMENTO :\n\n");
        printf("1 - para Cartão de Débito +2%%\n");
        printf("2 - para Cartão de Credito +2%%\n");
        printf("3 - para Dinheiro em Especie\n");
        printf("\n");
        printf("Digite a opção da forma de pagamento: ");
        scanf("%d", &opcao2);
        switch (opcao2)
        {
        case 1:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 2:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 3:
            printf("O valor a pagar sera de : %.2f Reais", mult);
            break;

        default:
            printf("Digite um codigo valido!");
            break;
        }
        break;

    case 5:
        printf("Digite a quantidade do item :");
        scanf("%f", &qtd);
        p = (float *)malloc(qtd * sizeof(float));
        mult = (qtd * 4.50);
        printf("O valor a pagar é: %.2f Reais\n\n", mult);

        printf("MENU DE PAGAMENTO :\n\n");
        printf("1 - para Cartão de Débito +2%%\n");
        printf("2 - para Cartão de Credito +2%%\n");
        printf("3 - para Dinheiro em Especie \n");
        printf("\n");
        printf("Digite a opção da forma de pagamento: ");
        scanf("%d", &opcao2);

        switch (opcao2)
        {
        case 1:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 2:
            porcentagem = (mult * 2) / 100;
            soma = (porcentagem + mult);
            printf("taxa de 2%%, sobre o valor da compra :\n");
            printf("O valor total é: %.2f Reais", soma);
            break;

        case 3:
            printf("O valor a pagar sera de : %.2f Reais", mult);
            break;

        default:
            printf("Digite um codigo valido!");
            break;
        }
        break;

    case 6:
        printf("Digite a sua idade :");
        scanf("%d", &idade);

        if (idade >= 18)
        {
            printf("Digite a quantidade do item :");
            scanf("%f", &qtd);
            p = (float *)malloc(qtd * sizeof(float));
            mult = (qtd * 6.00);
            printf("O valor a pagar é: %.2f Reais\n\n", mult);

            printf("MENU DE PAGAMENTO :\n\n");
            printf("1 - para Cartão de Débito +2%%\n");
            printf("2 - para Cartão de Credito +2%%\n");
            printf("3 - para Dinheiro em Especie \n");
            printf("\n");
            printf("Digite a opção da forma de pagamento: ");
            scanf("%d", &opcao2);

            switch (opcao2)
            {
            case 1:
                porcentagem = (mult * 2) / 100;
                soma = (porcentagem + mult);
                printf("taxa de 2%%, sobre o valor da compra :\n");
                printf("O valor total é: %.2f Reais", soma);
                break;

            case 2:
                porcentagem = (mult * 2) / 100;
                soma = (porcentagem + mult);
                printf("taxa de 2%%, sobre o valor da compra :\n");
                printf("O valor total é: %.2f Reais", soma);
                break;

            case 3:
                printf("O valor a pagar sera de : %.2f Reais", mult);
                break;

            default:
                printf("Digite um codigo valido!");
                break;
            }
            break;
        }
        else if ((idade >= 0) && (idade < 18))
        {
            printf("Venda proibida para menores de 18 anos");
        }
        else
        {
            printf("Digite uma idade valida!!");
        }
        break;

    case 7:
        printf("Digite a sua idade :");
        scanf("%d", &idade);

        if (idade >= 18)
        {
            printf("Digite a quantidade do item :");
            scanf("%f", &qtd);
            p = (float *)malloc(qtd * sizeof(float));
            mult = (qtd * 6.00);
            printf("O valor a pagar sera de : %.2f Reais\n\n", mult);

            printf("MENU DE PAGAMENTO :\n\n");
            printf("1 - para Cartão de Débito +2%%\n");
            printf("2 - para Cartão de Credito +2%%\n");
            printf("3 - para Dinheiro em Especie\n");
            printf("\n");
            printf("Digite a opção da forma de pagamento: ");
            scanf("%d", &opcao2);

            switch (opcao2)
            {
            case 1:
                porcentagem = (mult * 2) / 100;
                soma = (porcentagem + mult);
                printf("taxa de 2%%, sobre o valor da compra :\n");
                printf("O valor total é: %.2f Reais", soma);
                break;

            case 2:
                porcentagem = (mult * 2) / 100;
                soma = (porcentagem + mult);
                printf("taxa de 2%%, sobre o valor da compra :\n");
                printf("O valor total é: %.2f Reais", soma);
                break;

            case 3:
                printf("O valor a pagar sera de : %.2f Reais", mult);
                break;

            default:
                printf("Digite um codigo valido!");
                break;
            }
            break;
        }
        else if ((idade >= 0) && (idade < 18))
        {
            printf("Venda proibida para menores de 18 anos!");
        }
        else
        {
            printf("Digite uma idade valida!");
        }
        break;

    default:
        printf("Digite um codigo valido!");
        break;
    }

    return 0;
}
