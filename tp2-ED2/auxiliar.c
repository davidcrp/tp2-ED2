#include<stdio.h>
#include<stdlib.h>

#define MAX_PALAVRA 100

typedef struct{
    long inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
}Aluno;

typedef struct{
    long leituras;
    long escritas;
    long comparacoes;
    double tempo;
}Metricas;

Metricas metricas;

void inicializarMetricas(){
    metricas.leituras = 0;
    metricas.escritas = 0;
    metricas.comparacoes = 0;
    metricas.tempo = 0;
}

void imprimirMetricas(){
    printf("METRICAS DE DESEMPRENHO:\n");
    printf("Transferencia de leitura: %ld\n", metricas.leituras);
    printf("Transferencia de escrita: %lf\n", metricas.escritas);
}