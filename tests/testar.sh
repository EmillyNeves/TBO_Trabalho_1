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
echo "=== 2. Validar saídas (casos pequenos) ==="
PASS=0
FAIL=0
# casos pequenos seguem o padrão <nome>_n<N>_k<K>.txt
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
echo "=== 4. Tempo nos inputs do professor ==="
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

echo "=== Pronto. Resultados em $RES/ ==="
