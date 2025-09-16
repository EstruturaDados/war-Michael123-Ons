#include <stdio.h>
#include <string.h>


//definição da estrutura
typedef struct{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


int main() {
    Territorio territorios[5]; //Vetor para armazenar até 5 territórios


    //Laço para cadastrar os 5 territórios
    for (int i = 0; i < 5; i++){
        printf("\n=== Cadastro do Território %d ===\n", i + 1);


        //Lê o nome do território
        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin); 
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; 
        // Remove o '\n' do final


         // Leitura da cor do exército
        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // Consome o '\n' que fica no buffer após scanf
    }
    // Exibição dos dados cadastrados
    printf("\n\n=== Dados dos Territórios Cadastrados ===\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}
