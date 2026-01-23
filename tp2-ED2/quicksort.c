/*
 * quicksort.c - Implementação do Quicksort Externo
 * Algoritmo de Monard (1980)
 */

#include "structs.h"
// ============================================================================
// FUNÇÕES DA ÁREA DE MEMÓRIA (HEAP ORDENADO)
// ============================================================================

void FAVazia(TipoArea *Area) {
    Area->NumOcupadas = 0;
}

void InserirArea(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    int i = Area->NumOcupadas;
    
    while (i > 0 && Area->Registros[i-1].Chave > R->Chave) {
        metricas.comparacoes++;
        Area->Registros[i] = Area->Registros[i-1];
        i--;
    }
    if (i > 0) metricas.comparacoes++;
    
    Area->Registros[i] = *R;
    Area->NumOcupadas++;
    *NRArea = Area->NumOcupadas;
}

void RetiraPrimeiro(TipoArea *Area, TipoRegistro *R) {
    if (Area->NumOcupadas > 0) {
        *R = Area->Registros[0];
        for (int i = 0; i < Area->NumOcupadas - 1; i++) {
            Area->Registros[i] = Area->Registros[i + 1];
        }
        Area->NumOcupadas--;
    }
}

void RetiraUltimo(TipoArea *Area, TipoRegistro *R) {
    if (Area->NumOcupadas > 0) {
        Area->NumOcupadas--;
        *R = Area->Registros[Area->NumOcupadas];
    }
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraPrimeiro(Area, R);
    *NRArea = Area->NumOcupadas;
}

void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    RetiraUltimo(Area, R);
    *NRArea = Area->NumOcupadas;
}

// ============================================================================
// FUNÇÕES DE LEITURA DO ARQUIVO
// ============================================================================

void LeSup(FILE *Arq, TipoRegistro *UltLido, int *Ls, short *OndeLer) {
    Aluno aluno;
    fseek(Arq, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(&aluno, sizeof(Aluno), 1, Arq);
    metricas.leituras++;
    
    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;
    (*Ls)--;
    *OndeLer = 0;
}

void LeInf(FILE *Arq, TipoRegistro *UltLido, int *Li, short *OndeLer) {
    Aluno aluno;
    fseek(Arq, (*Li - 1) * sizeof(Aluno), SEEK_SET);
    fread(&aluno, sizeof(Aluno), 1, Arq);
    metricas.leituras++;

    UltLido->Chave = aluno.nota;
    UltLido->Aluno = aluno;
    (*Li)++;
    *OndeLer = 1;
}


// ============================================================================
// FUNÇÕES DE ESCRITA NO ARQUIVO
// ============================================================================

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


// ============================================================================
// PARTICIONAMENTO
// ============================================================================

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

// ============================================================================
// QUICKSORT EXTERNO
// ============================================================================

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