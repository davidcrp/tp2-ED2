#include "structs.h"
#define FIM_BLOCO -1.0


// metodo 1 - intercalacao balanceada 2f fitas
void metodo1(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 1: intercalacao balanceada 2f fitas ===\n");

    // imprime os primeiros registros antes da ordenacao
    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    inicializarMetricas();

    // marca o tempo inicial de execucao
    clock_t inicio = clock();

    // gera os blocos iniciais ordenados usando selecao por substituicao
    int numBlocos = selecaoPorSubstituicao(arquivo, 10);

    // distribui os blocos no metodo 2f
    distribuirBlocosEmFitas(numBlocos);

    // executa as passadas de intercalacao enquanto houver mais de um bloco
    int passo = 0;
    while (numBlocos > 1) {
        // realiza uma passada de intercalacao balanceada 2f
        numBlocos = realizarPassada2F(numBlocos, 10, passo);
        passo++;
    }

    // abre o arquivo original para escrita
    FILE *orig = fopen(arquivo, "wb");

    // abre a fita final que contem o bloco totalmente ordenado
    FILE *res  = fopen("blocos/entrada_fita0.bin", "rb");

    // copia os registros ordenados da fita final para o arquivo original
    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a)) {
            if (a.nota == FIM_BLOCO)
                break;
            escreverAlunoBinario(orig, &a);
        }
    }

    // fecha os arquivos utilizados
    if (orig) fclose(orig);
    if (res) fclose(res);

    // calcula o tempo total de execucao do metodo
    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;

    imprimirMetricas();

    // imprime os primeiros registros apos a ordenacao
    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// metodo 2 - intercalacao balanceada f+1 fitas
void metodo2(char *arquivo, int quantidade, int situacao, int imprimir) {
    printf("\n=== metodo 2: intercalacao balanceada f+1 fitas ===\n");

    // imprime os primeiros registros antes da ordenacao
    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    // inicializa contadores de metricas
    inicializarMetricas();

    // marca o tempo inicial de execucao
    clock_t inicio = clock();

    // gera os blocos iniciais ordenados usando selecao por substituicao
    int numBlocos = selecaoPorSubstituicao(arquivo, 19);

    // distribui os blocos iniciais nas fitas de entrada do metodo f+1
    distribuirBlocosEmFitasF1(numBlocos);

    // executa as passadas de intercalacao enquanto houver mais de um bloco
    int passo = 0;
    while (numBlocos > 1) {
        // realiza uma passada do metodo f+1
        numBlocos = realizarPassadaF1(numBlocos, passo);
        passo++;
    }

    // abre o arquivo original para escrita
    FILE *orig = fopen(arquivo, "wb");

    // abre a fita de saida final que contem o bloco totalmente ordenado
    FILE *res;

    if (situacao == 1) {
        res = fopen("blocos/passo0_bloco0.bin", "rb");
    } else {
        res = fopen("blocos/saida_fita.bin", "rb");
    }

    // copia os registros ordenados da fita final para o arquivo original
    Aluno a;
    if (orig && res) {
        while (lerAlunoBinario(res, &a)) {
            if (a.nota == FIM_BLOCO)
                break;
            escreverAlunoBinario(orig, &a);
        }
    }

    // fecha os arquivos utilizados
    if (orig) fclose(orig);
    if (res) fclose(res);

    // calcula o tempo total de execucao do metodo
    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;

    // imprime as metricas coletadas
    imprimirMetricas();

    // imprime os primeiros registros apos a ordenacao
    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }
}


// metodo 3 - quicksort externo
void metodo3(char *arquivo, int quantidade, int imprimir) {
    printf("\n=== metodo 3: quicksort externo ===\n");

    // imprime os primeiros registros antes da ordenacao
    if (imprimir) {
        printf("antes da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    // inicializa contadores de metricas
    inicializarMetricas();

    // abre o arquivo para leitura e escrita binaria
    FILE *arq = fopen(arquivo, "r+b");
    if (!arq) {
        printf("erro ao abrir arquivo\n");
        return;
    }

    // marca o tempo inicial de execucao
    clock_t inicio = clock();

    // executa o quicksort externo considerando o tamanho maximo da memoria
    QuicksortExterno(arq, 1, quantidade);

    // fecha o arquivo
    fclose(arq);

    // tempo total
    metricas.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;

    imprimirMetricas();

    // imprime os primeiros registros apos a ordenacao
    if (imprimir) {
        printf("depois da ordenacao:\n");
        imprimirPrimeirosRegistros(arquivo, 10);
    }

    verificarOrdenacao(arquivo, quantidade);
}
