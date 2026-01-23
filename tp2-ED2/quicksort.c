#include "structs.h"


//heap ordenado

//inicializa a area vazia
void FAVazia(TipoArea *Area) {
    Area->NumOcupadas = 0;
}

//insere um registro na area mantendo a ordenacao crescente
void InserirArea(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    int i = Area->NumOcupadas; //aponta pro proximo elemento livre
    
    //movimenta os elementos maiores que a chave para a direita
    while (i > 0 && Area->Registros[i-1].Chave > R->Chave) {
        metricas.comparacoes++;
        Area->Registros[i] = Area->Registros[i-1];
        i--;
    }

    //se der erro contabiliza a comparacao e encerra o loop
    if (i > 0) metricas.comparacoes++;
    

    //insere na posicao correta, incrementa o contador de posicoes ocupadas e retorna o novo numero de elementos
    Area->Registros[i] = *R;
    Area->NumOcupadas++;
    *NRArea = Area->NumOcupadas;
}

//remove o primeiro registro(menor)
void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R) {
    //verifica se tem elementos na area
    if (Area->NumOcupadas > 0) {
        *R = Area->Registros[0];
        for (int i = 0; i < Area->NumOcupadas - 1; i++) { //desloca tudo uma posicao para a esquerda e subescreve o primeiro
            Area->Registros[i] = Area->Registros[i + 1];
        }
        Area->NumOcupadas--;
    }
}


//remove o ultimo registro(maior)
void RetiraUltimo(TipoArea *Area, TipoRegistro *R) {
    if (Area->NumOcupadas > 0) { //ve se tem elementos
        Area->NumOcupadas--;//aponta pro ultimo valido
        *R = Area->Registros[Area->NumOcupadas];
    }
}

//remove o registro de menor chave(primeiro)
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraPrimeiro(Area, R);
    *NRArea = Area->NumOcupadas;
}

//remove o registro de maior chave(ultimo)
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraUltimo(Area, R);
    *NRArea = Area->NumOcupadas;
}

//funçoes para leitura do arquivo

//le um registro da parte superior do arquivo(decrescente)
void LeSup(FILE *Arq, TipoRegistro *UltLido, int *Ls, short *OndeLer) {
    Aluno aluno;
    fseek(Arq, (*Ls - 1) * sizeof(Aluno), SEEK_SET); //posiciona o ponteiro do arquivo em ls
    fread(&aluno, sizeof(Aluno), 1, Arq); //faz a leitura na posicao atual
    metricas.leituras++;
    
    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;
    (*Ls)--; //movimenta o limite superior para baixo
    *OndeLer = 0;
}

//le um registro da parte superior do arquivo(crescente)
void LeInf(FILE *Arq, TipoRegistro *UltLido, int *Li, short *OndeLer) {
    Aluno aluno;
    fseek(Arq, (*Li - 1) * sizeof(Aluno), SEEK_SET);//posiciona o ponteiro do arquivo em li
    fread(&aluno, sizeof(Aluno), 1, Arq);
    metricas.leituras++;

    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;
    (*Li)++; //movimenta o limite inferior para cima
    *OndeLer = 1; 
}


//funçoes para ecrita no arquivo

////escreve na parte superior do arquivo(descres)
void EscreveMax(FILE *Arq, TipoRegistro R, int *Es) {
    fseek(Arq, (*Es - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R.Aluno, sizeof(Aluno), 1, Arq);
    metricas.escritas++;
    (*Es)--;
}

void EscreveMin(FILE *Arq, TipoRegistro R, int *Ei) {
    fseek(Arq, (*Ei - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&R.Aluno, sizeof(Aluno), 1, Arq);
    metricas.escritas++;
    (*Ei)++;
}


//particionamento
void Particao(FILE *Arq, TipoArea *Area, int Esq, int Dir, int *i, int *j) {
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;
    float Linf = -1e9, Lsup = 1e9;
    short OndeLer = 1;
    TipoRegistro UltLido, R;
    
    fseek(Arq, (Li - 1) * sizeof(Aluno), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;
    
    while (Ls >= Li) {
        if (NRArea < TAMAREA - 1) {
            if (OndeLer)
                LeSup(Arq, &UltLido, &Ls, &OndeLer);
            else
                LeInf(Arq, &UltLido, &Li, &OndeLer);
            InserirArea(Area, &UltLido, &NRArea);
            continue;
        }
        
        if (Ls == Es)
            LeSup(Arq, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei)
            LeInf(Arq, &UltLido, &Li, &OndeLer);
        else if (OndeLer)
            LeSup(Arq, &UltLido, &Ls, &OndeLer);
        else
            LeInf(Arq, &UltLido, &Li, &OndeLer);
        
        metricas.comparacoes++;
        if (UltLido.Chave > Lsup) {
            *j = Es;
            EscreveMax(Arq, UltLido, &Es);
            continue;
        }
        
        metricas.comparacoes++;
        if (UltLido.Chave < Linf) {
            *i = Ei;
            fseek(Arq, (Ei - 1) * sizeof(Aluno), SEEK_SET);
            EscreveMin(Arq, UltLido, &Ei);
            fseek(Arq, (Li - 1) * sizeof(Aluno), SEEK_SET);
            continue;
        }
        
        InserirArea(Area, &UltLido, &NRArea);
        
        metricas.comparacoes++;
        if (Ei - Esq < Dir - Es) {
            RetiraMin(Area, &R, &NRArea);
            fseek(Arq, (Ei - 1) * sizeof(Aluno), SEEK_SET);
            EscreveMin(Arq, R, &Ei);
            Linf = R.Chave;
            fseek(Arq, (Li - 1) * sizeof(Aluno), SEEK_SET);
        } else {
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(Arq, R, &Es);
            Lsup = R.Chave;
        }
    }
    
    fseek(Arq, (Ei - 1) * sizeof(Aluno), SEEK_SET);
    while (Ei <= Es) {
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(Arq, R, &Ei);
    }
}


//quick
void QuicksortExterno(FILE *Arq, int Esq, int Dir) {
    int i, j;
    TipoArea Area;
    
    if (Dir - Esq < 1) return;
    
    FAVazia(&Area);
    Particao(Arq, &Area, Esq, Dir, &i, &j);
    
    if (i - Esq < Dir - j) {
        QuicksortExterno(Arq, Esq, i);
        QuicksortExterno(Arq, j, Dir);
    } else {
        QuicksortExterno(Arq, j, Dir);
        QuicksortExterno(Arq, Esq, i);
    }
}