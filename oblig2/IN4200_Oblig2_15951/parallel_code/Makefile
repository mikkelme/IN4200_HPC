CC = mpicc#gcc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = parallel_main
OBJS = parallel_main.o ../utilities/functions.o


all: simple-jpeg $(PROJ) $(OBJS) obj-clean run


parallel_main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@.out $(LDFLAGS)


parallel_main.o: parallel_main.c
	$(CC) $(CFLAGS) -c parallel_main.c


functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c


simple-jpeg:
	$(MAKE) -C ../simple-jpeg


run:
	mpirun -np 4 ./parallel_main.out

obj-clean:
	$(RM) *.o

exec-clean:
	$(RM) $(PROJ)

autosave-clean:
	$(RM) *~

clean:
	$(MAKE) obj-clean
	$(MAKE) exec-clean
	$(MAKE) autosave-clean
	$(MAKE) -C ../../one_folder/simple-jpeg clean
