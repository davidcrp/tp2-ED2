#include "structs.h"

// realiza uma passada do metodo f+1
// utiliza ate 19 arquivos de entrada e 1 de saida
int realizarPassadaF1(int numBlocosIn, int passoAtual) {
    int blocosGerados = 0;
    int processados = 0;

    // processa enquanto houver blocos de entrada
    while (processados < numBlocosIn) {

        // numero maximo de entradas no metodo f+1
        int qtdInput = 19;
        if (numBlocosIn - processados < 19)
            qtdInput = numBlocosIn - processados;

        FILE **entradas = (FILE**) malloc(qtdInput * sizeof(FILE*));
        char nomeIn[MAX_NOME];

        // abre os arquivos de entrada
        for (int i = 0; i < qtdInput; i++) {
            if (passoAtual == 0)
                sprintf(nomeIn, "blocos/passo0_bloco%d.bin", processados + i);
            else
                sprintf(nomeIn, "f1/passo%d_bloco%d.bin", passoAtual, processados + i);

            entradas[i] = fopen(nomeIn, "rb");
        }

        // arquivo de saida
        char nomeOut[MAX_NOME];
        sprintf(nomeOut, "f1/passo%d_bloco%d.bin", passoAtual + 1, blocosGerados);
        FILE *saida = fopen(nomeOut, "wb");

        // heap minima para intercalacao
        Heap *heap = criarHeap(qtdInput);
        Aluno reg;

        // carrega um registro de cada entrada
        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i] && lerAlunoBinario(entradas[i], &reg))
                inserirHeap(heap, reg, 0, i);
        }

        // intercalacao
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            escreverAlunoBinario(saida, &menor);

            if (lerAlunoBinario(entradas[origem], &reg))
                inserirHeap(heap, reg, 0, origem);
        }

        // libera memoria e arquivos
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
