#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/time.h>
 
#include <pthread.h>
#include "utils.h"
 
struct SumArgs {
  int *array;
  int begin;
  int end;
};
 
int count = 0;
 
int Sum(const struct SumArgs *args) {
  int sum = 0;
  int *ptr = args->array;
  int bg = args[count].begin;
  int ed = args[count].end;
  count++;
 
  while (bg != ed)
  {
      sum += ptr[bg];
      bg++;
  }
  return sum;
}
 
void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}
 
int main(int argc, char **argv) {
  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];
 
  while (true) {
    int current_optind = optind ? optind : 1;
 
    static struct option options[] = {{"threads_num", required_argument, 0, 0},
                                      {"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
 
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
 
    if (c == -1) break;
 
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            threads_num = atoi(optarg);
            if (threads_num <= 0)
            {
                printf("Threads number must be positive\n");
                return 1;
            }
            break;
          case 1:
            seed = atoi(optarg);
            if (seed <= 0)
            {
                printf("Seed must be positive\n");
                return 1;
            }
            break;
          case 2:
            array_size = atoi(optarg);
            if (array_size <= 0)
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
 
if (seed == 0 || array_size == 0 || threads_num == 0) {
    printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\" \n",
           argv[0]);
    return 1;
}
 
 
  int *array = malloc(sizeof(int) * array_size);
  struct SumArgs args[threads_num];
 
  GenerateArray(array, array_size, seed);
 
  struct timeval start_time;
  gettimeofday(&start_time, NULL);
 
  for (int i = 0; i < threads_num; i++)
  {
      args[i].array = array;
      args[i].begin = i*array_size/threads_num;
      args[i].end = (i+1)*array_size/threads_num;
  }
 
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }
 
  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }
 
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  double elapsed_time = (double)(finish_time.tv_usec - start_time.tv_usec)/1000;
 
  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}