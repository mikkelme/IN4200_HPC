CC = gcc
CFLAGS = -O2
LDFLAGS = -L../../one_folder/simple-jpeg/ -lsimplejpeg

PROJ = serial_main
OBJS = serial_main.o

all: simple-jpeg $(PROJ) obj-clean run


serial_main: $(OBJS)
	$(CC) $(CFLAGS) $< -o $@.out $(LDFLAGS)

simple-jpeg:
	$(MAKE) -C ../../one_folder/simple-jpeg


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
