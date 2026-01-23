/*
 * structs.h - Definições de Estruturas e Constantes
 * Trabalho Prático 02 - Ordenação Externa
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// CONSTANTES
// ============================================================================

#define TAMAREA 10  // Tamanho da área de memória interna para Quicksort

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

// Estrutura de um aluno
typedef struct {
    long inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Aluno;

// Estrutura para métricas de desempenho
typedef struct {
    long leituras;
    long escritas;
    long comparacoes;
    double tempo;
} Metricas;

// Estrutura para registro usado no Quicksort Externo
typedef struct {
    float Chave;
    Aluno Aluno;
} TipoRegistro;

// Estrutura para área de memória do Quicksort Externo
typedef struct {
    TipoRegistro Registros[TAMAREA];
    int NumOcupadas;
} TipoArea;

// ============================================================================
// VARIÁVEL GLOBAL
// ============================================================================

extern Metricas metricas;

// ============================================================================
// DECLARAÇÃO DE FUNÇÕES (auxiliar.c)
// ============================================================================

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

// ============================================================================
// DECLARAÇÃO DE FUNÇÕES (quicksort.c)
// ============================================================================

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

// ============================================================================
// DECLARAÇÃO DE FUNÇÕES (métodos.c)
// ============================================================================

void metodo1(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo2(char *arquivo, int quantidade, int situacao, int imprimir);
void metodo3(char *arquivo, int quantidade, int situacao, int imprimir);

#endif // STRUCTS_H