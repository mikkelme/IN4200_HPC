CC = gcc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = serial_main
OBJS = serial_main.o ../utilities/functions.o


all: simple-jpeg $(PROJ) $(OBJS) obj-clean run
 

serial_main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@.out $(LDFLAGS)



simple-jpeg:
	$(MAKE) -C ../simple-jpeg


run:
	./serial_main.out

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
	$(MAKE) -C ../../one_folder/simple-jpeg clean
