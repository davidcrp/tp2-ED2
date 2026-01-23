#include<stdio.h>
#include<stdlib.h>
#include "auxiliar.c"

typedef struct{
    Aluno *alunos;
    int tamanho;
    int capacidade;
    int *marcados; //vetor para marcar elementos  do proximo bloco
} Heap;

Heap* criarHeap(int capacidade){
    Heap *h = (Heap*)malloc(sizeof(Heap));
    h->alunos = (Aluno*)malloc(capacdade * sizeof(Aluno));
    h->marcados = (int*)malloc(capacidade, sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    return h;
}

void destruirHeap(Heap *h){
    free(h->alunos);
    free(h->marcados);
    free(h);
}

void trocar(Heap *h, int i, int j){
    Aluno temp = h->alunos[i];
    h->alunos[i] = h->alunos[j];
    h->alunos[j] = temp;

    int tempMarcado = h->marcados[i];
    h->marcados[i] = h->marcados[j];
    h->marcados[j] = tempMarcado;
}

void heapParaBaixo(Heap *h, int i){
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if(esq < h->tamanho){
        if(h->marcados[esq] < h->marcados[menor] || (h->marcados[esq] == marcados[menor] && compararAlunos(&h->alunos[esq], &h->alunos[menor]) < 0)){
            menor = esq;
        }
    }

    if(dir < h->tamanho){
        if(h->marcados[dir] < h->marcados[menor] || (h->marcados[dir] == h->marcados[menor] && compararAlunos(&h->alunos[dir], &h->alunos[menor]) < 0)){
            menor = dir;
        }
    }

    if(menor != i){
        trocar(h, i, menor);
        heapParaBaixo(h, menor);
    }
}

void heapParaCima(Heap *h, int i){
    while(i>0){
        int pai = (i - 1) / 2;
        if(h->marcados[i] < h->marcados[pai] || (h->marcados[i] == h->marcados[pai] && compararAlunos(&h->alunos[i], &h->alunos[pai]) < 0)){
            trocar(h, i, pai);
            i = pai;
        } else{
            break;
        }
    }
}

void inserirHeap(Heap *h, Aluno aluno, int marcado){
    if(h->tamanho < h->capacidade){
        h->aluno[h->tamanho] = aluno;
        h->marcados[h->tamanho] = marcado;
        heapParaCima(h,h->tamanho);
        h->tamanho++;
    }
}

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

void substituirMinHeap(Heap *h, Aluno novoAluno, int marcado){
    h->aluno[0] = novoAluno;
    h->marcados[0] = marcado;
    heapParaBaixo(h, 0);
}