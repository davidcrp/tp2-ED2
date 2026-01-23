#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "auxiliar.c"

#define MAX_FITAS 20

// estrutura do heap minimo com marcacao
typedef struct{
    Aluno *alunos;   // vetor de registros em memoria
    int tamanho;     // quantidade atual de elementos no heap
    int capacidade;  // capacidade maxima do heap (memoria)
    int *marcados;   // vetor para marcar elementos do proximo bloco
} Heap;

// cria o heap com capacidade maxima definida
Heap* criarHeap(int capacidade){
    Heap *h = (Heap*) malloc(sizeof(Heap));
    h->alunos = (Aluno*) malloc(capacidade * sizeof(Aluno));
    h->marcados = (int*) malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    return h;
}

// libera toda a memoria alocada pelo heap
void destruirHeap(Heap *h){
    free(h->alunos);
    free(h->marcados);
    free(h);
}

// troca dois elementos de posicao dentro do heap
void trocar(Heap *h, int i, int j){
    Aluno temp = h->alunos[i];
    h->alunos[i] = h->alunos[j];
    h->alunos[j] = temp;

    int tempMarcado = h->marcados[i];
    h->marcados[i] = h->marcados[j];
    h->marcados[j] = tempMarcado;
}

// ajusta o heap de cima para baixo para manter a propriedade do heap minimo
void heapParaBaixo(Heap *h, int i){
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    // compara com o filho da esquerda
    if(esq < h->tamanho){
        if(h->marcados[esq] < h->marcados[menor] ||
           (h->marcados[esq] == h->marcados[menor] &&
            compararAlunos(&h->alunos[esq], &h->alunos[menor]) < 0)){
            menor = esq;
        }
    }

    // compara com o filho da direita
    if(dir < h->tamanho){
        if(h->marcados[dir] < h->marcados[menor] ||
           (h->marcados[dir] == h->marcados[menor] &&
            compararAlunos(&h->alunos[dir], &h->alunos[menor]) < 0)){
            menor = dir;
        }
    }

    // se algum filho for menor, troca e continua ajustando
    if(menor != i){
        trocar(h, i, menor);
        heapParaBaixo(h, menor);
    }
}

// ajusta o heap de baixo para cima para manter a propriedade do heap minimo
void heapParaCima(Heap *h, int i){
    while(i > 0){
        int pai = (i - 1) / 2;

        if(h->marcados[i] < h->marcados[pai] ||
           (h->marcados[i] == h->marcados[pai] &&
            compararAlunos(&h->alunos[i], &h->alunos[pai]) < 0)){
            trocar(h, i, pai);
            i = pai;
        } else {
            break;
        }
    }
}

// insere um novo elemento no heap respeitando a ordem
void inserirHeap(Heap *h, Aluno aluno, int marcado){
    if(h->tamanho < h->capacidade){
        h->alunos[h->tamanho] = aluno;
        h->marcados[h->tamanho] = marcado;
        heapParaCima(h, h->tamanho);
        h->tamanho++;
    }
}

// remove o menor elemento do heap
Aluno removerMinHeap(Heap *h, int *marcado){
    Aluno min = h->alunos[0];
    *marcado = h->marcados[0];

    h->tamanho--;

    if(h->tamanho > 0){
        h->alunos[0] = h->alunos[h->tamanho];
        h->marcados[0] = h->marcados[h->tamanho];
        heapParaBaixo(h, 0);
    }

    return min;
}

// substitui o menor elemento do heap por um novo registro
void substituirMinHeap(Heap *h, Aluno novoAluno, int marcado){
    h->alunos[0] = novoAluno;
    h->marcados[0] = marcado;
    heapParaBaixo(h, 0);
}

// implementacao da selecao por substituicao
int selecaoPorSubstituicao(char *arquivoEntrada, char *prefixoFitas, int M){
    FILE *in = fopen(arquivoEntrada, "rb");
    if (!in) {
        printf("erro ao abrir arquivo de entrada\n");
        exit(1);
    }

    Heap *heap = criarHeap(M);
    Aluno aluno;
    int totalFitas = 0;

    // carrega os m primeiros registros na memoria
    for(int i = 0; i < M; i++){
        if(lerAlunoBinario(in, &aluno))
            inserirHeap(heap, aluno, 0);
        else
            break;
    }

    if(heap->tamanho == 0){
        fclose(in);
        destruirHeap(heap);
        return 0;
    }

    // cria a primeira fita de saida
    char nomeFita[50];
    sprintf(nomeFita, "%s%d.bin", prefixoFitas, totalFitas);
    FILE *out = fopen(nomeFita, "wb");

    // armazena a ultima nota gravada na fita atual
    float ultimaNota = -1e9;

    // loop principal da selecao por substituicao
    while(heap->tamanho > 0){

        int marcado;
        Aluno min = removerMinHeap(heap, &marcado);

        // se o menor estiver marcado, inicia nova fita
        if(marcado){
            fclose(out);
            totalFitas++;
            sprintf(nomeFita, "%s%d.bin", prefixoFitas, totalFitas);
            out = fopen(nomeFita, "wb");
            ultimaNota = -100000; // setamos assim pq a primeira comparacao da fita precisa acessar qualquer valor

            // desmarca todos os elementos do heap
            for(int i = 0; i < heap->tamanho; i++)
                heap->marcados[i] = 0;
        }

        // grava o menor registro na fita atual
        escreverAlunoBinario(out, &min);
        ultimaNota = min.nota;

        // le novo registro do arquivo de entrada
        if(lerAlunoBinario(in, &aluno)){
            if(aluno.nota >= ultimaNota)
                inserirHeap(heap, aluno, 0); // entra no bloco atual
            else
                inserirHeap(heap, aluno, 1); // vai para o proximo bloco
        }
    }

    fclose(out);
    fclose(in);
    destruirHeap(heap);

    // retorna a quantidade total de fitas geradas
    return totalFitas + 1;
}
