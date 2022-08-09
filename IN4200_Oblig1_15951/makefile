.PHONY: serial omp clean default run_default


# --- Serial code --- #
CC = gcc
CFLAGS = -I.
DEPS = main.h
OBJ = main.o read_graph_from_file.o PageRank_iterations.o top_n_webpages.o utilities.o

serial: main clean

main: $(OBJ)
	$(CC) -o $@.out $^ $(CFLAGS)


# Compile serial code and run with default settings
default: serial run_default

run_default:
	./main.out 1 1e-6 8 8-webpages.txt


# --- Parallel code --- #
OBJ_OMP = main.o read_graph_from_file.o PageRank_iterations_omp.o top_n_webpages_omp.o utilities.o
CC_OMP = gcc -Xpreprocessor -fopenmp
CFLAGS_OMP = -lomp

omp: main_omp clean

main.o: main.c $(DEPS)
		gcc -c -o main.o main.c

read_graph_from_file.o: read_graph_from_file.c $(DEPS)
		gcc -c -o read_graph_from_file.o read_graph_from_file.c

PageRank_iterations_omp.o: PageRank_iterations_omp.c $(DEPS)
		$(CC_OMP) -c -o PageRank_iterations_omp.o PageRank_iterations_omp.c

top_n_webpages_omp.o: top_n_webpages_omp.c $(DEPS)
		$(CC_OMP) -c -o top_n_webpages_omp.o top_n_webpages_omp.c

utilities.o: utilities.c $(DEPS)
		gcc -c -o utilities.o utilities.c


main_omp: $(OBJ_OMP)
	$(CC_OMP) -o $@.out $^ $(CFLAGS_OMP)



# Remove o-files
clean:
	rm -f *.o *~
