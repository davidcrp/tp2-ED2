/*
 * Trabalho Prático 02 - Ordenação Externa
 * BCC203 - Estrutura de Dados II a
 * UFOP - 2025/2
 */


 
#include "structs.h"

void imprimirUso() {
    printf("\n========================================\n");
    printf("TRABALHO PRÁTICO 02 - ORDENAÇÃO EXTERNA\n");
    printf("========================================\n\n");
    printf("Uso: ordena <método> <quantidade> <situação> [-P]\n\n");
    printf("Parâmetros:\n");
    printf("  <método>: 1, 2 ou 3\n");
    printf("    1 - Intercalação balanceada 2f fitas (10 entrada + 10 saída)\n");
    printf("    2 - Intercalação balanceada f+1 fitas (19 entrada + 1 saída)\n");
    printf("    3 - Quicksort externo\n\n");
    printf("  <quantidade>: número de alunos a ordenar\n");
    printf("    Exemplos: 100, 200, 2000, 20000, 200000, 471705\n\n");
    printf("  <situação>: ordem inicial do arquivo\n");
    printf("    1 - Ordenado ascendentemente (melhor caso)\n");
    printf("    2 - Ordenado descendentemente (pior caso)\n");
    printf("    3 - Aleatório (caso médio)\n\n");
    printf("  [-P]: (opcional) imprime dados antes e depois da ordenação\n\n");
    printf("Exemplos de uso:\n");
    printf("  ./ordena 3 100 3        # Quicksort em 100 alunos aleatórios\n");
    printf("  ./ordena 1 2000 1 -P    # Método 1 em 2000 alunos ordenados, com impressão\n");
    printf("  ./ordena 2 20000 2      # Método 2 em 20000 alunos em ordem decrescente\n");
    printf("========================================\n\n");
}

int main(int argc, char *argv[]) {
    // Verificar número de argumentos
    if (argc < 4) {
        imprimirUso();
        return 1;
    }
    
    // Ler argumentos
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int imprimir = (argc == 5 && strcmp(argv[4], "[-P]") == 0);
    
    // Validações
    if (metodo < 1 || metodo > 3) {
        printf("ERRO: Método inválido! Use 1, 2 ou 3.\n");
        imprimirUso();
        return 1;
    }
    
    if (quantidade <= 0) {
        printf("ERRO: Quantidade inválida! Deve ser um número positivo.\n");
        imprimirUso();
        return 1;
    }
    
    if (situacao < 1 || situacao > 3) {
        printf("ERRO: Situação inválida! Use 1, 2 ou 3.\n");
        imprimirUso();
        return 1;
    }
    
    // Cabeçalho
    printf("\n========================================\n");
    printf("TRABALHO PRÁTICO 02 - ORDENAÇÃO EXTERNA\n");
    printf("========================================\n\n");
    
    // Mostrar configuração
    printf("Configuração:\n");
    printf("  Método:     %d - ", metodo);
    switch(metodo) {
        case 1: printf("Intercalação Balanceada 2f Fitas\n"); break;
        case 2: printf("Intercalação Balanceada f+1 Fitas\n"); break;
        case 3: printf("Quicksort Externo\n"); break;
    }
    
    printf("  Quantidade: %d alunos\n", quantidade);
    printf("  Situação:   %d - ", situacao);
    switch(situacao) {
        case 1: printf("Ordenado Ascendentemente\n"); break;
        case 2: printf("Ordenado Descendentemente\n"); break;
        case 3: printf("Aleatório\n"); break;
    }
    
    printf("  Impressão:  %s\n", imprimir ? "Sim (-P)" : "Não");
    printf("\n========================================\n\n");
    
    // Preparar arquivo de entrada
    char arquivoEntrada[100];
    printf("Preparando arquivo de entrada...\n");
    prepararArquivo("PROVAO.TXT", quantidade, situacao, arquivoEntrada);
    printf("Arquivo preparado: %s\n", arquivoEntrada);
    
    // Executar método selecionado
    switch(metodo) {
        case 1:
            metodo1(arquivoEntrada, quantidade, situacao, imprimir);
            break;
        case 2:
            metodo2(arquivoEntrada, quantidade, situacao, imprimir);
            break;
        case 3:
            metodo3(arquivoEntrada, quantidade, imprimir);
            break;
    }
    
    printf("\n========================================\n");
    printf("EXECUÇÃO CONCLUÍDA!\n");
    printf("========================================\n\n");
    
    
    char nomeSaidaTxt[100];
    sprintf(nomeSaidaTxt, "RESULTADO_FINAL.TXT");
    exportarParaTxt(arquivoEntrada, nomeSaidaTxt); // Gera o arquivo TXT aqui
    printf("Arquivo texto para conferencia gerado: %s\n", nomeSaidaTxt);

    return 0;
}