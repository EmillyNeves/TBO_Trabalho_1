all:
	gcc -Wall -Wextra -g src/main.c src/point/point.c src/graph/graph.c src/union_find/union_find.c src/kruskal/kruskal.c src/io/io.c -o trab1 -lm

clean:
	rm -f trab1
