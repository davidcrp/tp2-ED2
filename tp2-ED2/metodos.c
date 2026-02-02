#include "structs.h"
#define FIM_BLOCO -1.0


// metodo 1 - intercalacao balanceada 2f fitas
void metodo1(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 1: intercalacao balanceada 2f fitas ===\n");

    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    inicializarMetricas();
    clock_t inicio = clock();

   int numBlocos = selecaoPorSubstituicao(arquivo, 10);

    distribuirBlocosEmFitas(numBlocos);

    int passo = 0;
    while (numBlocos > 1) {
        numBlocos = realizarPassada2F(numBlocos, 10, passo);
        passo++;
    }


    FILE *orig = fopen(arquivo, "wb");
    FILE *res  = fopen("blocos/entrada_fita0.bin", "rb");

    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a)) {
            if (a.nota == FIM_BLOCO)
                break;
            escreverAlunoBinario(orig, &a);
        }
    }

    if (orig) fclose(orig);
    if (res) fclose(res);

    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    imprimirMetricas();

    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// metodo 2 - intercalacao balanceada f+1 fitas
void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 2: intercalacao balanceada f+1 fitas ===\n");

    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    inicializarMetricas();
    clock_t inicio = clock();

    int numBlocos = selecaoPorSubstituicao(arquivo, 19);

    distribuirBlocosEmFitasF1(numBlocos);

    int passo = 0;
    while (numBlocos > 1) {
        numBlocos = realizarPassadaF1(numBlocos, passo);
        passo++;
    }

    FILE *orig = fopen(arquivo, "wb");
    FILE *res  = fopen("blocos/saida_fita.bin", "rb");

    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a)) {
            if (a.nota == FIM_BLOCO)
                break;
            escreverAlunoBinario(orig, &a);
        }
    }

    if (orig) fclose(orig);
    if (res) fclose(res);

    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    imprimirMetricas();

    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// metodo 3 - quicksort externo
void metodo3(char *arquivo, int quantidade, int imprimir) {
    printf("\n=== metodo 3: quicksort externo ===\n");

    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    inicializarMetricas();

    FILE *arq = fopen(arquivo, "r+b");
    if (!arq) {
        printf("erro ao abrir arquivo\n");
        return;
    }

    clock_t inicio = clock();
    QuicksortExterno(arq, 1, quantidade);
    fclose(arq);

    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    imprimirMetricas();

    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    verificarOrdenacao(arquivo, quantidade);
}
