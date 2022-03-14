#
#
#

# .PHONY = serial_task
#
# CC = gcc
# FLAGS = -Wall
#
# DEP = functions.h
#
# serial_task: compile_serial run_serial
#
# # serial_task: compile_serial
#
# compile_serial: serial.c $(DEP)
# 	gcc serial.c -o serial.out
#
# run_serial:
# 	./serial.out
#




CC = gcc
FLAGS = -Wall
DEP = header.h utilities.h

.PHONY: all

all: program run

program: main.c read_graph_from_file.c PageRank_iterations.c  $(DEP)
	gcc main.c read_graph_from_file.c PageRank_iterations.c -o main.out

	#top_n_webpages.c


run:
	./main.out





# CC := gcc
# FLAGS = -Wall
#
# PROGRAMS = $(patsubst %.c,%.exe,$(wildcard *.c))
# DEP = oddeven.h
#
# .PHONY : all clean
#
# all: $(PROGRAMS)
#
# w8_e2_output.exe : w8_e2_output.c
# 	$(CC) $< -o $@ -fopenmp $(FLAGS)
#
# w8_e3_oddeven.exe : w8_e3_oddeven.c $(DEP)
# 	$(CC) $< -o $@ -fopenmp $(FLAGS)
#
# clean :
# 	rm -f *.o $(PROGRAMS)
#
# print-%  : ; @echo $* = $($*)

#
# CPPflags = c++ -O3 -std=c++11
# LIB =  -larmadillo #-llapack -lblas
# PROG = main
#
# ${PROG}: 						${PROG}.o functions.o
# 										${CPPflags} ${PROG}.o functions.o ${LIB} -o ${PROG}.exe
#
#
# ${PROG}.o: 					${PROG}.cpp
# 										${CPPflags} -c ${PROG}.cpp
#
# functions.o: 				functions.cpp functions.h
# 										${CPPflags} -c functions.cpp
#
#
# clean:
# 	rm -f *.o *~
