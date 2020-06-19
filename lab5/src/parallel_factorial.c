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

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
 
 
int Fact(const int *args) {
  int a = -1;
  int b = 1;
  int m = mod;
  int prod = 1;
  while (a<=b)
  {
      pthread_mutex_lock(&mut);
      a = begin;
      b = end;
      ++begin;
      --end;
      while (a == begin || b == end);
      if (a==b)
      {
          prod = (prod * a) % m;
          pthread_mutex_unlock(&mut);
          break;
      }
      if (a>b) 
      {
          pthread_mutex_unlock(&mut);
          break;
      }
      pthread_mutex_unlock(&mut);
      prod = (prod * a * b) % m;
  }
  return prod;
}
 
void *ThreadFact(void *args) {
  return (void *)(size_t)Fact(args);
}
 
int main(int argc, char **argv) {
  int k = 0;
  int pnum = 0;
  pthread_t threads[pnum];
 
  while (true) {
    int current_optind = optind ? optind : 1;
 
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
 
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
 
    if (c == -1) break;
 
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            end = k;
            if (k <= 0)
            {
                printf("Threads number must be positive\n");
                return 1;
            }
            break;
          case 1:
            pnum = atoi(optarg);
            if (pnum <= 0)
            {
                printf("Seed must be positive\n");
                return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod <= 0)
            {
                printf("Array size must be positive\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
            break;
        }
        break;
 
      case '?':
        break;
 
      default:
        printf("getopt returned character code 0%o?\n", c);
        break;
    }
  }
 
if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }
 
if (mod == 1 || pnum == 0 || k == 0) {
    printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\" \n",
           argv[0]);
    return 1;
}
 
 
  int *array = malloc(sizeof(int) * 1);
 
  struct timeval start_time;
  gettimeofday(&start_time, NULL);
 
  for (uint32_t i = 0; i < pnum; i++) {
    if (pthread_create(&threads[i], NULL, ThreadFact, (void *)&array)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }
 
  int total_prod = 1;
  for (uint32_t i = 0; i < pnum; i++) {
    int prod = 0;
    pthread_join(threads[i], (void **)&prod);
    total_prod = (total_prod * prod) % mod;
  }
 
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  double elapsed_time = (double)(finish_time.tv_usec - start_time.tv_usec)/1000;
 
  free(array);
  printf("Total: %d\n", total_prod);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}