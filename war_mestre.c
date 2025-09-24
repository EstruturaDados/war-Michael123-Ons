#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// definição da estrutura struct
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos das funções 
void exibirMapa(Territorio *mapa, int n);
void atacar(Territorio *atacante, Territorio *defensor, int *dadoAtacante, int *dadoDefensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void exibirMissao(const char* missao, const char* corJogador);
void liberarMemoria(Territorio* mapa, int n, char** coresUnicas, int numCores, char** missoesJogadores, int numJogadores);
void pausa();

/* Exibe o mapa formatado como nas imagens */
void exibirMapa(Territorio *mapa, int n) {
    printf("\n============== MAPA DO MUNDO ==============\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %-15s (Exercito: %-7s , Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("===========================================\n");
}

/* Função de ataque: joga um dado para atacante e defensor (1-6), atualiza tropas/controle */
void atacar(Territorio *atacante, Territorio *defensor, int *dadoAtacante, int *dadoDefensor) {
    *dadoAtacante = (rand() % 6) + 1;
    *dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Ataque (%s): %d | Defesa (%s): %d\n", atacante->nome, *dadoAtacante, defensor->nome, *dadoDefensor);

    if (*dadoAtacante > *dadoDefensor) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;
        if (defensor->tropas <= 0) {
            printf(">>> O território %s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            // transfere cor e metade das tropas do atacante (ocupação) 
            strcpy(defensor->cor, atacante->cor);
            int tropasParaOcupacao = atacante->tropas / 2;
            if (tropasParaOcupacao < 1) tropasParaOcupacao = 1;
            defensor->tropas = tropasParaOcupacao;
            atacante->tropas -= defensor->tropas;
            if (atacante->tropas < 0) atacante->tropas = 0;
        }
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Sorteia missão e copia para destino (destino deve estar alocado) 
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

// Exibe a missão (passagem por valor - apenas leitura) 
void exibirMissao(const char* missao, const char* corJogador) {
    printf("\n--- SUA MISSAO (Exercito %s) ---\n", corJogador);
    printf("%s\n", missao);
}


 //Verifica se a missão foi cumprida para o jogador identificado por corJogador.
 //Retorna 1 se cumprida, 0 caso contrário.

int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    int territorPorJogador = 0;
    int tropasTotais = 0;
    int territoriosComMaisQue5 = 0;

    // Conta territórios do jogador e tropas totais 
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territorPorJogador++;
            tropasTotais += mapa[i].tropas;
            if (mapa[i].tropas > 5) territoriosComMaisQue5++;
        }
    }

    // Verificações simples 
    if (strstr(missao, "Possuir pelo menos 3 territórios") != NULL) {
        return (territorPorJogador >= 3) ? 1 : 0;
    }
    if (strstr(missao, "Ter pelo menos 15 tropas no total") != NULL) {
        return (tropasTotais >= 15) ? 1 : 0;
    }
    if (strstr(missao, "Possuir a maioria dos territórios") != NULL) {
        return (territorPorJogador > (tamanho / 2)) ? 1 : 0;
    }
    if (strstr(missao, "Eliminar um exército inimigo") != NULL ||
        strstr(missao, "controlar todos os territórios") != NULL) {
        // Consideramos cumprida se o jogador controla todos os territorios 
        return (territorPorJogador == tamanho) ? 1 : 0;
    }
    if (strstr(missao, "Possuir 2 territórios com mais de 5 tropas") != NULL) {
        return (territoriosComMaisQue5 >= 2) ? 1 : 0;
    }

    // Missão desconhecida -> não cumprida 
    return 0;
}

// Libera toda a memória alocada dinamicamente 
void liberarMemoria(Territorio* mapa, int n, char** coresUnicas, int numCores, char** missoesJogadores, int numJogadores) {
    if (mapa) free(mapa);
    if (coresUnicas) {
        for (int i = 0; i < numCores; i++) {
            if (coresUnicas[i]) free(coresUnicas[i]);
        }
        free(coresUnicas);
    }
    if (missoesJogadores) {
        for (int i = 0; i < numJogadores; i++) {
            if (missoesJogadores[i]) free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}

//Pausa simples
void pausa() {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n'); 
}

// MAIN: fluxo do jogo 
int main() {
    srand((unsigned int) time(NULL));

    int n = 5; 
    Territorio *territorios = (Territorio*) malloc(n * sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memoria para territorios.\n");
        return 1;
    }

   
    char* missoesDisponiveis[] = {
        "Possuir pelo menos 3 territórios.",
        "Ter pelo menos 15 tropas no total.",
        "Possuir a maioria dos territórios.",
        "Eliminar um exército inimigo (controlar todos os territórios).",
        "Possuir 2 territórios com mais de 5 tropas cada."
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    // Cadastro dos territorios (leitura do usuário) 
    for (int i = 0; i < n; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);
        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        while (scanf("%d", &territorios[i].tropas) != 1) {
            printf("Entrada invalida. Digite um numero: ");
            while (getchar() != '\n');
        }
        getchar(); 
    }

    // Identifica cores unicas (cada cor = um jogador)
    char** coresUnicas = NULL;
    int numCores = 0;
    for (int i = 0; i < n; i++) {
        int achou = 0;
        for (int j = 0; j < numCores; j++) {
            if (strcmp(territorios[i].cor, coresUnicas[j]) == 0) {
                achou = 1; break;
            }
        }
        if (!achou) {
            coresUnicas = (char**) realloc(coresUnicas, (numCores + 1) * sizeof(char*));
            coresUnicas[numCores] = (char*) malloc(strlen(territorios[i].cor) + 1);
            strcpy(coresUnicas[numCores], territorios[i].cor);
            numCores++;
        }
    }

    // Aloca missoes dinamicamente para cada jogador e atribui 
    char** missoesJogadores = (char**) malloc(numCores * sizeof(char*));
    if (!missoesJogadores) {
        printf("Erro ao alocar memoria para missoes dos jogadores.\n");
        liberarMemoria(territorios, n, coresUnicas, numCores, NULL, 0);
        return 1;
    }
    for (int i = 0; i < numCores; i++) {
        missoesJogadores[i] = (char*) malloc(200 * sizeof(char)); //buffer para string
        if (!missoesJogadores[i]) {
            printf("Erro ao alocar memoria para uma missao.\n");
            liberarMemoria(territorios, n, coresUnicas, numCores, missoesJogadores, i);
            return 1;
        }
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    //Exibe mapa e missoes (cada missao exibida apenas uma vez) 
    exibirMapa(territorios, n);
    for (int i = 0; i < numCores; i++) {
        exibirMissao(missoesJogadores[i], coresUnicas[i]);
    }

    // Loop principal com menu parecido com as imagens 
    int escolha = -1;
    int jogoAtivo = 1;
    while (jogoAtivo) {
        printf("\n--- MENU DE ACOES ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha sua acao: ");
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Saindo...\n");
            break;
        }
        getchar(); 

        if (escolha == 0) {
            printf("Saindo do jogo...\n");
            break;
        } else if (escolha == 1) {
            // FASE DE ATAQUE 
            exibirMapa(territorios, n);
            printf("\n--- FASE DE ATAQUE ---\n");
            int atk, def;
            printf("Escolha o territorio atacante (1 a %d): ", n);
            if (scanf("%d", &atk) != 1) { printf("Entrada invalida.\n"); while(getchar() != '\n'); continue; }
            printf("Escolha o territorio defensor (1 a %d): ", n);
            if (scanf("%d", &def) != 1) { printf("Entrada invalida.\n"); while(getchar() != '\n'); continue; }
            getchar(); 

            atk--; def--;
            if (atk < 0 || atk >= n || def < 0 || def >= n || atk == def) {
                printf("Escolha invalida!\n");
                pausa();
                continue;
            }
            if (strcmp(territorios[atk].cor, territorios[def].cor) == 0) {
                printf("Nao e possivel atacar territorios do mesmo exército!\n");
                pausa();
                continue;
            }
            int dadoA = 0, dadoD = 0;
            atacar(&territorios[atk], &territorios[def], &dadoA, &dadoD);
            exibirMapa(territorios, n);

            // Verificacao silenciosa de missoes apos ataque 
            for (int j = 0; j < numCores; j++) {
                if (verificarMissao(missoesJogadores[j], territorios, n, coresUnicas[j])) {
                    printf("\n\n>>> MISSÃO CUMPRIDA! O exército %s cumpriu sua missão: <<<\n", coresUnicas[j]);
                    printf("%s\n", missoesJogadores[j]);
                    printf(">>> %s é declarado VENCEDOR! Parabens!\n\n", coresUnicas[j]);
                    jogoAtivo = 0;
                    break;
                }
            }
            if (!jogoAtivo) break;

            // Verifica dominacao total (apenas uma cor com tropas)
            int coresPresentes = 0;
            char ultimaCor[10] = "";
            for (int i = 0; i < n; i++) {
                if (territorios[i].tropas > 0) {
                    int jaCont = 0;
                    for (int k = 0; k < i; k++) {
                        if (strcmp(territorios[k].cor, territorios[i].cor) == 0) { jaCont = 1; break; }
                    }
                    if (!jaCont) {
                        coresPresentes++;
                        strcpy(ultimaCor, territorios[i].cor);
                    }
                }
            }
            if (coresPresentes == 1) {
                printf("\n>>> DOMINACAO: O exército %s controla todos os territórios e vence o jogo!\n", ultimaCor);
                jogoAtivo = 0;
                break;
            }

            pausa();
        } else if (escolha == 2) {
            // Verificar Missao (menu) - exibe se cada jogador cumpriu ou não (consulta direta)
            printf("\n--- VERIFICAR MISSAO ---\n");
            for (int i = 0; i < numCores; i++) {
                printf("\nExercito %s: \n", coresUnicas[i]);
                printf("Missao: %s\n", missoesJogadores[i]);
                if (verificarMissao(missoesJogadores[i], territorios, n, coresUnicas[i])) {
                    printf("Status: MISSÃO CUMPRIDA!\n");
                } else {
                    printf("Status: NAO CUMPRIDA.\n");
                }
            }
            pausa();
        } else {
            printf("Opcao invalida!\n");
            pausa();
        }
    }

    // Libera memória
    liberarMemoria(territorios, n, coresUnicas, numCores, missoesJogadores, numCores);

    return 0;
}
