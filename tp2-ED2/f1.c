#include "structs.h"

#define F 19
#define FIM_BLOCO -1.0


// distribui os blocos iniciais gerados pela selecao por substituicao nas f fitas de entrada do metodo f+1
void distribuirBlocosEmFitasF1(int numBlocos) {

    FILE *fitas[F];
    char nome[MAX_NOME];

    // cria e zera todas as fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        fitas[i] = fopen(nome, "wb");
    }

    // percorre todos os blocos gerados na fase inicial
    for (int b = 0; b < numBlocos; b++) {

        // abre o bloco b gerado na selecao por substituicao
        sprintf(nome, "blocos/passo0_bloco%d.bin", b);
        FILE *bloco = fopen(nome, "rb");
        if (!bloco) continue;

        // define a fita de destino usando distribuicao circular
        int fita = b % F;
        Aluno a;

        // copia todos os registros do bloco para a fita correspondente
        while (lerAlunoBinario(bloco, &a))
            escreverAlunoBinario(fitas[fita], &a);

        //  indica o fim do bloco
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(fitas[fita], &fim);

        fclose(bloco);
    }

    // fecha todas as fitas de entrada
    for (int i = 0; i < F; i++)
        fclose(fitas[i]);
}


// le blocos das f fitas de entrada e escreve os resultados  em uma unica fita de saida
int realizarPassadaF1(int numBlocosIn, int passoAtual) {

    FILE *entrada[F];
    FILE *saida;
    char nome[MAX_NOME];

    // abre todas as f fitas de entrada para leitura
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        entrada[i] = fopen(nome, "rb");
    }

    // cria e zera a unica fita de saida
    sprintf(nome, "blocos/saida_fita.bin");
    saida = fopen(nome, "wb");

    // cria heap minima para intercalacao dos registros
    Heap *heap = criarHeap(F);

    // contador de blocos gerados nesta passada
    int blocosGerados = 0;

    // repete enquanto ainda houver blocos a serem intercalados
    while (1) {

        // carrega o inicio de um bloco de cada fita de entrada
        for (int i = 0; i < F; i++) {
            if (!entrada[i]) continue;

            Aluno a;
            if (lerAlunoBinario(entrada[i], &a)) {
                // verifica se o registro lido e um registro valido do bloco
                if (a.nota != FIM_BLOCO)
                    inserirHeap(heap, a, 0, i);
            }
        }

        // se nenhum registro foi inserido no heap, encerra a passada
        if (heap->tamanho == 0)
            break;

        // intercalacao de um unico bloco
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);

            // escreve o menor registro na fita de saida
            escreverAlunoBinario(saida, &menor);

            // le o proximo registro da fita de onde veio o menor
            Aluno prox;
            if (lerAlunoBinario(entrada[origem], &prox)) {
                if (prox.nota != FIM_BLOCO)
                    inserirHeap(heap, prox, 0, origem);
            }
        }

        // eindica fim do bloco na fita de saida
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(saida, &fim);

        blocosGerados++;
    }

    // libera a memoria utilizada pelo heap
    destruirHeap(heap);

    // fecha todas as fitas de entrada
    for (int i = 0; i < F; i++)
        if (entrada[i]) fclose(entrada[i]);

    // fecha a fita de saida
    fclose(saida);

    // redistribui os blocos da fita de saida
    // novamente nas f fitas de entrada 
    FILE *novaEntrada[F];
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        novaEntrada[i] = fopen(nome, "wb");
    }

    // reabre a fita de saida para leitura
    saida = fopen("blocos/saida_fita.bin", "rb");

    int fitaAtual = 0;
    Aluno a;

    // copia os registros da fita de saida para as novas fitas de entrada
    while (lerAlunoBinario(saida, &a)) {
        escreverAlunoBinario(novaEntrada[fitaAtual], &a);

        // ao encontrar o fim, avanca para a proxima fita
        if (a.nota == FIM_BLOCO)
            fitaAtual = (fitaAtual + 1) % F;
    }

    // fecha a fita de saida
    fclose(saida);

    // fecha todas as novas fitas de entrada
    for (int i = 0; i < F; i++)
        fclose(novaEntrada[i]);

    // remove a fita de saida apenas se nao for a ultima passada
    if (blocosGerados > 1) 
        remove("blocos/saida_fita.bin");

    // retorna quantos blocos foram gerados nesta passada
    return blocosGerados;
}
