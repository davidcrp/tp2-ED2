# TP2 — Ordenação externa (Estrutura de Dados II)

> Trabalho prático em grupo da disciplina **BCC203 — Estrutura de Dados II** (Ciência da Computação, UFOP).
> Repositório original: [pedrosouzag/tp2-ED2](https://github.com/pedrosouzag/tp2-ED2).

Implementação e comparação de métodos de **ordenação externa** sobre um arquivo real de 471.705 registros de alunos (notas do Provão). Cada execução mede leituras, escritas, comparações e tempo.

**Stack:** C · arquivos binários · gcc

## Métodos

| # | Método | Arquivo |
|---|---|---|
| 1 | Intercalação balanceada com 2f fitas (10 de entrada + 10 de saída) | `2f.c` |
| 2 | Intercalação balanceada com f+1 fitas (19 de entrada + 1 de saída) | `f1.c` |
| 3 | Quicksort externo | `quicksort.c` |

Os métodos de intercalação geram os blocos iniciais por **seleção por substituição** (`selecaoPorSubstituicao.c`).

## Compilar e executar

```bash
cd tp2-ED2
mkdir -p blocos
gcc *.c -o ordena
./ordena <metodo> <quantidade> <situacao> [-P]
```

- `metodo`: 1, 2 ou 3 (tabela acima)
- `quantidade`: número de alunos a ordenar (100, 200, 2000, 20000, 200000 ou 471705)
- `situacao`: 1 ordenado ascendente (melhor caso), 2 descendente (pior caso), 3 aleatório (caso médio)
- `-P`: imprime os dados antes e depois da ordenação

Exemplo: `./ordena 3 100 3` roda o quicksort externo em 100 alunos em ordem aleatória.

Entrada: `PROVAO.TXT`. Saída ordenada: `RESULTADO_FINAL.TXT`.
