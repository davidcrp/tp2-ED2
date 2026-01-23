/*
 * metodos.c - Implementação dos 3 Métodos de Ordenação Externa
 */

#include "structs.h"

// ============================================================================
// MÉTODO 1: INTERCALAÇÃO BALANCEADA 2f FITAS
// ============================================================================

void metodo1(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== MÉTODO 1: Intercalação Balanceada 2f Fitas ===\n");
    printf("Memória: 10 registros | Fitas: 10 entrada + 10 saída\n\n");
    
    if (imprimir) {
        printf("ANTES DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    inicializarMetricas();
    clock_t inicio = clock();
    
    // TODO: Implementar método 1
    printf("⚠ Método 1 ainda não implementado!\n");
    
    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("DEPOIS DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}

// ============================================================================
// MÉTODO 2: INTERCALAÇÃO BALANCEADA f+1 FITAS
// ============================================================================

void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== MÉTODO 2: Intercalação Balanceada f+1 Fitas ===\n");
    printf("Memória: 19 registros | Fitas: 19 entrada + 1 saída\n\n");
    
    if (imprimir) {
        printf("ANTES DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
    
    inicializarMetricas();
    clock_t inicio = clock();
    
    // TODO: Implementar método 2
    printf("⚠ Método 2 ainda não implementado!\n");
    
    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("DEPOIS DA ORDENAÇÃO:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}

// ============================================================================
// MÉTODO 3: QUICKSORT EXTERNO
// ============================================================================

void metodo3(char *arquivo, int quantidade, int situacao, int imprimir) {
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