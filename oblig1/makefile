

CC = gcc
CFLAGS = -I.
DEPS = main.h
OBJ = main.o read_graph_from_file.o PageRank_iterations.o top_n_webpages.o utilities.o
.PHONY: all parallel clean


# Compile the serial code
serial: main clean run

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@.out $^ $(CFLAGS)

run:
	 	./main.out



# Compile the parallel code
OBJ_OMP = main.o read_graph_from_file.o PageRank_iterations_omp.o top_n_webpages_omp.o utilities.o
CC_OMP = gcc -Xpreprocessor -fopenmp
CFLAGS_OMP = -lomp
omp: main_omp run_omp clean

%.o: %.c $(DEPS)
		$(CC_OMP) -c -o $@ $< $(CFLAGS_OMP)

main_omp: $(OBJ_OMP)
	$(CC_OMP) -o $@.out $^ $(CFLAGS_OMP)

run_omp:
	 	./main_omp.out


# Remove o-files
clean:
	rm -f *.o *~
