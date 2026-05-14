# Casos de teste

Casos pequenos com saídas esperadas conhecidas (calculadas à mão ou retiradas do PDF da especificação).

## Estrutura

```
tests/
├── input/        # arquivos de entrada
├── output/       # saídas esperadas (gabarito)
├── results/      # resultados gerados pelo script (saidas, valgrind, tempos)
├── testar.sh     # script único de verificação
└── README.md
```

Cada caso tem um arquivo de mesmo nome em `input/` e em `output/`. O nome do arquivo segue o padrão `<tipo>_n<N>_k<K>.txt`, onde N é o número de pontos e K é o número de grupos.

## Casos

| Caso                       | n  | m | k | O que testa                                                        |
|----------------------------|----|---|---|--------------------------------------------------------------------|
| `exemplo_pdf_n10_k3`       | 10 | 2 | 3 | Caso canônico do PDF (seção 2.3) — gold standard                   |
| `trivial_juntos_n4_k1`     | 4  | 2 | 1 | k=1 → todos os pontos no mesmo grupo                               |
| `trivial_separados_n4_k4`  | 4  | 2 | 4 | k=n → cada ponto sozinho no seu grupo                              |
| `minimo_n2_k1`             | 2  | 2 | 1 | Menor entrada possível                                             |
| `clusters_obvios_n4_k2`    | 4  | 2 | 2 | Dois clusters muito separados — valida agrupamento básico          |
| `tres_dimensoes_n5_k2`     | 5  | 3 | 2 | Coordenadas em 3D — valida que m>2 funciona                        |
| `negativos_n5_k2`          | 5  | 2 | 2 | Coordenadas negativas — valida cálculo de distância com sinais     |
| `quase_separados_n5_k4`    | 5  | 2 | 4 | k = n-1 → um par junto e três isolados                             |
| `ordem_lex_n6_k2`          | 6  | 2 | 2 | Ids `P10`, `P2`, `P100`… — valida ordenação lex, não numérica      |
| `tres_clusters_n9_k3`      | 9  | 2 | 3 | Três clusters em forma de "L" — valida múltiplos grupos            |

## Verificação completa (script único)

Da raiz do projeto, basta rodar:

```bash
bash tests/testar.sh
```

O script `testar.sh` executa 4 etapas em sequência:
1. **Compila** o projeto.
2. **Valida** saídas dos casos pequenos (pasta `tests/input/`) contra o gabarito (`tests/output/`).
3. **Roda Valgrind** em todos os casos pequenos (pula com aviso se o valgrind não estiver instalado).
4. **Mede tempo** nos inputs do professor (`input/1.txt` a `input/5.txt`) com os valores de `k` fixados no script.

Os resultados são salvos em `tests/results/`:
- `saidas/<caso>.txt` — saída gerada pelo programa.
- `valgrind/<caso>.log` — log do valgrind para cada caso.
- `tempos.log` — tempos dos inputs do professor.

## Como rodar (no Linux/WSL, da raiz do projeto)

Caso individual:

```bash
./trab1 tests/input/exemplo_pdf_n10_k3.txt 3 saida.txt
diff saida.txt tests/output/exemplo_pdf_n10_k3.txt
```

Se o `diff` não imprimir nada, a saída bate com o gabarito.

Loop para rodar todos os casos pequenos:

```bash
for f in tests/input/*_n*_k*.txt; do
    name=$(basename "$f" .txt)
    k=${name##*_k}
    ./trab1 "$f" "$k" /tmp/saida.txt
    if diff -q /tmp/saida.txt "tests/output/$name.txt" > /dev/null; then
        echo "OK   $name"
    else
        echo "FAIL $name"
    fi
done
```

## Verificação de vazamento de memória (Valgrind)

A especificação exige que o programa não vaze memória (todo `malloc` com seu `free`). Os casos pequenos rodam em milissegundos sob o Valgrind.

Pré-requisito (Ubuntu/Debian):

```bash
sudo apt install valgrind
```

Caso individual:

```bash
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 \
    ./trab1 tests/input/exemplo_pdf_n10_k3.txt 3 /tmp/saida.txt
```

Saída esperada (resumida):

```
==XXXX== HEAP SUMMARY:
==XXXX==     in use at exit: 0 bytes in 0 blocks
==XXXX==   total heap usage: N allocs, N frees, ... bytes allocated
==XXXX==
==XXXX== All heap blocks were freed -- no leaks are possible
==XXXX==
==XXXX== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

`in use at exit: 0 bytes` e `ERROR SUMMARY: 0 errors` indicam que o programa está livre de vazamento e de acesso inválido à memória.

## Notas sobre os casos

- **`trivial_juntos`** e **`trivial_separados`** usam o mesmo input (4 pontos colineares com espaçamento crescente: 0, 1, 3, 7) e diferem apenas no `k`. Coordenadas escolhidas para evitar empates de distância.
- **`clusters_obvios`** usa pontos `(0,0)`, `(0,1)`, `(100,100)`, `(101,101)` — duas duplas distantes, sem empates de distância.
- **`tres_dimensoes`** valida que o programa lê e processa qualquer `m` (não apenas 2D).
- **`ordem_lex`** é importante: ids alfanuméricos como `P10`, `P2`, `P100` devem ser ordenados lexicograficamente (`P10` < `P100` < `P2`), não numericamente.
- **`tres_clusters`** garante que o algoritmo separa corretamente em mais de 2 grupos sem misturas.
- O caractere de fim de linha em todos os arquivos é `\n` (sem `\r\n`), conforme especificação.
