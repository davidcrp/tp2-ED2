/*
 * auxiliar.c - Funções Auxiliares
 * Trabalho Prático 02 - Ordenação Externa
 */

#include "structs.h"

// ============================================================================
// VARIÁVEL GLOBAL
// ============================================================================

Metricas metricas;

// ============================================================================
// FUNÇÕES DE MÉTRICAS
// ============================================================================

void inicializarMetricas() {
    metricas.leituras = 0;
    metricas.escritas = 0;
    metricas.comparacoes = 0;
    metricas.tempo = 0.0;
}

void imprimirMetricas() {
    printf("\n========== MÉTRICAS DE DESEMPENHO ==========\n");
    printf("Transferências de Leitura:  %ld\n", metricas.leituras);
    printf("Transferências de Escrita:  %ld\n", metricas.escritas);
    printf("Número de Comparações:      %ld\n", metricas.comparacoes);
    printf("Tempo de Execução:          %.6f segundos\n", metricas.tempo);
    printf("============================================\n\n");
}

// ============================================================================
// FUNÇÕES DE COMPARAÇÃO
// ============================================================================

int compararAlunos(Aluno *a, Aluno *b) {
    metricas.comparacoes++;
    if (a->nota < b->nota) return -1;
    if (a->nota > b->nota) return 1;
    return 0;
}

int compararAlunosQsort(const void *a, const void *b) {
    Aluno *a1 = (Aluno*)a;
    Aluno *a2 = (Aluno*)b;
    if (a1->nota < a2->nota) return -1;
    if (a1->nota > a2->nota) return 1;
    return 0;
}

int compararAlunosQsortDesc(const void *a, const void *b) {
    return -compararAlunosQsort(a, b);
}

// ============================================================================
// FUNÇÕES DE LEITURA/ESCRITA (TXT)
// ============================================================================

int lerAlunoTexto(FILE *txt, Aluno *aluno) {
    char linha[256];

    if (!fgets(linha, sizeof(linha), txt))
        return 0;

    sscanf(linha, "%8ld %5f %2s",
           &aluno->inscricao,
           &aluno->nota,
           aluno->estado);

    strncpy(aluno->cidade, linha + 18, 50);

    aluno->cidade[50] = '\0';

    strncpy(aluno->curso, linha + 69, 30);
    aluno->curso[30] = '\0';

    return 1;
}


// ============================================================================
// FUNÇÕES DE LEITURA/ESCRITA (BIN)
// ============================================================================

int lerAlunoBinario(FILE *bin, Aluno *aluno) {
    if (fread(aluno, sizeof(Aluno), 1, bin) == 1) {
        metricas.leituras++;
        return 1;
    }
    return 0;
}

void escreverAlunoBinario(FILE *bin, Aluno *aluno) {
    fwrite(aluno, sizeof(Aluno), 1, bin);
    metricas.escritas++;
}

int lerAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao) {
    fseek(arquivo, posicao * sizeof(Aluno), SEEK_SET);
    
    if (fread(aluno, sizeof(Aluno), 1, arquivo) == 1) {
        metricas.leituras++;
        return 1;
    }
    return 0;
}

void escreverAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao) {
    fseek(arquivo, posicao * sizeof(Aluno), SEEK_SET);
    fwrite(aluno, sizeof(Aluno), 1, arquivo);
    metricas.escritas++;
}

long obterNumeroAlunos(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    return tamanho / sizeof(Aluno);
}

void imprimirAluno(Aluno *aluno) {
    printf("%8ld | %5.2f | %2s | %-30s | %-25s\n",
           aluno->inscricao, aluno->nota, aluno->estado,
           aluno->cidade, aluno->curso);
}

// ============================================================================
// CONVERSÃO TXT -> BIN
// ============================================================================

void converterTxtParaBin(char *arquivoTxt, char *arquivoBin, int quantidade) {
    FILE *txt = fopen(arquivoTxt, "r");
    if (!txt) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivoTxt);
        exit(1);
    }
    
    FILE *bin = fopen(arquivoBin, "wb");
    if (!bin) {
        printf("ERRO: Não foi possível criar '%s'\n", arquivoBin);
        fclose(txt);
        exit(1);
    }
    
    printf("Convertendo %s -> %s (%d registros)...\n", 
           arquivoTxt, arquivoBin, quantidade);
    
    Aluno aluno;
    int convertidos = 0;
    
    while (convertidos < quantidade && lerAlunoTexto(txt, &aluno)) {
        fwrite(&aluno, sizeof(Aluno), 1, bin);
        convertidos++;
        
        if (convertidos % 10000 == 0) {
            printf("  Convertidos: %d registros\r", convertidos);
            fflush(stdout);
        }
    }
    
    printf("\n✓ Conversão concluída! %d registros convertidos.\n", convertidos);
    
    fclose(txt);
    fclose(bin);
}

// ============================================================================
// PREPARAÇÃO DE ARQUIVOS
// ============================================================================

void criarArquivoOrdenadoAsc(char *origem, char *destino, int n) {
    printf("Criando arquivo ordenado ascendentemente...\n");
    
    Aluno *alunos = (Aluno*)malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("ERRO: Memória insuficiente!\n");
        exit(1);
    }
    
    FILE *arq = fopen(origem, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", origem);
        free(alunos);
        exit(1);
    }
    
    fread(alunos, sizeof(Aluno), n, arq);
    fclose(arq);
    
    qsort(alunos, n, sizeof(Aluno), compararAlunosQsort);
    
    FILE *ord = fopen(destino, "wb");
    if (!ord) {
        printf("ERRO: Não foi possível criar '%s'\n", destino);
        free(alunos);
        exit(1);
    }
    
    fwrite(alunos, sizeof(Aluno), n, ord);
    fclose(ord);
    free(alunos);
    
    printf("Arquivo ordenado ascendente criado: %s\n", destino);
}

void criarArquivoOrdenadoDesc(char *origem, char *destino, int n) {
    printf("Criando arquivo ordenado descendentemente...\n");
    
    Aluno *alunos = (Aluno*)malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("ERRO: Memória insuficiente!\n");
        exit(1);
    }
    
    FILE *arq = fopen(origem, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", origem);
        free(alunos);
        exit(1);
    }
    
    fread(alunos, sizeof(Aluno), n, arq);
    fclose(arq);
    
    qsort(alunos, n, sizeof(Aluno), compararAlunosQsortDesc);
    
    FILE *ord = fopen(destino, "wb");
    if (!ord) {
        printf("ERRO: Não foi possível criar '%s'\n", destino);
        free(alunos);
        exit(1);
    }
    
    fwrite(alunos, sizeof(Aluno), n, ord);
    fclose(ord);
    free(alunos);
    
    printf("Arquivo ordenado descendente criado: %s\n", destino);
}

void prepararArquivo(char *arquivoOriginal, int quantidade, int situacao, 
                     char *arquivoSaida) {
    // Converter TXT para BIN se necessário
    char arquivoBinOriginal[100];
    sprintf(arquivoBinOriginal, "PROVAO_%d.bin", quantidade);
    
    FILE *teste = fopen(arquivoBinOriginal, "rb");
    if (!teste) {
        converterTxtParaBin(arquivoOriginal, arquivoBinOriginal, quantidade);
    } else {
        fclose(teste);
        printf("Usando arquivo binário existente: %s\n", arquivoBinOriginal);
    }
    
    switch(situacao) {
        case 1: // Ordenado ascendente
            sprintf(arquivoSaida, "PROVAO_%d_ASC.bin", quantidade);
            criarArquivoOrdenadoAsc(arquivoBinOriginal, arquivoSaida, quantidade);
            break;
        case 2: // Ordenado descendente
            sprintf(arquivoSaida, "PROVAO_%d_DESC.bin", quantidade);
            criarArquivoOrdenadoDesc(arquivoBinOriginal, arquivoSaida, quantidade);
            break;
        case 3: // Aleatório
            strcpy(arquivoSaida, arquivoBinOriginal);
            break;
        default:
            printf("Situação inválida!\n");
            exit(1);
    }
}

// ============================================================================
// FUNÇÕES DE VISUALIZAÇÃO
// ============================================================================

void imprimirPrimeirosRegistros(char *arquivo, int n) {
    FILE *arq = fopen(arquivo, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivo);
        return;
    }
    
    printf("\n========== PRIMEIROS %d REGISTROS ==========\n", n);
    printf("%-8s | %-5s | %-2s | %-30s | %-25s\n",
           "Inscr", "Nota", "UF", "Cidade", "Curso");
    printf("--------------------------------------------------------------------------------\n");
    
    Aluno aluno;
    int lidos = 0;
    while (lidos < n && fread(&aluno, sizeof(Aluno), 1, arq) == 1) {
        imprimirAluno(&aluno);
        lidos++;
    }
    
    fclose(arq);
    printf("============================================\n\n");
}

void verificarOrdenacao(char *arquivo, int quantidade) {
    FILE *arq = fopen(arquivo, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivo);
        return;
    }
    
    Aluno anterior, atual;
    int erros = 0;
    
    if (fread(&anterior, sizeof(Aluno), 1, arq) != 1) {
        fclose(arq);
        return;
    }
    
    for (int i = 1; i < quantidade; i++) {
        if (fread(&atual, sizeof(Aluno), 1, arq) != 1) break;
        
        if (atual.nota < anterior.nota) {
            erros++;
            if (erros <= 5) {
                printf("ERRO na posição %d: %.2f > %.2f\n", 
                       i, anterior.nota, atual.nota);
            }
        }
        anterior = atual;
    }
    
    fclose(arq);
    
    if (erros == 0) {
        printf("✓ Arquivo ORDENADO CORRETAMENTE!\n");
    } else {
        printf("✗ Arquivo com %d ERROS de ordenação!\n", erros);
    }
}

void imprimirPrimeirosAlunos(char *nomeArquivo, int quantidade) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        printf("ERRO: Não foi possível abrir '%s'\n", nomeArquivo);
        return;
    }
    
    printf("\n========== PRIMEIROS %d ALUNOS ==========\n", quantidade);
    printf("%-8s | %-5s | %-2s | %-50s | %-30s\n",
           "Inscr", "Nota", "UF", "Cidade", "Curso");
    printf("------------------------------------------------------------------------\n");
    
    Aluno aluno;
    int lidos = 0;
    
    while (lidos < quantidade && lerAlunoBinario(arquivo, &aluno)) {
        imprimirAluno(&aluno);
        lidos++;
    }
    
    fclose(arquivo);
    printf("=========================================\n\n");
}