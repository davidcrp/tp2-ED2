#define F 10
#define FIM_BLOCO -1.0

#include "structs.h"

// distribui os blocos iniciais gerados pela selecao por substituicao
void distribuirBlocosEmFitas(int numBlocos) {

    FILE *fitas[F];
    char nome[MAX_NOME];

    // cria e zera todas as fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        fitas[i] = fopen(nome, "wb");
    }

    // percorre todos os blocos gerados na selecao por substituicao
    for (int b = 0; b < numBlocos; b++) {

        // abre o bloco b gerado na fase inicial
        sprintf(nome, "blocos/passo0_bloco%d.bin", b);
        FILE *bloco = fopen(nome, "rb");
        if (!bloco) continue;

        // define a fita de destino usando distribuicao circular
        int fita = b % F;
        Aluno a;

        // copia todos os registros do bloco para a fita correspondente
        while (lerAlunoBinario(bloco, &a))
            escreverAlunoBinario(fitas[fita], &a);

        // indica o fim do bloco
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(fitas[fita], &fim);

        fclose(bloco);
    }

    // fecha todas as fitas de entrada
    for (int i = 0; i < F; i++)
        fclose(fitas[i]);
}


// le blocos das f fitas de entrada e gera novos blocos, distribuidos nas f fitas de saida
int realizarPassada2F(int numBlocosIn, int grau, int passoAtual) {

    FILE *entrada[F], *saida[F];
    char nome[MAX_NOME];

    // abre as f fitas de entrada para leitura
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        entrada[i] = fopen(nome, "rb");
    }

    // cria e zera as f fitas de saida
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/saida_fita%d.bin", i);
        saida[i] = fopen(nome, "wb");
    }

    // cria heap para comparacao
    Heap *heap = criarHeap(F);

    int blocosGerados = 0;

    // indica qual fita de saida sera utilizada no momento
    int fitaSaida = 0;

    // repete enquanto ainda houver blocos a serem intercalados
    while (1) {

        // carrega o inicio de um bloco de cada fita de entrada
        for (int i = 0; i < F; i++) {
            if (!entrada[i]) continue;

            Aluno a;
            if (lerAlunoBinario(entrada[i], &a)) {
                // ignora o sentinela de fim de bloco
                if (a.nota != FIM_BLOCO)
                    inserirHeap(heap, a, 0, i);
            }
        }

        // se nao foi inserido nenhum registro no heap, encerra a passada
        if (heap->tamanho == 0)
            break;

        // intercalacao de um unico bloco
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);

            // escreve o menor registro na fita de saida atual
            escreverAlunoBinario(saida[fitaSaida], &menor);

            // le o proximo registro da fita de onde veio o menor
            Aluno prox;
            if (lerAlunoBinario(entrada[origem], &prox)) {
                if (prox.nota != FIM_BLOCO)
                    inserirHeap(heap, prox, 0, origem);
            }
        }

        // indica fim do bloco na fita de saida
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(saida[fitaSaida], &fim);

        // alterna a fita de saida de forma circular
        fitaSaida = (fitaSaida + 1) % F;

        blocosGerados++;
    }

    // libera a memoria do heap
    destruirHeap(heap);

    // fecha todas as fitas de entrada e saida
    for (int i = 0; i < F; i++) {
        if (entrada[i]) fclose(entrada[i]);
        if (saida[i]) fclose(saida[i]);
    }

    // inverter fitas, as fitas de saida passam a ser as novas fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        remove(nome);

        char nomeSaida[MAX_NOME];
        sprintf(nomeSaida, "blocos/saida_fita%d.bin", i);
        rename(nomeSaida, nome);
    }

    return blocosGerados;
}
