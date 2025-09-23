#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// definição da estrutura Struct
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para exibir o estado atual do mapa
void exibirMapa(Territorio *mapa, int n) {
    printf("\n=== MAPA DO MUNDO - ESTADO ATUAL ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (Exército %s, Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função de ataque entre territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;

        // Se tropas chegarem a 0, troca de dono
        if (defensor->tropas <= 0) {
            printf(">>> O território %s foi conquistado pelo exército %s!\n",
                   defensor->nome, atacante->cor);

            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2;  // metade ocupa
            atacante->tropas -= defensor->tropas;     // atacante perde essa metade
        }
    } else {
        printf("DEFESA RESISTIU! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
    }
}

int main() {
    srand(time(NULL)); // para garantir aleatoriedade

    int n = 5; // agora fixo em 5, mas poderia ser digitado pelo usuário
    Territorio *territorios = (Territorio*) malloc(n * sizeof(Territorio));

    if (territorios == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro dos territórios
    for (int i = 0; i < n; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // consome '\n'
    }

    exibirMapa(territorios, n);

    // Loop de ataque
    int atacanteIndex, defensorIndex;
    do {
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", n);
        scanf("%d", &atacanteIndex);

        if (atacanteIndex == 0) break;

        printf("Escolha o território defensor (1 a %d, diferente do atacante): ", n);
        scanf("%d", &defensorIndex);

        atacanteIndex--;
        defensorIndex--;

        // Validações
        if (atacanteIndex < 0 || atacanteIndex >= n ||
            defensorIndex < 0 || defensorIndex >= n ||
            atacanteIndex == defensorIndex) {
            printf("Escolha inválida!\n");
            continue;
        }

        if (strcmp(territorios[atacanteIndex].cor, territorios[defensorIndex].cor) == 0) {
            printf("Não é possível atacar territórios do mesmo exército!\n");
            continue;
        }

        atacar(&territorios[atacanteIndex], &territorios[defensorIndex]);
        exibirMapa(territorios, n);

    } while (1);

    free(territorios); // libera memória
    return 0;
}
