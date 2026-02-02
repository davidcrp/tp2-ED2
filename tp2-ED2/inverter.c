#include <stdio.h>
#include <stdlib.h>
// a

/*void inverterArquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    
    if (tamanho == 0) {
        printf("Arquivo vazio\n");
        fclose(arquivo);
        return;
    }

    unsigned char *buffer = (unsigned char *)malloc(tamanho);
    if (buffer == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(arquivo);
        return;
    }

    fseek(arquivo, 0, SEEK_SET);
    fread(buffer, 1, tamanho, arquivo);

    for (long i = 0; i < tamanho / 2; i++) {
        unsigned char temp = buffer[i];
        buffer[i] = buffer[tamanho - 1 - i];
        buffer[tamanho - 1 - i] = temp;
    }
    
    fseek(arquivo, 0, SEEK_SET);
    fwrite(buffer, 1, tamanho, arquivo);

    free(buffer);
    fclose(arquivo);
    
    printf("Arquivo %s invertido com sucesso!\n", nomeArquivo);
}

int main() {
    inverterArquivo("PROVAO_471705.bin");
    return 0;
}
*/