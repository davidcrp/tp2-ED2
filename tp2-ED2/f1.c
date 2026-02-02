#include "structs.h"

#define F 19
#define FIM_BLOCO -1.0

void distribuirBlocosEmFitasF1(int numBlocos) {

    FILE *fitas[F];
    char nome[MAX_NOME];

    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        fitas[i] = fopen(nome, "wb");
    }

    for (int b = 0; b < numBlocos; b++) {

        sprintf(nome, "blocos/passo0_bloco%d.bin", b);
        FILE *bloco = fopen(nome, "rb");
        if (!bloco) continue;

        int fita = b % F;
        Aluno a;

        while (lerAlunoBinario(bloco, &a))
            escreverAlunoBinario(fitas[fita], &a);

        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(fitas[fita], &fim);

        fclose(bloco);
    }

    for (int i = 0; i < F; i++)
        fclose(fitas[i]);
}

int realizarPassadaF1(int numBlocosIn, int passoAtual) {

    FILE *entrada[F];
    FILE *saida;
    char nome[MAX_NOME];

    // abre fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        entrada[i] = fopen(nome, "rb");
    }

    // abre fita de saida unica
    sprintf(nome, "blocos/saida_fita.bin");
    saida = fopen(nome, "wb");

    Heap *heap = criarHeap(F);
    int blocosGerados = 0;

    while (1) {

        // carrega inicio de UM bloco por fita
        for (int i = 0; i < F; i++) {
            if (!entrada[i]) continue;

            Aluno a;
            if (lerAlunoBinario(entrada[i], &a)) {
                if (a.nota != FIM_BLOCO)
                    inserirHeap(heap, a, 0, i);
            }
        }

        if (heap->tamanho == 0)
            break;

        // intercala UM BLOCO
        while (heap->tamanho > 0) {
            int origem;
            Aluno menor = removerMinHeap(heap, NULL, &origem);
            escreverAlunoBinario(saida, &menor);

            Aluno prox;
            if (lerAlunoBinario(entrada[origem], &prox)) {
                if (prox.nota != FIM_BLOCO)
                    inserirHeap(heap, prox, 0, origem);
            }
        }

        // fim de bloco
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(saida, &fim);

        blocosGerados++;
    }

    destruirHeap(heap);

    for (int i = 0; i < F; i++)
        if (entrada[i]) fclose(entrada[i]);

    fclose(saida);

    // redistribui saida como entradas (round-robin)
    FILE *novaEntrada[F];
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        novaEntrada[i] = fopen(nome, "wb");
    }

    saida = fopen("blocos/saida_fita.bin", "rb");

    int fitaAtual = 0;
    Aluno a;

    while (lerAlunoBinario(saida, &a)) {
        escreverAlunoBinario(novaEntrada[fitaAtual], &a);
        if (a.nota == FIM_BLOCO)
            fitaAtual = (fitaAtual + 1) % F;
    }

    fclose(saida);

    for (int i = 0; i < F; i++)
        fclose(novaEntrada[i]);

    if (blocosGerados > 1) 
        remove("blocos/saida_fita.bin"); // teste commit




    return blocosGerados;
}
