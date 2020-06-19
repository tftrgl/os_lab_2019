L3PATH=lab3/src/
L4PATH=lab4/src/
CC=gcc -std=c99
CFLAGS=-I.
 
all : process_memory parallel_min_max parallel_sum
 
process_memory :
	cd /projects/tftrgl-os_lab_2019/lab4/src && $(CC)  -o prMem process_memory.c $(CFLAGS)

parallel_min_max :
	cd /projects/tftrgl-os_lab_2019/lab3/src && $(CC) -o parallel_min_max parallel_min_max.c utils.c find_min_max.c $(CFLAGS)

parallel_sum :
	cd /projects/tftrgl-os_lab_2019 && $(CC) -pthread -o $(L4PATH)parallel_sum $(L4PATH)parallel_sum.c $(L3PATH)utils.c $(CFLAGS)
 
clean :
	rm process_memory parallel_min_max parallel_sum
 
