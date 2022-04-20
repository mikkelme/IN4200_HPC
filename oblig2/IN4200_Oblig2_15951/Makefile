
all: serial parallel

quick_run: serial parallel


serial: 
	$(MAKE) -C serial_code


serial_run:
	serial_code/serial_main.out 0.2 100 mona_lisa_noisy.jpg 


parallel: 
	$(MAKE) -C parallel_code


parallel_run:
	mpirun -np 4 ./parallel_code/parallel_main.out 0.2 100 mona_lisa_noisy.jpg


clean:
	$(MAKE) -C serial_code clean
	$(MAKE) -C parallel_code clean

