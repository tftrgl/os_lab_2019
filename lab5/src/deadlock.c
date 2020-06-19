#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>
 
#include <pthread.h>
 
int begin = 2;
int end = 0;
int mod = 1;

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
 
 
void* Fact1(void *args) {
  int a = -1;
  int b = 1;
  int m = mod;
  printf("Fact1 1st lock\n");
  pthread_mutex_lock(&mut1);
  while (begin < 50000)
  {
      begin++;
  }
  printf("Fact1 2nd lock\n");
  pthread_mutex_lock(&mut2);
  m = begin + end;
  printf("Fact1 2nd unlock\n");
  pthread_mutex_unlock(&mut2);
  printf("Fact1 1st unlock\n");
  pthread_mutex_unlock(&mut1);
  int prod = 1;
  
  return 0;
}

void* Fact2(void *args) {
  int a = -1;
  int b = 1;
  int m = mod;
  printf("Fact2 1st lock\n");
  pthread_mutex_lock(&mut2);
  while (end < 50000)
  {
      end++;
  }
  printf("Fact1 2nd lock\n");
  pthread_mutex_lock(&mut1);
  m = begin + end;
  printf("Fact1 2nd unlock\n");
  pthread_mutex_unlock(&mut1);
  printf("Fact1 1st unlock\n");
  pthread_mutex_unlock(&mut2);
  
  return 0;
}
 
 
int main() {
  int pnum = 2;
  pthread_t threads[pnum];
  int *array = malloc(sizeof(int) * 1);
 
 
    if (pthread_create(&threads[0], NULL, Fact1, (void *)&array)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
    if (pthread_create(&threads[1], NULL, Fact2, (void *)&array)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
 
  int total_prod = 1;
  for (uint32_t i = 0; i < pnum; i++) {
    int prod = 0;
    pthread_join(threads[i], (void **)&prod);
    total_prod = (total_prod * prod) % mod;
  }
 
  free(array);
  printf("Total: %d\n", total_prod);
  return 0;
}