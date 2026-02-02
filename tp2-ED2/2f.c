#include "structs.h"

// realiza uma passada de intercalacao do metodo 2f
// recebe o numero de blocos de entrada, o grau f e o passo atual
// retorna quantos blocos foram gerados na passada
int realizarPassada2F(int numBlocosIn, int grau, int passoAtual) {
    int blocosGerados = 0;
    int processados = 0;

    // processa os blocos enquanto ainda houver blocos de entrada
    while (processados < numBlocosIn) {

        // define quantos arquivos de entrada serao usados
        int qtdInput = grau;
        if (numBlocosIn - processados < grau)
            qtdInput = numBlocosIn - processados;

        // vetor de arquivos de entrada
        FILE **entradas = (FILE**) malloc(qtdInput * sizeof(FILE*));
        char nomeIn[MAX_NOME];

        // abre os arquivos de entrada conforme o passo
        for (int i = 0; i < qtdInput; i++) {
            if (passoAtual == 0)
                sprintf(nomeIn, "blocos/passo0_bloco%d.bin", processados + i);
            else
                sprintf(nomeIn, "2f/passo%d_bloco%d.bin", passoAtual, processados + i);

            entradas[i] = fopen(nomeIn, "rb");
        }

        // cria o arquivo de saida da intercalacao
        char nomeOut[MAX_NOME];
        sprintf(nomeOut, "2f/passo%d_bloco%d.bin", passoAtual + 1, blocosGerados);
        FILE *saida = fopen(nomeOut, "wb");

        // cria heap minima para intercalacao
        Heap *heap = criarHeap(qtdInput);
        Aluno reg;

        // carrega o primeiro registro de cada entrada no heap
        for (int i = 0; i < qtdInput; i++) {
            if (entradas[i] && lerAlunoBinario(entradas[i], &reg)) {
                inserirHeap(heap, reg, 0, i);
            }
        }

        // intercalacao propriamente dita
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            escreverAlunoBinario(saida, &menor);

            if (lerAlunoBinario(entradas[origem], &reg))
                inserirHeap(heap, reg, 0, origem);
        }

        // libera recursos
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
