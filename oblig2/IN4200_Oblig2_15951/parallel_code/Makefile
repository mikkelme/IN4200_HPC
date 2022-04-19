CC = mpicc #gcc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = parallel_main
OBJS = parallel_main.o ../utilities/functions.o ../utilities/functions_parallel.o


all: simple-jpeg $(PROJ) $(OBJS) obj-clean run


parallel_main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@.out $(LDFLAGS)



simple-jpeg:
	$(MAKE) -C ../simple-jpeg


run:
	mpirun -np 4 ./parallel_main.out

obj-clean:
	$(RM) *.o
	$(RM) ../utilities/*.o

exec-clean:
	$(RM) *.out

autosave-clean:
	$(RM) *~

clean:
	$(MAKE) obj-clean
	$(MAKE) exec-clean
	$(MAKE) autosave-clean
	$(MAKE) -C ../simple-jpeg clean
