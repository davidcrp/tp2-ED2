/*
 * metodos.c - Implementação dos 3 Métodos de Ordenação Externa
 */

#include "structs.h"

// ============================================================================
// PASSADA MÉTODO 1 - 2f FITAS  → grava em pasta 2f/
// ============================================================================

int realizarPassada(int numBlocosIn, int grau, int passoAtual) {
    int blocosGerados = 0;
    int processados = 0;

    while (processados < numBlocosIn) {

        int qtdInput = grau;
        if (numBlocosIn - processados < grau)
            qtdInput = numBlocosIn - processados;

        FILE **entradas = (FILE**) malloc(qtdInput * sizeof(FILE*));
        char nomeIn[MAX_NOME];

        for (int i = 0; i < qtdInput; i++) {
            if (passoAtual == 0)
                sprintf(nomeIn, "blocos/passo0_bloco%d.bin", processados + i);
            else
                sprintf(nomeIn, "2f/passo%d_bloco%d.bin", passoAtual, processados + i);

            entradas[i] = fopen(nomeIn, "rb");
        }

        char nomeOut[MAX_NOME];
        sprintf(nomeOut, "2f/passo%d_bloco%d.bin", passoAtual + 1, blocosGerados);
        FILE *saida = fopen(nomeOut, "wb");

        Heap *heap = criarHeap(qtdInput);
        Aluno reg;

        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i] && lerAlunoBinario(entradas[i], &reg)) {
                inserirHeap(heap, reg, 0, i);
            }
        }

        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            escreverAlunoBinario(saida, &menor);

            if (lerAlunoBinario(entradas[origem], &reg))
                inserirHeap(heap, reg, 0, origem);
        }

        destruirHeap(heap);
        fclose(saida);
        for (int i = 0; i < qtdInput; i++)
            if (entradas[i]) fclose(entradas[i]);

        free(entradas);

        processados += qtdInput;
        blocosGerados++;
    }

    return blocosGerados;
}

// ============================================================================
// PASSADA MÉTODO 2 - f+1 FITAS → grava em pasta 2f1/
// ============================================================================

int realizarPassadaF1(int numBlocosIn, int passoAtual) {
    int blocosGerados = 0;
    int processados = 0;

    while (processados < numBlocosIn) {

        int qtdInput = 19;
        if (numBlocosIn - processados < 19)
            qtdInput = numBlocosIn - processados;

        FILE **entradas = (FILE**) malloc(qtdInput * sizeof(FILE*));
        char nomeIn[MAX_NOME];

        for (int i = 0; i < qtdInput; i++) {
            if (passoAtual == 0)
                sprintf(nomeIn, "blocos/passo0_bloco%d.bin", processados + i);
            else
                sprintf(nomeIn, "f1/passo%d_bloco%d.bin", passoAtual, processados + i);

            entradas[i] = fopen(nomeIn, "rb");
        }

        char nomeOut[MAX_NOME];
        sprintf(nomeOut, "f1/passo%d_bloco%d.bin", passoAtual + 1, blocosGerados);
        FILE *saida = fopen(nomeOut, "wb");

        Heap *heap = criarHeap(qtdInput);
        Aluno reg;

        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i] && lerAlunoBinario(entradas[i], &reg))
                inserirHeap(heap, reg, 0, i);
        }

        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            escreverAlunoBinario(saida, &menor);

            if (lerAlunoBinario(entradas[origem], &reg))
                inserirHeap(heap, reg, 0, origem);
        }

        destruirHeap(heap);
        fclose(saida);
        for (int i = 0; i < qtdInput; i++)
            if (entradas[i]) fclose(entradas[i]);

        free(entradas);

        processados += qtdInput;
        blocosGerados++;
    }

    return blocosGerados;
}

// ============================================================================
// MÉTODO 1 - INTERCALAÇÃO BALANCEADA 2f FITAS
// ============================================================================

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

    printf("\nselecao por substituicao concluida\n");
    printf("total de blocos gerados: %d\n", numBlocos);

    while (numBlocos > 1) {
        numBlocos = realizarPassada(numBlocos, 10, passo);
        passo++;
        printf("Passada %d concluida. Blocos restantes: %d\n", passo, numBlocos);
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

    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    imprimirMetricas();

    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}

// ============================================================================
// MÉTODO 2 - INTERCALAÇÃO BALANCEADA f+1 FITAS
// ============================================================================

void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== Metodo 2: Intercalacao Balanceada f+1 Fitas ===\n");

    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    inicializarMetricas();
    clock_t inicio = clock();

    int numBlocos = selecaoPorSubstituicao(arquivo, 19);
    int passo = 0;

    printf("\nselecao por substituicao concluida\n");
    printf("total de blocos gerados: %d\n", numBlocos);

    while (numBlocos > 1) {
        numBlocos = realizarPassadaF1(numBlocos, passo);
        passo++;
        printf("Passada %d concluida. Blocos restantes: %d\n", passo, numBlocos);
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

    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    imprimirMetricas();

    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// ============================================================================
// MÉTODO 3: QUICKSORT EXTERNO
// ============================================================================

void metodo3(char *arquivo, int quantidade, int imprimir) {
    printf("\n=== MÉTODO 3: Quicksort Externo ===\n");
    printf("Memória: 10 registros | Algoritmo: Monard (1980)\n\n");
    
    if (imprimir) {
        printf("ANTES DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    inicializarMetricas();
    
    FILE *arq = fopen(arquivo, "r+b");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivo);
        return;
    }
    
    clock_t inicio = clock();
    QuicksortExterno(arq, 1, quantidade);
    clock_t fim = clock();
    
    fclose(arq);
    
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("DEPOIS DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    printf("\nVerificando ordenação...\n");
    verificarOrdenacao(arquivo, quantidade);
}