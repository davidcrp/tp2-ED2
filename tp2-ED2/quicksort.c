#include "structs.h"

// heap ordenado usado como area de memoria no quicksort externo
// a area mantem os registros ordenados pela chave

// inicializa a area como vazia
void FAVazia(TipoArea *Area) {
    Area->NumOcupadas = 0;
}

// insere um registro na area mantendo a ordenacao crescente pela chave
// desloca registros maiores para a direita ate achar a posicao correta
void InserirArea(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    int i = Area->NumOcupadas; // aponta para a proxima posicao livre
    
    // desloca elementos maiores que a chave
    while (i > 0 && Area->Registros[i-1].Chave > R->Chave) {
        metricas.comparacoes++;
        Area->Registros[i] = Area->Registros[i-1];
        i--;
    }

    // contabiliza a comparacao final que encerra o while
    if (i > 0) metricas.comparacoes++;
    
    // insere o registro na posicao correta
    Area->Registros[i] = *R;

    // atualiza quantidade de registros na area
    Area->NumOcupadas++;
    *NRArea = Area->NumOcupadas;
}

// remove o primeiro registro da area
// corresponde ao registro de menor chave
void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R) {

    // verifica se ha registros na area
    if (Area->NumOcupadas > 0) {

        // copia o menor registro
        *R = Area->Registros[0];

        // desloca os demais registros uma posicao para a esquerda
        for (int i = 0; i < Area->NumOcupadas - 1; i++) {
            Area->Registros[i] = Area->Registros[i + 1];
        }

        // decrementa o numero de registros
        Area->NumOcupadas--;
    }
}

// remove o ultimo registro da area
// corresponde ao registro de maior chave
void RetiraUltimo(TipoArea *Area, TipoRegistro *R) {

    // verifica se ha registros na area
    if (Area->NumOcupadas > 0) {

        // aponta para o ultimo registro valido
        Area->NumOcupadas--;

        // copia o maior registro
        *R = Area->Registros[Area->NumOcupadas];
    }
}

// remove o menor registro da area e atualiza o contador
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraPrimeiro(Area, R);
    *NRArea = Area->NumOcupadas;
}

// remove o maior registro da area e atualiza o contador
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraUltimo(Area, R);
    *NRArea = Area->NumOcupadas;
}

// le um registro do limite superior do arquivo
// usada quando a leitura acontece pela direita do intervalo
void LeSup(FILE *Arq, TipoRegistro *UltLido, int *Ls, short *OndeLer) {
    Aluno aluno;

    // posiciona o ponteiro no limite superior
    fseek(Arq, (*Ls - 1) * sizeof(Aluno), SEEK_SET);

    // realiza a leitura
    fread(&aluno, sizeof(Aluno), 1, Arq);
    metricas.leituras++;
    
    // atualiza o registro lido
    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;

    // move o limite superior para baixo
    (*Ls)--;

    // proxima leitura sera pelo limite inferior
    *OndeLer = 0;
}

// le um registro do limite inferior do arquivo
// usada quando a leitura acontece pela esquerda do intervalo
void LeInf(FILE *Arq, TipoRegistro *UltLido, int *Li, short *OndeLer) {
    Aluno aluno;

    // posiciona o ponteiro no limite inferior
    fseek(Arq, (*Li - 1) * sizeof(Aluno), SEEK_SET);

    // realiza a leitura
    fread(&aluno, sizeof(Aluno), 1, Arq);
    metricas.leituras++;

    // atualiza o registro lido
    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;

    // move o limite inferior para cima
    (*Li)++;

    // proxima leitura sera pelo limite superior
    *OndeLer = 1; 
}

// escreve um registro no limite superior do arquivo
// usada para gravar registros maiores
void EscreveMax(FILE *Arq, TipoRegistro R, int *Es) {
    fseek(Arq, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R.Aluno, sizeof(Aluno), 1, Arq);
    metricas.escritas++;

    // move o limite superior de escrita
    (*Es)--;
}

// escreve um registro no limite inferior do arquivo
// usada para gravar registros menores
void EscreveMin(FILE *Arq, TipoRegistro R, int *Ei) {
    fseek(Arq, (*Ei - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R.Aluno, sizeof(Aluno), 1, Arq);
    metricas.escritas++;

    // move o limite inferior de escrita
    (*Ei)++;
}

// realiza a particao do quicksort externo
// distribui registros menores para a esquerda e maiores para a direita
void Particao(FILE *Arq, TipoArea *Area, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq;
    int NRArea = 0;

    // limites de comparacao das chaves
    float Linf = -1e9, Lsup = 1e9;

    // indica de onde sera feita a proxima leitura
    short OndeLer = 1;

    TipoRegistro UltLido, R;
    
    // posiciona o ponteiro no inicio do intervalo
    fseek(Arq, (Li - 1) * sizeof(Aluno), SEEK_SET);

    // inicializa os indices de particao
    *i = Esq - 1;
    *j = Dir + 1;
    
    // executa enquanto houver registros no intervalo
    while (Ls >= Li) {

        // enquanto houver espaco na area, apenas insere
        if (NRArea < TAMAREA - 1) {

            if (OndeLer)
                LeSup(Arq, &UltLido, &Ls, &OndeLer);
            else
                LeInf(Arq, &UltLido, &Li, &OndeLer);

            InserirArea(Area, &UltLido, &NRArea);
            continue;
        }
        
        // escolhe de onde ler com base nos limites
        if (Ls == Es)
            LeSup(Arq, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei)
            LeInf(Arq, &UltLido, &Li, &OndeLer);
        else if (OndeLer)
            LeSup(Arq, &UltLido, &Ls, &OndeLer);
        else
            LeInf(Arq, &UltLido, &Li, &OndeLer);
        
        // verifica se a chave eh maior que o limite superior
        metricas.comparacoes++;
        if (UltLido.Chave > Lsup) {
            *j = Es;
            EscreveMax(Arq, UltLido, &Es);
            continue;
        }
        
        // verifica se a chave eh menor que o limite inferior
        metricas.comparacoes++;
        if (UltLido.Chave < Linf) {
            *i = Ei;
            EscreveMin(Arq, UltLido, &Ei);
            continue;
        }
        
        // insere o registro na area
        InserirArea(Area, &UltLido, &NRArea);
        
        // decide qual extremo remover da area
        metricas.comparacoes++;
        if (Ei - Esq < Dir - Es) {

            // remove o menor e escreve no limite inferior
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(Arq, R, &Ei);
            Linf = R.Chave;

        } else {

            // remove o maior e escreve no limite superior
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(Arq, R, &Es);
            Lsup = R.Chave;
        }
    }
    
    // grava os registros restantes da area
    while (Ei <= Es) {
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(Arq, R, &Ei);
    }
}

// implementacao do quicksort externo
// ordena o arquivo usando particionamento com memoria limitada
void QuicksortExterno(FILE *Arq, int Esq, int Dir) {
    int i, j;
    TipoArea Area;
    
    // condicao de parada da recursao
    if (Dir - Esq < 1) return;
    
    // inicializa a area
    FAVazia(&Area);

    // realiza a particao
    Particao(Arq, &Area, Esq, Dir, &i, &j);
    
    // aplica recursao priorizando o menor intervalo
    if (i - Esq < Dir - j) {
        QuicksortExterno(Arq, Esq, i);
        QuicksortExterno(Arq, j, Dir);
    } else {
        QuicksortExterno(Arq, j, Dir);
        QuicksortExterno(Arq, Esq, i);
    }
}
