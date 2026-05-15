all:
	gcc -O2 -Wall -Wextra src/main.c src/point/point.c src/graph/graph.c src/union_find/union_find.c src/kruskal/kruskal.c src/io/io.c -o trab1 -lm

bench:
	gcc -Wall -Wextra -O2 bench/bench.c src/point/point.c src/graph/graph.c src/union_find/union_find.c src/io/io.c -o bench/trab1_bench -lm

clean:
	rm -f trab1 bench/trab1_bench bench/trab1_bench.exe
