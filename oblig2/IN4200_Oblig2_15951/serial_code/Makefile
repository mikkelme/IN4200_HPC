CC = gcc
CFLAGS = -O2
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

PROJ = serial_main
OBJS = serial_main.o ../utilities/functions.o


all: simple-jpeg $(PROJ) $(OBJS) obj-clean run


serial_main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@.out $(LDFLAGS)



serial_main.o: serial_main.c
	$(CC) $(CFLAGS) -c serial_main.c


functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c


simple-jpeg:
	$(MAKE) -C ../simple-jpeg


run:
	./serial_main.out

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
