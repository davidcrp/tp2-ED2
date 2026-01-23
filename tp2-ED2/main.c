/* #include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "auxiliar.c"

void imprimirUso() {
    printf("Uso: ordena <método> <quantidade> <situação> [-P]\n");
    printf("  <método>: 1, 2 ou 3\n");
    printf("    1 - Intercalação balanceada 2f fitas (10 entrada + 10 saída)\n");
    printf("    2 - Intercalação balanceada f+1 fitas (19 entrada + 1 saída)\n");
    printf("    3 - Quicksort externo\n");
    printf("  <quantidade>: número de alunos a ordenar\n");
    printf("  <situação>: 1 (asc), 2 (desc) ou 3 (aleatório)\n");
    printf("  [-P]: opcional, imprime dados antes e depois da ordenação\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        imprimirUso();
        return 1;
    }
    
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int imprimir = (argc == 5 && strcmp(argv[4], "-P") == 0);
    
    // validacoes
    if (metodo < 1 || metodo > 3) {
        printf("Método inválido! Use 1, 2 ou 3.\n");
        return 1;
    }
    
    if (situacao < 1 || situacao > 3) {
        printf("Situação inválida! Use 1, 2 ou 3.\n");
        return 1;
    }
    
    printf("TRABALHO PRÁTICO 02 - ORDENAÇÃO EXTERNA\n");
    printf("Método: %d\n", metodo);
    printf("Quantidade: %d\n", quantidade);
    printf("Situação: %d\n", situacao);
    printf("Imprimir: %s\n", imprimir ? "Sim" : "Não");
    
    //prepara arquivo de entrada
    char arquivoEntrada[100];
    prepararArquivo("PROVAO.TXT", quantidade, situacao, arquivoEntrada);
    
    //executa o metodo
    switch(metodo) {
        case 1:
            metodo1(arquivoEntrada, quantidade, situacao, imprimir);
            break;
        case 2:
            metodo2(arquivoEntrada, quantidade, situacao, imprimir);
            break;
        case 3:
            metodo3(arquivoEntrada, quantidade, situacao, imprimir);
            break;
    }
    
    return 0;
} */