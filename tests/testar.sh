# Roteiro de verificação do trab1
# Rodar da raiz: bash tests/testar.sh
cd "$(dirname "$0")/.."

# pasta de resultados (limpa a cada execução)
RES=tests/results
rm -rf "$RES"
mkdir -p "$RES/saidas" "$RES/valgrind"

# detecta valgrind (não falha se não tiver)
if command -v valgrind > /dev/null; then
    HAS_VALGRIND=1
else
    HAS_VALGRIND=0
fi

echo "=== 1. Compilar ==="
make clean
make || { echo "ERRO: compilação falhou"; exit 1; }

echo ""
echo "=== 2. Validar saídas (casos pequenos com gabarito) ==="
PASS=0
FAIL=0
for f in tests/input/*_n*_k*.txt; do
    [ -e "$f" ] || continue
    name=$(basename "$f" .txt)
    k=${name##*_k}
    out="$RES/saidas/$name.txt"
    expected="tests/output/$name.txt"
    if [ ! -f "$expected" ]; then
        echo "SKIP $name (sem gabarito em $expected)"
        continue
    fi
    ./trab1 "$f" "$k" "$out"
    if diff -q "$out" "$expected" > /dev/null; then
        echo "OK   $name"
        PASS=$((PASS + 1))
    else
        echo "FAIL $name"
        FAIL=$((FAIL + 1))
    fi
done
echo "Resultado: $PASS OK, $FAIL FAIL"

echo ""
echo "=== 3. Valgrind (casos pequenos) ==="
if [ "$HAS_VALGRIND" -eq 0 ]; then
    echo "AVISO: valgrind não instalado. Instale com: sudo apt install valgrind"
    echo "Pulando esta etapa."
else
    VG_PASS=0
    VG_FAIL=0
    for f in tests/input/*_n*_k*.txt; do
        [ -e "$f" ] || continue
        name=$(basename "$f" .txt)
        k=${name##*_k}
        log="$RES/valgrind/$name.log"
        if valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 \
            ./trab1 "$f" "$k" /tmp/saida.txt 2>"$log"; then
            echo "OK   $name"
            VG_PASS=$((VG_PASS + 1))
        else
            echo "FAIL $name (ver $log)"
            VG_FAIL=$((VG_FAIL + 1))
        fi
    done
    echo "Resultado: $VG_PASS OK, $VG_FAIL FAIL"
fi

echo ""
echo "=== 4. Robustez (entradas inválidas / mal formadas) ==="
ROB_PASS=0
ROB_FAIL=0
TMP=$(mktemp -d)

# 4.1 arquivo vazio: deve retornar != 0
printf '' > "$TMP/empty.txt"
./trab1 "$TMP/empty.txt" 1 /tmp/o.txt 2>/dev/null
if [ $? -ne 0 ]; then echo "OK   arquivo vazio (exit != 0)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL arquivo vazio (deveria sair com erro)"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.2 linhas em branco no meio: deve ler os pontos válidos
printf 'A,1,2\n\n\nB,3,4\n\nC,5,6\n' > "$TMP/blanks.txt"
./trab1 "$TMP/blanks.txt" 3 /tmp/o.txt 2>/dev/null
if [ $(wc -l < /tmp/o.txt) -eq 3 ]; then echo "OK   linhas em branco"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL linhas em branco"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.3 CRLF (formato Windows): deve funcionar
printf 'A,1,2\r\nB,3,4\r\n' > "$TMP/crlf.txt"
./trab1 "$TMP/crlf.txt" 2 /tmp/o.txt 2>/dev/null
if [ $(wc -l < /tmp/o.txt) -eq 2 ]; then echo "OK   CRLF (Windows)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL CRLF (Windows)"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.4 linha mal formada: deve pular e ler as válidas
printf 'A,1,2\nB,3\nC,5,6\n' > "$TMP/malformed.txt"
./trab1 "$TMP/malformed.txt" 2 /tmp/o.txt 2>/dev/null
if [ $(wc -l < /tmp/o.txt) -eq 2 ]; then echo "OK   linha mal formada (pula)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL linha mal formada"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.5 k inválido (k=0, k>n, k='abc')
./trab1 tests/input/minimo_n2_k1.txt 0 /tmp/o.txt 2>/dev/null
A=$?
./trab1 tests/input/minimo_n2_k1.txt 99 /tmp/o.txt 2>/dev/null
B=$?
./trab1 tests/input/minimo_n2_k1.txt abc /tmp/o.txt 2>/dev/null
C=$?
if [ $A -ne 0 ] && [ $B -ne 0 ] && [ $C -ne 0 ]; then echo "OK   k inválido (exit != 0)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL k inválido"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.6 argc errado: deve retornar != 0
./trab1 2>/dev/null; A=$?
./trab1 tests/input/minimo_n2_k1.txt 2>/dev/null; B=$?
if [ $A -ne 0 ] && [ $B -ne 0 ]; then echo "OK   argc errado (exit != 0)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL argc errado"; ROB_FAIL=$((ROB_FAIL+1)); fi

# 4.7 arquivo inexistente: deve retornar != 0
./trab1 /tmp/nao_existe_$$ 1 /tmp/o.txt 2>/dev/null
if [ $? -ne 0 ]; then echo "OK   arquivo inexistente (exit != 0)"; ROB_PASS=$((ROB_PASS+1));
else echo "FAIL arquivo inexistente"; ROB_FAIL=$((ROB_FAIL+1)); fi

rm -rf "$TMP"
echo "Resultado: $ROB_PASS OK, $ROB_FAIL FAIL"

echo ""
echo "=== 5. Tempo nos inputs do professor ==="
TEMPOS="$RES/tempos.log"
: > "$TEMPOS"
declare -A K=( [1]=2 [2]=4 [3]=5 [4]=5 [5]=10 )
for n in 1 2 3 4 5; do
    if [ -f "input/$n.txt" ]; then
        echo "input/$n.txt (k=${K[$n]}):" | tee -a "$TEMPOS"
        { time ./trab1 "input/$n.txt" "${K[$n]}" /tmp/saida.txt; } 2>&1 | tee -a "$TEMPOS"
        echo "" | tee -a "$TEMPOS"
    fi
done

echo ""
echo "=== 6. Caso grande (tests/input/large_test.txt) ==="
LARGE=tests/input/large_test.txt
if [ -f "$LARGE" ]; then
    nlinhas=$(wc -l < "$LARGE")
    echo "$LARGE (n≈$nlinhas, k=5):" | tee -a "$TEMPOS"
    { time ./trab1 "$LARGE" 5 "$RES/saidas/large_test.txt"; } 2>&1 | tee -a "$TEMPOS"
    nclusters=$(wc -l < "$RES/saidas/large_test.txt")
    if [ "$nclusters" -eq 5 ]; then
        echo "OK   saída tem 5 clusters como esperado"
    else
        echo "FAIL saída tem $nclusters clusters (esperado 5)"
    fi
else
    echo "SKIP (arquivo $LARGE não existe)"
fi

echo ""
echo "=== Pronto. Resultados em $RES/ ==="
