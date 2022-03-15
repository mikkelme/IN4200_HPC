

CC = gcc
CFLAGS = -I.
DEPS = main.h
OBJ = main.o read_graph_from_file.o PageRank_iterations.o utilities.o top_n_webpages.o

all: main clean run

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@.out $^ $(CFLAGS)

run:
	 	./main.out

clean:
	rm -f *.o *~
