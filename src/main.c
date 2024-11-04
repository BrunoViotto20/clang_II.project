#include <stdio.h>
#include <stdlib.h>

typedef struct Product Product;
typedef struct PaymentMethod PaymentMethod;

struct Product
{
    char name[32];
    double price;
};

struct PaymentMethod
{
    char name[32];
    double fee;
};

Product products_menu(const Product *products, int length);
PaymentMethod payment_menu(const PaymentMethod *paymentMethods, int length);

int main(void)
{
    const Product products[] = {
        {"Cachorro Quente", 12},
        {"X-Salada", 15.5},
        {"X-Bacon", 18},
        {"Torrada Simples", 8},
        {"Refrigerante", 4.5},
        {"Cerveja Brahma", 6},
        {"Cigarro Malboro", 6}};
    const int productsLength = sizeof(products) / sizeof(Product);

    const PaymentMethod paymentMethods[] = {
        {"Crédito", .02},
        {"Débito", .02},
        {"Dinheiro", 0}};
    const int paymentMethodsLength = sizeof(paymentMethods) / sizeof(Product);

    Product product = products_menu(products, productsLength);

    printf("Digite a quantidade do item: ");
    int amount;
    scanf("%d", &amount);

    PaymentMethod paymentMethod = payment_menu(paymentMethods, paymentMethodsLength);

    if (paymentMethod.fee != 0)
    {
        printf("Taxa de %.1lf%%, sobre o valor da compra:\n", paymentMethod.fee * 100);
    }
    
    double total_price = amount * product.price;
    double final_price = total_price * (1 + paymentMethod.fee);
    printf("O valor total é: R$%.2f\n", final_price);

    return 0;
}

Product products_menu(const Product *products, int length)
{
    printf("MENU DE PEDIDOS:\n\n");
    for (int i = 0; i < length; i++)
    {
        Product product = products[i];
        printf("[ %d ] %s R$%.2lf\n", i + 1, product.name, product.price);
    }
    printf("\n");
    printf("Digite a opção do produto: ");
    int option;
    scanf("%d", &option);

    return products[option - 1];
}

PaymentMethod payment_menu(const PaymentMethod *paymentMethods, int length)
{
    printf("MENU DE PAGAMENTO :\n\n");
    for (int i = 0; i < length; i++)
    {
        PaymentMethod payment_method = paymentMethods[i];
        printf("[ %d ] %s", i + 1, payment_method.name);

        if (payment_method.fee != 0) {
            printf(" (+%.1lf%%)\n", payment_method.fee * 100);
        }
    }
    printf("\n");
    printf("Digite a opção da forma de pagamento: ");
    int option;
    scanf("%d", &option);

    return paymentMethods[option - 1];
}
