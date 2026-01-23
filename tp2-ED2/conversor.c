#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long inscricao;      // Colunas 1-8
    float nota;          // Colunas 10-14
    char estado[3];      // Colunas 16-17
    char cidade[51];     // Colunas 19-68
    char curso[31];      // Colunas 70-99
} Aluno;

// Função para ler um aluno do arquivo TXT (formato fixo)
int lerAlunoTexto(FILE *txt, Aluno *aluno) {
    char linha[256];
    
    if (fgets(linha, sizeof(linha), txt) == NULL) {
        return 0; // Fim do arquivo ou erro
    }
    
    char temp[100];
    
    // Inscrição (colunas 1-8, índice 0-7)
    strncpy(temp, linha + 0, 8);
    temp[8] = '\0';
    aluno->inscricao = atol(temp);
    
    // Nota (colunas 10-14, índice 9-13)
    strncpy(temp, linha + 9, 5);
    temp[5] = '\0';
    aluno->nota = atof(temp);
    
    // Estado (colunas 16-17, índice 15-16)
    strncpy(aluno->estado, linha + 15, 2);
    aluno->estado[2] = '\0';
    
    // Cidade (colunas 19-68, índice 18-67)
    strncpy(aluno->cidade, linha + 18, 50);
    aluno->cidade[50] = '\0';
    
    // Remover espaços em branco no final da cidade
    int len = strlen(aluno->cidade) - 1;
    while (len >= 0 && (aluno->cidade[len] == ' ' || aluno->cidade[len] == '\n' || aluno->cidade[len] == '\r')) {
        aluno->cidade[len] = '\0';
        len--;
    }
    
    // Curso (colunas 70-99, índice 69-98)
    if (strlen(linha) >= 99) {
        strncpy(aluno->curso, linha + 69, 30);
        aluno->curso[30] = '\0';
        
        // Remover espaços em branco no final do curso
        len = strlen(aluno->curso) - 1;
        while (len >= 0 && (aluno->curso[len] == ' ' || aluno->curso[len] == '\n' || aluno->curso[len] == '\r')) {
            aluno->curso[len] = '\0';
            len--;
        }
    } else {
        aluno->curso[0] = '\0';
    }
    
    return 1; // Sucesso
}

// Função principal de conversão
void converterTxtParaBin(char *arquivoTxt, char *arquivoBin, int quantidade) {
    FILE *txt = fopen(arquivoTxt, "r");
    if (!txt) {
        printf("ERRO: Não foi possível abrir o arquivo '%s'\n", arquivoTxt);
        printf("Certifique-se de que o arquivo PROVAO.TXT está no diretório atual.\n");
        exit(1);
    }
    
    FILE *bin = fopen(arquivoBin, "wb");
    if (!bin) {
        printf("ERRO: Não foi possível criar o arquivo '%s'\n", arquivoBin);
        fclose(txt);
        exit(1);
    }
    
    printf("\n========================================\n");
    printf("CONVERSOR PROVAO.TXT -> BINÁRIO\n");
    printf("========================================\n");
    printf("Arquivo entrada: %s\n", arquivoTxt);
    printf("Arquivo saída:   %s\n", arquivoBin);
    printf("Quantidade:      %d registros\n", quantidade);
    printf("========================================\n\n");
    
    int convertidos = 0;
    int erros = 0;
    Aluno aluno;
    
    while (convertidos < quantidade && lerAlunoTexto(txt, &aluno)) {
        // Escrever registro binário
        size_t escritos = fwrite(&aluno, sizeof(Aluno), 1, bin);
        
        if (escritos != 1) {
            printf("ERRO ao escrever registro %d\n", convertidos + 1);
            erros++;
        } else {
            convertidos++;
            
            // Mostrar progresso
            if (convertidos % 10000 == 0) {
                printf("Convertidos: %d registros...\r", convertidos);
                fflush(stdout);
            }
        }
    }
    
    printf("\n\n========================================\n");
    printf("CONVERSÃO CONCLUÍDA!\n");
    printf("========================================\n");
    printf("Total convertido: %d registros\n", convertidos);
    printf("Erros:            %d\n", erros);
    printf("Tamanho do arquivo: %ld bytes\n", convertidos * sizeof(Aluno));
    printf("Tamanho por registro: %ld bytes\n", sizeof(Aluno));
    printf("========================================\n\n");
    
    fclose(txt);
    fclose(bin);
}

// Função para verificar o arquivo binário (ler primeiros registros)
void verificarArquivoBinario(char *arquivoBin, int numRegistros) {
    FILE *bin = fopen(arquivoBin, "rb");
    if (!bin) {
        printf("ERRO: Não foi possível abrir '%s'\n", arquivoBin);
        return;
    }
    
    // Contar total de registros
    fseek(bin, 0, SEEK_END);
    long tamanho = ftell(bin);
    long totalRegistros = tamanho / sizeof(Aluno);
    fseek(bin, 0, SEEK_SET);
    
    printf("\n========================================\n");
    printf("VERIFICAÇÃO DO ARQUIVO BINÁRIO\n");
    printf("========================================\n");
    printf("Arquivo: %s\n", arquivoBin);
    printf("Total de registros: %ld\n", totalRegistros);
    printf("Primeiros %d registros:\n", numRegistros);
    printf("========================================\n\n");
    
    Aluno aluno;
    int lidos = 0;
    
    while (lidos < numRegistros && fread(&aluno, sizeof(Aluno), 1, bin) == 1) {
        printf("Registro %d:\n", lidos + 1);
        printf("  Inscrição: %ld\n", aluno.inscricao);
        printf("  Nota:      %.2f\n", aluno.nota);
        printf("  Estado:    %s\n", aluno.estado);
        printf("  Cidade:    %s\n", aluno.cidade);
        printf("  Curso:     %s\n\n", aluno.curso);
        lidos++;
    }
    
    fclose(bin);
}

// Programa principal
int main(int argc, char *argv[]) {
    char *arquivoEntrada = "PROVAO.TXT";
    char *arquivoSaida = "PROVAO.bin";
    int quantidade = 471705; // Total de registros no PROVAO.TXT
    
    // Processar argumentos da linha de comando
    if (argc >= 2) {
        quantidade = atoi(argv[1]);
        if (quantidade <= 0) {
            printf("ERRO: Quantidade inválida!\n");
            printf("Uso: %s [quantidade] [arquivo_saida]\n", argv[0]);
            return 1;
        }
    }
    
    if (argc >= 3) {
        arquivoSaida = argv[2];
    }
    
    // Executar conversão
    converterTxtParaBin(arquivoEntrada, arquivoSaida, quantidade);
    
    // Verificar arquivo criado (mostrar primeiros 5 registros)
    verificarArquivoBinario(arquivoSaida, 5);
    
    printf("✓ Conversão bem-sucedida!\n");
    printf("  Use o arquivo '%s' para os testes de ordenação.\n\n", arquivoSaida);
    
    return 0;
}