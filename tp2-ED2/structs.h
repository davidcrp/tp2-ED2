#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h> 

#define TAMAREA 10   // tamanho area para quicksort
#define MAX_NOME 100 // tamanho maximo para nomes de arquivos

void exportarParaTxt(char *nomeBinario, char *nomeTexto);

// =====================
// ESTRUTURAS PRINCIPAIS
// =====================

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

// Registro quicksort externo
typedef struct {
    float Chave;
    Aluno Aluno;
} TipoRegistro;

// Area de memoria quicksort externo
typedef struct {
    TipoRegistro Registros[TAMAREA];
    int NumOcupadas;
} TipoArea;

// Heap minimo com marcacao e origem
typedef struct{
    Aluno *alunos;
    int tamanho;
    int capacidade;
    int *marcados;
    int *origem;
} Heap;

extern Metricas metricas;

// =================
// FUNCOES AUXILIARES
// =================

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

// =====================
// QUICKSORT EXTERNO
// =====================

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

// =================
// METODOS DE ORDENACAO
// =================

void metodo1(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo2(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo3(char *arquivo, int quantidade, int imprimir);

// ===========================
// HEAP + SELECAO POR SUBSTITUICAO
// ===========================

Heap* criarHeap(int capacidade);
void destruirHeap(Heap *h);
void trocar(Heap *h, int i, int j);
void heapParaBaixo(Heap *h, int i);
void heapParaCima(Heap *h, int i);

void inserirHeap(Heap *h, Aluno aluno, int marcado, int origem);
Aluno removerMinHeap(Heap *h, int *marcado, int *origem);
void substituirMinHeap(Heap *h, Aluno novoAluno, int marcado, int origem);

int selecaoPorSubstituicao(char *arquivoEntrada, int M);


// intercalacao 

// funcoes de intercalacao balanceada
/* 2f - distribuicao inicial dos blocos nas fitas */
void distribuirBlocosEmFitas(int numBlocos);

/* 2f - realiza uma passada de intercalacao balanceada */
int realizarPassada2F(int numBlocosIn, int grau, int passoAtual);

/* f+1 - distribuicao inicial dos blocos nas fitas */
void distribuirBlocosEmFitasF1(int numBlocos);

/* f+1 - realiza uma passada de intercalacao balanceada */
int realizarPassadaF1(int numBlocosIn, int passoAtual);


#endif
