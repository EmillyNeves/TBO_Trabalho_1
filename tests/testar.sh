# Roteiro de verificação do trab1
# Rodar da raiz: bash tests/testar.sh
cd "$(dirname "$0")/.."

# pasta de resultados (limpa a cada execução)
RES=tests/results
rm -rf "$RES"
mkdir -p "$RES/saidas" "$RES/valgrind"

echo "=== 1. Compilar ==="
make clean
make || { echo "ERRO: compilação falhou"; exit 1; }

echo ""
echo "=== 2. Validar saídas (casos pequenos) ==="
for f in tests/input/*.txt; do
    name=$(basename "$f" .txt)
    k=${name##*_k}
    out="$RES/saidas/$name.txt"
    ./trab1 "$f" "$k" "$out"
    if diff -q "$out" "tests/output/$name.txt" > /dev/null; then
        echo "OK   $name"
    else
        echo "FAIL $name"
    fi
done

echo ""
echo "=== 3. Valgrind (casos pequenos) ==="
for f in tests/input/*.txt; do
    name=$(basename "$f" .txt)
    k=${name##*_k}
    log="$RES/valgrind/$name.log"
    if valgrind --leak-check=full --error-exitcode=1 ./trab1 "$f" "$k" /tmp/saida.txt 2>"$log"; then
        echo "OK   $name"
    else
        echo "FAIL $name (ver $log)"
    fi
done

echo ""
echo "=== 4. Tempo nos input do professor ==="
TEMPOS="$RES/tempos.log"
declare -A K=( [1]=2 [2]=4 [3]=5 [4]=5 [5]=10 )
for n in 1 2 3 4 5; do
    if [ -f "input/$n.txt" ]; then
        echo "input/$n.txt (k=${K[$n]}):" | tee -a "$TEMPOS"
        { time ./trab1 "input/$n.txt" "${K[$n]}" /tmp/saida.txt; } 2>&1 | tee -a "$TEMPOS"
        echo "" | tee -a "$TEMPOS"
    fi
done

echo "=== Pronto. Resultados em $RES/ ==="
