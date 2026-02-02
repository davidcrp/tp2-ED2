#include "structs.h"

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
    int passo = 0;

    while (numBlocos > 1) {
        numBlocos = realizarPassada2F(numBlocos, 10, passo);
        passo++;
    }

    char nomeFinal[MAX_NOME];
    if (passo == 0)
        sprintf(nomeFinal, "blocos/passo0_bloco0.bin");
    else
        sprintf(nomeFinal, "2f/passo%d_bloco0.bin", passo);

    FILE *orig = fopen(arquivo, "wb");
    FILE *res  = fopen(nomeFinal, "rb");

    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a))
            escreverAlunoBinario(orig, &a);
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
    int passo = 0;

    while (numBlocos > 1) {
        numBlocos = realizarPassadaF1(numBlocos, passo);
        passo++;
    }

    char nomeFinal[MAX_NOME];
    if (passo == 0)
        sprintf(nomeFinal, "blocos/passo0_bloco0.bin");
    else
        sprintf(nomeFinal, "f1/passo%d_bloco0.bin", passo);

    FILE *orig = fopen(arquivo, "wb");
    FILE *res  = fopen(nomeFinal, "rb");

    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a))
            escreverAlunoBinario(orig, &a);
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
