#include "structs.h"

//estrutura metricas
Metricas metricas;

//inicializacao das metricas
void inicializarMetricas() {
    metricas.leituras = 0;
    metricas.escritas = 0;
    metricas.comparacoes = 0;
    metricas.tempo = 0.0;
}

//mostra o relatorio final de desempenho 
void imprimirMetricas() {
    printf("\n========== MÉTRICAS DE DESEMPENHO ==========\n");
    printf("Transferências de Leitura:  %ld\n", metricas.leituras);
    printf("Transferências de Escrita:  %ld\n", metricas.escritas);
    printf("Número de Comparações:      %ld\n", metricas.comparacoes);
    printf("Tempo de Execução:          %.6f segundos\n", metricas.tempo);
    printf("============================================\n\n");
}

//FUNCOES PARA COMPARACAO

//compara dois alunos usando o campo nota
//retorna -1 se a < b, 1 se a > b e 0 se forem iguais
int compararAlunos(Aluno *a, Aluno *b) {
    metricas.comparacoes++;
    if (a->nota < b->nota) return -1;
    if (a->nota > b->nota) return 1;
    return 0;
}

// funcao de comparacao usada pelo qsort em ordem crescente
// recebe ponteiros genericos e faz o cast para aluno
int compararAlunosQsort(const void *a, const void *b) {
    Aluno *a1 = (Aluno*)a;
    Aluno *a2 = (Aluno*)b;
    if (a1->nota < a2->nota) return -1;
    if (a1->nota > a2->nota) return 1;
    return 0;
}

// funcao de comparacao usada pelo qsort em ordem decrescente
// reaproveita a comparacao crescente invertendo o sinal
int compararAlunosQsortDesc(const void *a, const void *b) {
    return -compararAlunosQsort(a, b);
}


// le um registro de aluno a partir do arquivo texto provao
int lerAlunoTexto(FILE *txt, Aluno *aluno) {
    char linha[256];

    // tenta ler uma linha inteira do arquivo
    // se falhar, indica fim de arquivo
    if (!fgets(linha, sizeof(linha), txt))
        return 0;

    // extrai os campos iniciais da linha usando sscanf
    // inscricao, nota e estado tem tamanhos fixos
    sscanf(linha, "%8ld %5f %2s",
           &aluno->inscricao,
           &aluno->nota,
           aluno->estado);

    // copia o campo cidade a partir da posicao fixa da linha
    strncpy(aluno->cidade, linha + 18, 50);
    aluno->cidade[50] = '\0';

    // copia o campo curso a partir da posicao fixa da linha
    strncpy(aluno->curso, linha + 69, 30);
    aluno->curso[30] = '\0';

    return 1;
}

// FUNCOES LEITURAS E ESCRITAS BIN

// le um aluno do arquivo binario de forma sequencial
// atualiza a metrica de leituras
int lerAlunoBinario(FILE *bin, Aluno *aluno) {
    if (fread(aluno, sizeof(Aluno), 1, bin) == 1) {
        metricas.leituras++;
        return 1;
    }
    return 0;
}

// escreve um aluno no arquivo binario de forma sequencial
// atualiza a metrica de escritas
void escreverAlunoBinario(FILE *bin, Aluno *aluno) {
    fwrite(aluno, sizeof(Aluno), 1, bin);
    metricas.escritas++;
}

// le um aluno em uma posicao especifica do arquivo binario
// usa fseek para acesso direto (acesso aleatorio)
int lerAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao) {
    fseek(arquivo, posicao * sizeof(Aluno), SEEK_SET);
    
    if (fread(aluno, sizeof(Aluno), 1, arquivo) == 1) {
        metricas.leituras++;
        return 1;
    }
    return 0;
}

// escreve um aluno em uma posicao especifica do arquivo binario
// tambem utiliza acesso direto com fseek
void escreverAlunoNaPosicao(FILE *arquivo, Aluno *aluno, long posicao) {
    fseek(arquivo, posicao * sizeof(Aluno), SEEK_SET);
    fwrite(aluno, sizeof(Aluno), 1, arquivo);
    metricas.escritas++;
}

// calcula quantos alunos existem no arquivo binario
// faz isso dividindo o tamanho total pelo tamanho de um registro
long obterNumeroAlunos(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    return tamanho / sizeof(Aluno);
}

// imprime um aluno formatado na tela
// usada apenas para exibicao e depuracao
void imprimirAluno(Aluno *aluno) {
    printf("%8ld | %5.2f | %2s | %-30s | %-25s\n",
           aluno->inscricao, aluno->nota, aluno->estado,
           aluno->cidade, aluno->curso);
}

// CONVERSAO TXT PARA BIN

// converte um arquivo texto em binario
// limita a conversao ao numero de registros informado
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
    
    printf("Convertendo %s -> %s (%d registros)...\n", arquivoTxt, arquivoBin, quantidade);
    
    Aluno aluno;
    int convertidos = 0;
    
    // le alunos do txt e grava no bin ate atingir a quantidade desejada
    while (convertidos < quantidade && lerAlunoTexto(txt, &aluno)) {
        fwrite(&aluno, sizeof(Aluno), 1, bin);
        convertidos++;
        
        // imprime progresso para arquivos grandes
        if (convertidos % 10000 == 0) {
            printf("  Convertidos: %d registros\r", convertidos);
            fflush(stdout);
        }
    }
    
    printf("\n Conversao concluida! %d registros convertidos.\n", convertidos);
    
    fclose(txt);
    fclose(bin);
}

// CRIACAO ORDENADA DOS ARQUIVOS ASC E DESC

// cria um arquivo binario ordenado em ordem crescente de nota
void criarArquivoOrdenadoAsc(char *origem, char *destino, int n) {
    printf("Criando arquivo ordenado ascendentemente...\n");
    
    // aloca memoria para carregar todos os alunos
    Aluno *alunos = (Aluno*)malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("ERRO: Memória insuficiente!\n");
        exit(1);
    }
    
    // abre o arquivo de origem
    FILE *arq = fopen(origem, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", origem);
        free(alunos);
        exit(1);
    }
    
    // le todos os registros do arquivo para memoria
    fread(alunos, sizeof(Aluno), n, arq);
    fclose(arq);
    
    // ordena os alunos pela nota usando qsort
    qsort(alunos, n, sizeof(Aluno), compararAlunosQsort);
    
    // cria o arquivo de destino ordenado
    FILE *ord = fopen(destino, "wb");
    if (!ord) {
        printf("ERRO: Não foi possível criar '%s'\n", destino);
        free(alunos);
        exit(1);
    }
    
    // grava os alunos ordenados no arquivo
    fwrite(alunos, sizeof(Aluno), n, ord);
    fclose(ord);
    free(alunos);
    
    printf("Arquivo ordenado ascendente criado: %s\n", destino);
}

// cria um arquivo binario ordenado em ordem decrescente de nota
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
    
    // ordena os alunos em ordem decrescente
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

// prepara o arquivo final conforme a situacao escolhida
// situacao: 1 ascendente, 2 descendente, 3 aleatorio
void prepararArquivo(char *arquivoOriginal, int quantidade, int situacao, char *arquivoSaida) {
    // sempre converte o txt original para binario
    char arquivoBinOriginal[100];
    sprintf(arquivoBinOriginal, "PROVAO_%d.bin", quantidade);

    converterTxtParaBin(arquivoOriginal, arquivoBinOriginal, quantidade);

    switch(situacao) {
        case 1:
            sprintf(arquivoSaida, "PROVAO_%d_ASC.bin", quantidade);
            criarArquivoOrdenadoAsc(arquivoBinOriginal, arquivoSaida, quantidade);
            break;

        case 2:
            sprintf(arquivoSaida, "PROVAO_%d_DESC.bin", quantidade);
            criarArquivoOrdenadoDesc(arquivoBinOriginal, arquivoSaida, quantidade);
            break;

        case 3:
            strcpy(arquivoSaida, arquivoBinOriginal);
            break;

        default:
            printf("Situação inválida!\n");
            exit(1);
    }
}

// FUNCAO DE PRINT

// imprime os primeiros registros de um arquivo binario
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

// verifica se o arquivo esta ordenado corretamente por nota
void verificarOrdenacao(char *arquivo, int quantidade) {
    FILE *arq = fopen(arquivo, "rb");
    if (!arq) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivo);
        return;
    }
    
    Aluno anterior, atual;
    int erros = 0;
    
    // le o primeiro aluno como referencia inicial
    if (fread(&anterior, sizeof(Aluno), 1, arq) != 1) {
        fclose(arq);
        return;
    }
    
    // percorre o arquivo comparando registros consecutivos
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
        printf(" Arquivo ORDENADO CORRETAMENTE!\n");
    } else {
        printf("Arquivo com %d ERROS de ordenação!\n", erros);
    }
}

// imprime os primeiros alunos usando leitura binaria com metricas
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
    
    // le alunos enquanto houver registros e limite nao for atingido
    while (lidos < quantidade && lerAlunoBinario(arquivo, &aluno)) {
        imprimirAluno(&aluno);
        lidos++;
    }
    
    fclose(arquivo);
    printf("=========================================\n\n");
}

// exporta um arquivo binario para texto
void exportarParaTxt(char *nomeBinario, char *nomeTexto) {
    FILE *bin = fopen(nomeBinario, "rb");
    FILE *txt = fopen(nomeTexto, "w");
    
    if (!bin || !txt) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    Aluno a;
    // le cada registro do binario e grava no txt
    while (fread(&a, sizeof(Aluno), 1, bin)) {
        // mantem o formato original
        fprintf(txt, "%08ld %05.1f %s %-50s %-30s\n", 
                a.inscricao, a.nota, a.estado, a.cidade, a.curso);
    }

    fclose(bin);
    fclose(txt);
    printf("Arquivo texto gerado: %s\n", nomeTexto);
}
