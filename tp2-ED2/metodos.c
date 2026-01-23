/*
 * metodos.c - Implementação dos 3 Métodos de Ordenação Externa
 */

#include "structs.h"

// ============================================================================
// MÉTODO 1: INTERCALAÇÃO BALANCEADA 2f FITASso
// ============================================================================

void metodo1(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 1: intercalacao balanceada 2f fitas ===\n");
    printf("memoria: 10 registros | fitas: 10 entrada + 10 saida\n\n");
    
    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    inicializarMetricas();
    clock_t inicio = clock();
    
    // gera as fitas iniciais usando selecao por substituicao
    int totalFitas = selecaoPorSubstituicao(arquivo, "fita", 10);
    
    printf("\nselecao por substituicao concluida\n");
    printf("total de fitas geradas: %d\n", totalFitas);
    
    // aqui devera ser implementada a intercalacao balanceada 2f
    // 10 fitas de entrada e 10 fitas de saida
    // enquanto totalFitas > 1, repetir as fases de intercalacao
    
    printf("intercalacao balanceada 2f ainda nao implementada\n");
    
    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("depois da ordenacao (apenas fitas geradas):\n");
        imprimirPrimeirosAlunos("fita0.bin", 10);
    }
}


// ============================================================================
// MÉTODO 2: INTERCALAÇÃO BALANCEADA f+1 FITAS
// ============================================================================

void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 2: intercalacao balanceada f+1 fitas ===\n");
    printf("memoria: 19 registros | fitas: 19 entrada + 1 saida\n\n");
    
    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    inicializarMetricas();
    clock_t inicio = clock();
    
    // gera as fitas iniciais usando selecao por substituicao
    int totalFitas = selecaoPorSubstituicao(arquivo, "fita", 19);
    
    printf("\nselecao por substituicao concluida\n");
    printf("total de fitas geradas: %d\n", totalFitas);
    
    // aqui devera ser implementada a intercalacao balanceada f+1
    // 19 fitas de entrada e 1 fita de saida
    // enquanto totalFitas > 1, repetir as fases de intercalacao
    
    printf("intercalacao balanceada f+1 ainda nao implementada\n");
    
    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("depois da ordenacao (apenas fitas geradas):\n");
        imprimirPrimeirosAlunos("fita0.bin", 10);
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