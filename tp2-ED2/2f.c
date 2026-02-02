#define F 10
#define FIM_BLOCO -1.0

#include "structs.h"
void distribuirBlocosEmFitas(int numBlocos) {

    FILE *fitas[F];
    char nome[MAX_NOME];

    // cria/zera fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        fitas[i] = fopen(nome, "wb");
    }

    // distribui blocos round-robin
    for (int b = 0; b < numBlocos; b++) {

        sprintf(nome, "blocos/passo0_bloco%d.bin", b);
        FILE *bloco = fopen(nome, "rb");
        if (!bloco) continue;

        int fita = b % F;
        Aluno a;

        while (lerAlunoBinario(bloco, &a))
            escreverAlunoBinario(fitas[fita], &a);

        // marca fim do bloco
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(fitas[fita], &fim);

        fclose(bloco);
    }

    for (int i = 0; i < F; i++)
        fclose(fitas[i]);
}

int realizarPassada2F(int numBlocosIn, int grau, int passoAtual) {

    FILE *entrada[F], *saida[F];
    char nome[MAX_NOME];

    // abre fitas de entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        entrada[i] = fopen(nome, "rb");
    }

    // cria/zera fitas de saida
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/saida_fita%d.bin", i);
        saida[i] = fopen(nome, "wb");
    }

    Heap *heap = criarHeap(F);
    int blocosGerados = 0;
    int fitaSaida = 0;

    while (1) {

        // carrega inicio de um bloco de cada fita
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
            escreverAlunoBinario(saida[fitaSaida], &menor);

            Aluno prox;
            if (lerAlunoBinario(entrada[origem], &prox)) {
                if (prox.nota != FIM_BLOCO)
                    inserirHeap(heap, prox, 0, origem);
            }
        }

        // marca fim do bloco na fita de saida
        Aluno fim;
        fim.nota = FIM_BLOCO;
        escreverAlunoBinario(saida[fitaSaida], &fim);

        fitaSaida = (fitaSaida + 1) % F;
        blocosGerados++;
    }

    destruirHeap(heap);

    for (int i = 0; i < F; i++) {
        if (entrada[i]) fclose(entrada[i]);
        if (saida[i]) fclose(saida[i]);
    }

    // troca papeis: saida vira entrada
    for (int i = 0; i < F; i++) {
        sprintf(nome, "blocos/entrada_fita%d.bin", i);
        remove(nome);

        char nomeSaida[MAX_NOME];
        sprintf(nomeSaida, "blocos/saida_fita%d.bin", i);
        rename(nomeSaida, nome);
    }

    return blocosGerados;
}
