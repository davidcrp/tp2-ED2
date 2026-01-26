/*
 * metodos.c - Implementação dos 3 Métodos de Ordenação Externa
 */

#include "structs.h"

// Funcao auxiliar para realizar uma passada de intercalacao
// Pega 'grau' blocos da entrada (arquivos separados) e gera 1 bloco de saida a
int realizarPassada(int numBlocosIn, int grau, int passoAtual) {
    int blocosGerados = 0;
    int processados = 0;
    
    while (processados < numBlocosIn) {
        // Quantos arquivos vamos abrir agora? (No maximo 'grau')
        int qtdInput = grau;
        if (numBlocosIn - processados < grau) {
            qtdInput = numBlocosIn - processados;
        }

        FILE **entradas = (FILE**) malloc(qtdInput * sizeof(FILE*));
        char nomeIn[MAX_NOME];

        // Abre os arquivos de entrada
        for (int i = 0; i < qtdInput; i++) {
            sprintf(nomeIn, "passo%d_bloco%d.bin", passoAtual, processados + i);
            entradas[i] = fopen(nomeIn, "rb");
        }

        char nomeOut[MAX_NOME];
        sprintf(nomeOut, "passo%d_bloco%d.bin", passoAtual + 1, blocosGerados);
        FILE *saida = fopen(nomeOut, "wb");

        // Heap para intercalacao (tamanho = numero de arquivos abertos)
        Heap *heap = criarHeap(qtdInput); 
        Aluno reg;

        // Preenche o heap com o primeiro de cada arquivo
        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i] && lerAlunoBinario(entradas[i], &reg)) {
                inserirHeap(heap, reg, 0, i); // i eh o indice da origem
            }
        }

        // Loop de Merge
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            
            escreverAlunoBinario(saida, &menor);

            // Repoe do mesmo arquivo
            if (lerAlunoBinario(entradas[origem], &reg)) {
                inserirHeap(heap, reg, 0, origem);
            }
        }

        destruirHeap(heap);
        fclose(saida);
        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i]) fclose(entradas[i]);
        }
        free(entradas);

        processados += qtdInput;
        blocosGerados++;
    }

    return blocosGerados;
}

// ============================================================================
// MÉTODO 1: INTERCALAÇÃO BALANCEADA 2f FITAS
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
    
    // gera os blocos iniciais (passo 0)
    // aqui usamos 10 fitas virtuais de entrada (tamanho memoria 10)
    int numBlocos = selecaoPorSubstituicao(arquivo, 10);
    int passo = 0;
    
    printf("\nselecao por substituicao concluida\n");
    printf("total de blocos gerados: %d\n", numBlocos);
    
    // aqui devera ser implementada a intercalacao balanceada 2f
    // 10 fitas de entrada e 10 fitas de saida
    // enquanto totalFitas > 1, repetir as fases de intercalacao
    
    while (numBlocos > 1) {
        // Intercala de 10 em 10
        numBlocos = realizarPassada(numBlocos, 10, passo);
        passo++;
        printf("Passada %d concluida. Blocos restantes: %d\n", passo, numBlocos);
    }
    
    // Copia o resultado final para o arquivo original
    char nomeFinal[MAX_NOME];
    sprintf(nomeFinal, "passo%d_bloco0.bin", passo);
    
    FILE *orig = fopen(arquivo, "wb");
    FILE *res = fopen(nomeFinal, "rb");
    Aluno a;
    if (res && orig) {
        while(lerAlunoBinario(res, &a)) escreverAlunoBinario(orig, &a);
    }
    if(orig) fclose(orig);
    if(res) fclose(res);

    clock_t fim = clock();
    metricas.tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    imprimirMetricas();
    
    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// ============================================================================
// MÉTODO 2: INTERCALAÇÃO BALANCEADA f+1 FITAS
// ============================================================================

void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== Metodo 2: Intercalacao Balanceada f+1 Fitas ===\n");
    printf("memoria: 19 registros | fitas: 19 entrada + 1 saida\n\n");
    
    
    // f+1 fitas...
    
    inicializarMetricas();
    imprimirMetricas();
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