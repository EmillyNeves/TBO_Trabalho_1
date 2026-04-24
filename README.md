# TBO - Trabalho 1: Maximum Spacing Clustering

Implementação de um algoritmo de clusterização por máximo espaçamento usando Árvore Geradora Mínima (MST) com o algoritmo de Kruskal.

## Estrutura do Projeto

```
TBO_Trabalho_1/
├── Makefile
├── src/
│   ├── main.c
│   ├── point/          # Struct e operações de ponto m-dimensional
│   ├── graph/          # Struct de grafo completo e cálculo de arestas
│   ├── union_find/     # Estrutura Union-Find para o Kruskal
│   └── io/             # Leitura do arquivo de entrada e escrita da saída
├── input/              # Casos de teste
└── docs/               # Especificação e modelo do projeto
```

## Como Compilar

```bash
make
```

Gera o executável `trab1`.

## Como Executar

```bash
./trab1 <arquivo_entrada> <k> <arquivo_saida>
```

**Exemplo:**
```bash
./trab1 input/1.txt 2 saida.txt
```
## Formato de Entrada

Cada linha: `<identificador>,<coord1>,<coord2>,...,<coordM>`

O número de dimensões `m` e de pontos `n` são descobertos dinamicamente.

## Formato de Saída

Um cluster por linha, elementos separados por vírgula em ordem lexicográfica. Clusters ordenados pelo primeiro elemento.
