L3PATH=/lab3/src/
L4PATH=/lab4/src/
CC=gcc -std=c99
CFLAGS=-I.
 
all : process_memory parallel_min_max parallel_sum
 
process_memory :
	cd /projects/tftrgl-os_lab_2019/lab4/src && $(CC) -o prMem process_memory.c $(CFLAGS)

parallel_min_max :
	cd /projects/tftrgl-os_lab_2019/lab3/src && $(CC) -o parallel_min_max utils.c find_min_max.c $(CFLAGS)

utils.o :
	cd /projects/tftrgl-os_lab_2019 && $(CC) -o utils.o -c /lab3/src/utils.c $(CFLAGS)

parallel_sum :
	cd /projects/tftrgl-os_lab_2019/lab3/src && $(CC) -pthread -o parallel_min_max utils.c find_min_max.c $(CFLAGS)
 
clean :
	rm process_memory
 
