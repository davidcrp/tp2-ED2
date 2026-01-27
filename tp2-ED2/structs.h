#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h> 


#define TAMAREA 10  // tamanho area para quicksort
#define MAX_NOME 100 // tamanho maximo para nomes de arquivos (ADICIONADO)

void exportarParaTxt(char *nomeBinario, char *nomeTexto);

typedef struct {
    long inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Aluno;


typedef struct {
    long leituras;
    long escritas;
    long comparacoes;
    double tempo;
} Metricas;

// registro quick sort  externo
typedef struct {
    float Chave;
    Aluno Aluno;
} TipoRegistro;

// area de memoria quicksort externo
typedef struct {
    TipoRegistro Registros[TAMAREA];
    int NumOcupadas;
} TipoArea;

// estrutura do heap minimo com marcacao
typedef struct{
    Aluno *alunos;   // vetor de registros em memoria
    int tamanho;     // quantidade atual de elementos no heap
    int capacidade;  // capacidade maxima do heap (memoria)
    int *marcados;   // vetor para marcar elementos do proximo bloco
    int *origem;     // vetor para saber de qual arquivo/fita veio o registro (ADICIONADO)
} Heap;

extern Metricas metricas;

// declaracao de funcoes auxiliares que estao implementadas no auxiliar.c

void inicializarMetricas();
void imprimirMetricas();
int compararAlunos(Aluno *a, Aluno *b);
int compararAlunosQsort(const void *a, const void *b);
int compararAlunosQsortDesc(const void *a, const void *b);

int lerAlunoTexto(FILE *txt, Aluno *aluno);
int lerAlunoBinario(FILE *bin, Aluno *aluno);
void escreverAlunoBinario(FILE *bin, Aluno *aluno);
int lerAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao);
void escreverAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao);
long obterNumeroAlunos(FILE *arquivo);
void imprimirAluno(Aluno *aluno);
void imprimirPrimeirosAlunos(char *nomeArquivo, int quantidade);

void converterTxtParaBin(char *arquivoTxt, char *arquivoBin, int quantidade);
void criarArquivoOrdenadoAsc(char *origem, char *destino, int n);
void criarArquivoOrdenadoDesc(char *origem, char *destino, int n);
void prepararArquivo(char *arquivoOriginal, int quantidade, int situacao, char *arquivoSaida);

void imprimirPrimeirosRegistros(char *arquivo, int n);
void verificarOrdenacao(char *arquivo, int quantidade);

// declaracao das funcoes do quicksort externo

void FAVazia(TipoArea *Area);
void InserirArea(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R);
void RetiraUltimo(TipoArea *Area, TipoRegistro *R);
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);

void LeSup(FILE *Arq, TipoRegistro *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE *Arq, TipoRegistro *UltLido, int *Li, short *OndeLer);
void EscreveMax(FILE *Arq, TipoRegistro R, int *Es);
void EscreveMin(FILE *Arq, TipoRegistro R, int *Ei);

void Particao(FILE *Arq, TipoArea *Area, int Esq, int Dir, int *i, int *j);
void QuicksortExterno(FILE *Arq, int Esq, int Dir);

// declaracao das funcoes dos metodos, que a main chama

void metodo1(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo2(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo3(char *arquivo, int quantidade, int imprimir);


// selecao por substituicao 

// funcoes do heap
Heap* criarHeap(int capacidade);
void destruirHeap(Heap *h);
void trocar(Heap *h, int i, int j);
void heapParaBaixo(Heap *h, int i);
void heapParaCima(Heap *h, int i);
// Atualizado: recebe 'origem'
void inserirHeap(Heap *h, Aluno aluno, int marcado, int origem);
// Atualizado: retorna 'origem' por referencia
Aluno removerMinHeap(Heap *h, int *marcado, int *origem);
void substituirMinHeap(Heap *h, Aluno novoAluno, int marcado, int origem);

// selecao por substituicao
int selecaoPorSubstituicao(char *arquivoEntrada, int tamMemoria);

#endif