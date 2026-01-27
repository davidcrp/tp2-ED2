#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

#define MAX_FITAS 20

// estrutura do heap minimo com marcacao
// (Estrutura movida para structs.h, funcoes aqui)

// cria o heap com capacidade maxima definida
Heap* criarHeap(int capacidade){
    Heap *h = (Heap*) malloc(sizeof(Heap));
    h->alunos = (Aluno*) malloc(capacidade * sizeof(Aluno));
    h->marcados = (int*) malloc(capacidade * sizeof(int));
    h->origem = (int*) malloc(capacidade * sizeof(int)); // Adicionado
    h->capacidade = capacidade;
    h->tamanho = 0;
    return h;
}

// libera toda a memoria alocada pelo heap
void destruirHeap(Heap *h){
    free(h->alunos);
    free(h->marcados);
    free(h->origem); // Adicionado
    free(h);
}

// troca dois elementos de posicao dentro do heap a
void trocar(Heap *h, int i, int j){
    Aluno temp = h->alunos[i];
    h->alunos[i] = h->alunos[j];
    h->alunos[j] = temp;

    int tempMarcado = h->marcados[i];
    h->marcados[i] = h->marcados[j];
    h->marcados[j] = tempMarcado;

    // Troca tambem a origem
    int tempOrigem = h->origem[i];
    h->origem[i] = h->origem[j];
    h->origem[j] = tempOrigem;
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
void inserirHeap(Heap *h, Aluno aluno, int marcado, int origem){
    if(h->tamanho < h->capacidade){
        h->alunos[h->tamanho] = aluno;
        h->marcados[h->tamanho] = marcado;
        h->origem[h->tamanho] = origem; // Adicionado
        heapParaCima(h, h->tamanho);
        h->tamanho++;
    }
}

// remove o menor elemento do heap
Aluno removerMinHeap(Heap *h, int *marcado, int *origem){
    Aluno min = h->alunos[0];
    if (marcado) *marcado = h->marcados[0];
    if (origem) *origem = h->origem[0]; // Adicionado

    h->tamanho--;

    if(h->tamanho > 0){
        h->alunos[0] = h->alunos[h->tamanho];
        h->marcados[0] = h->marcados[h->tamanho];
        h->origem[0] = h->origem[h->tamanho]; // Adicionado
        heapParaBaixo(h, 0);
    }

    return min;
}

// substitui o menor elemento do heap por um novo registro
void substituirMinHeap(Heap *h, Aluno novoAluno, int marcado, int origem){
    h->alunos[0] = novoAluno;
    h->marcados[0] = marcado;
    h->origem[0] = origem;
    heapParaBaixo(h, 0);
}

// implementacao da selecao por substituicao
int selecaoPorSubstituicao(char *arquivoEntrada, int M){
    FILE *in = fopen(arquivoEntrada, "rb");
    if (!in) {
        printf("erro ao abrir arquivo de entrada\n");
        exit(1);
    }

    Heap *heap = criarHeap(M);
    Aluno aluno;

    // carrega os m primeiros registros na memoria
    for(int i = 0; i < M; i++){
        if(lerAlunoBinario(in, &aluno))
            inserirHeap(heap, aluno, 0, -1);
        else
            break;
    }

    if(heap->tamanho == 0){
        fclose(in);
        destruirHeap(heap);
        return 0;
    }

    int totalBlocos = 0;
    // cria o primeiro arquivo de bloco
    char nomeBloco[MAX_NOME];
    sprintf(nomeBloco, "blocos/passo0_bloco%d.bin", totalBlocos);

    FILE *out = fopen(nomeBloco, "wb");

    // armazena a ultima nota gravada na fita atual
    float ultimaNota = -1.0;

    // loop principal da selecao por substituicao
    while(heap->tamanho > 0){

        int marcado;
        Aluno min = removerMinHeap(heap, &marcado, NULL);

        // se o menor estiver marcado, significa que o bloco acabou
        if(marcado){
            fclose(out);
            totalBlocos++;
            sprintf(nomeBloco, "blocos/passo0_bloco%d.bin", totalBlocos);

            out = fopen(nomeBloco, "wb");
            
            // Reinsere o item atual (agora desmarcado para o novo bloco)
            inserirHeap(heap, min, 0, -1);
            
            // desmarca todos os elementos do heap
            for(int i = 0; i < heap->tamanho; i++)
                heap->marcados[i] = 0;
            
            // Reconstrói o heap (necessário pois mudamos as prioridades ao desmarcar)
            for(int i = heap->tamanho/2; i >= 0; i--)
                heapParaBaixo(heap, i);
                
            continue; // Recomeça o loop com o novo bloco
        }

        // grava o menor registro na fita atual
        escreverAlunoBinario(out, &min);
        ultimaNota = min.nota;

        // le novo registro do arquivo de entrada
        if(lerAlunoBinario(in, &aluno)){
            if(aluno.nota >= ultimaNota)
                inserirHeap(heap, aluno, 0, -1); // entra no bloco atual
            else
                inserirHeap(heap, aluno, 1, -1); // vai para o proximo bloco
        }
    }

    fclose(out);
    fclose(in);
    destruirHeap(heap);

    // retorna a quantidade total de blocos gerados
    return totalBlocos + 1;
}