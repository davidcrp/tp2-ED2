/*
 * Trabalho Prático 02 - Ordenação Externa
 * BCC203 - Estrutura de Dados II a
 * UFOP - 2025/2
 */

// inclui as estruturas, tipos e prototipos usados em todo o trabalho
#include "structs.h"

// imprime na tela as instrucoes de uso do programa
// chamada quando o usuario fornece parametros invalidos ou insuficientes
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

// funcao principal do programa
// responsavel por validar parametros, preparar arquivos e chamar o metodo escolhido
int main(int argc, char *argv[]) {

    // verifica se o numero minimo de argumentos foi fornecido
    if (argc < 4) {
        imprimirUso();
        return 1;
    }
    
    // leitura dos argumentos da linha de comando
    // metodo define qual algoritmo sera usado
    // quantidade define o numero de alunos
    // situacao define a ordem inicial do arquivo
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);

    // verifica se a opcao -P foi passada
    // nesse caso, o programa imprime dados antes e depois da ordenacao
    int imprimir = (argc == 5 && strcmp(argv[4], "[-P]") == 0);
    
    // validacao do metodo escolhido
    if (metodo < 1 || metodo > 3) {
        printf("ERRO: Método inválido! Use 1, 2 ou 3.\n");
        imprimirUso();
        return 1;
    }
    
    // validacao da quantidade de alunos
    if (quantidade <= 0) {
        printf("ERRO: Quantidade inválida! Deve ser um número positivo.\n");
        imprimirUso();
        return 1;
    }
    
    // validacao da situacao inicial do arquivo
    if (situacao < 1 || situacao > 3) {
        printf("ERRO: Situação inválida! Use 1, 2 ou 3.\n");
        imprimirUso();
        return 1;
    }
    
    // impressao do cabecalho do trabalho
    printf("\n========================================\n");
    printf("TRABALHO PRÁTICO 02 - ORDENAÇÃO EXTERNA\n");
    printf("========================================\n\n");
    
    // exibicao da configuracao escolhida pelo usuario
    printf("Configuração:\n");
    printf("  Método:     %d - ", metodo);

    // identifica o metodo selecionado
    switch(metodo) {
        case 1: printf("Intercalação Balanceada 2f Fitas\n"); break;
        case 2: printf("Intercalação Balanceada f+1 Fitas\n"); break;
        case 3: printf("Quicksort Externo\n"); break;
    }
    
    // mostra a quantidade de alunos
    printf("  Quantidade: %d alunos\n", quantidade);

    // identifica a situacao inicial do arquivo
    printf("  Situação:   %d - ", situacao);
    switch(situacao) {
        case 1: printf("Ordenado Ascendentemente\n"); break;
        case 2: printf("Ordenado Descendentemente\n"); break;
        case 3: printf("Aleatório\n"); break;
    }
    
    // indica se a impressao dos registros foi solicitada
    printf("  Impressão:  %s\n", imprimir ? "Sim (-P)" : "Não");
    printf("\n========================================\n\n");
    
    // prepara o arquivo de entrada a partir do PROVAO.TXT
    // converte para binario e ajusta conforme a situacao escolhida
    char arquivoEntrada[100];
    printf("Preparando arquivo de entrada...\n");
    prepararArquivo("PROVAO.TXT", quantidade, situacao, arquivoEntrada);
    printf("Arquivo preparado: %s\n", arquivoEntrada);
    
    // executa o metodo de ordenacao selecionado
    switch(metodo) {
        case 1:
            // intercalação balanceada com 2f fitas
            metodo1(arquivoEntrada, quantidade, situacao, imprimir);
            break;

        case 2:
            // intercalação balanceada com f+1 fitas
            metodo2(arquivoEntrada, quantidade, situacao, imprimir);
            break;

        case 3:
            // quicksort externo
            metodo3(arquivoEntrada, quantidade, imprimir);
            break;
    }
    
    // mensagem final indicando o termino da execucao
    printf("\n========================================\n");
    printf("EXECUÇÃO CONCLUÍDA!\n");
    printf("========================================\n\n");
    
    // gera um arquivo texto final para conferencia dos resultados
    // usa o arquivo binario final produzido pelo metodo
    char nomeSaidaTxt[100];
    sprintf(nomeSaidaTxt, "RESULTADO_FINAL.TXT");
    exportarParaTxt(arquivoEntrada, nomeSaidaTxt);
    printf("Arquivo texto para conferencia gerado: %s\n", nomeSaidaTxt);

    return 0;
}
