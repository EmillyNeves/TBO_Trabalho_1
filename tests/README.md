# Casos de teste

Casos pequenos com saídas esperadas conhecidas (calculadas à mão ou retiradas do PDF da especificação).

## Estrutura

```
tests/
├── inputs/      # arquivos de entrada
├── expected/    # saídas esperadas (gabarito)
└── README.md
```

Cada caso tem um arquivo de mesmo nome em `inputs/` e em `expected/`. O nome do arquivo segue o padrão `<tipo>_n<N>_k<K>.txt`, onde N é o número de pontos e K é o número de grupos.

## Casos

| Caso                          | n  | m | k | O que testa                                                |
|-------------------------------|----|---|---|------------------------------------------------------------|
| `exemplo_pdf_n10_k3`          | 10 | 2 | 3 | Caso canônico do PDF (seção 2.3) — gold standard           |
| `trivial_juntos_n4_k1`        | 4  | 2 | 1 | k=1 → todos os pontos no mesmo grupo                       |
| `trivial_separados_n4_k4`     | 4  | 2 | 4 | k=n → cada ponto sozinho no seu grupo                      |
| `minimo_n2_k1`                | 2  | 2 | 1 | Menor entrada possível                                     |
| `clusters_obvios_n4_k2`       | 4  | 2 | 2 | Dois clusters muito separados — valida agrupamento básico  |

## Verificação completa (script único)

Da raiz do projeto, basta rodar:

```bash
bash tests/testar.sh
```

O script `testar.sh` executa 4 etapas em sequência: (1) compila, (2) valida saídas dos casos pequenos contra o gabarito, (3) checa vazamento de memória com Valgrind, e (4) mede tempo nos inputs do professor (`input/1.txt` a `input/5.txt`).

Se preferir rodar passo a passo, segue abaixo.

## Como rodar (no Linux/WSL, da raiz do projeto)

Caso individual:

```bash
./trab1 tests/inputs/exemplo_pdf_n10_k3.txt 3 saida.txt
diff saida.txt tests/expected/exemplo_pdf_n10_k3.txt
```

Se o `diff` não imprimir nada, a saída bate com o gabarito.

Loop pra rodar todos:

```bash
for f in tests/inputs/*.txt; do
    name=$(basename "$f" .txt)
    k=${name##*_k}              # extrai o k do final do nome
    ./trab1 "$f" "$k" /tmp/saida.txt
    if diff -q /tmp/saida.txt "tests/expected/$name.txt" > /dev/null; then
        echo "OK   $name"
    else
        echo "FAIL $name"
    fi
done
```

## Verificação de vazamento de memória (Valgrind)

A especificação exige que o programa não vaze memória (todo `malloc` com seu `free`). Os casos pequenos rodam em milissegundos sob o Valgrind, então dá pra rodar em todos rapidamente.

Caso individual:

```bash
valgrind --leak-check=full --error-exitcode=1 ./trab1 tests/inputs/exemplo_pdf_n10_k3.txt 3 /tmp/saida.txt
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

Loop pra rodar Valgrind em todos os casos:

```bash
for f in tests/inputs/*.txt; do
    name=$(basename "$f" .txt)
    k=${name##*_k}
    if valgrind --leak-check=full --error-exitcode=1 -q ./trab1 "$f" "$k" /tmp/saida.txt 2>/tmp/vg.log; then
        echo "OK   $name"
    else
        echo "FAIL $name"
        cat /tmp/vg.log
    fi
done
```

A flag `-q` silencia a saída do Valgrind quando não há erros — só mostra problemas. Se algum caso falhar, o log fica em `/tmp/vg.log`.

## Notas sobre os casos

- **`trivial_juntos`** e **`trivial_separados`** usam o mesmo input (4 pontos colineares com espaçamento crescente: 0, 1, 3, 7) e diferem apenas no `k`. Coordenadas escolhidas para evitar empates de distância.
- **`clusters_obvios`** usa pontos `(0,0)`, `(0,1)`, `(100,100)`, `(101,101)` — duas duplas distantes, sem empates de distância.
- O caractere de fim de linha em todos os arquivos é `\n` (sem `\r\n`), conforme especificação.
