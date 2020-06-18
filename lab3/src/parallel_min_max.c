#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) 
            {
                printf("Seed must be positive\n");
                return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) 
            {
                printf("Array size must be positive\n");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) 
            {
                printf("Process number must be positive\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          case 4:
            timeout = atoi(optarg);
            if (timeout <= 0) 
            {
                printf("Timeout must be positive\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
            break;
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int (*fd)[2] = malloc(sizeof(int) * 2 * pnum);
  for (int i = 0; i < pnum; i++)
  {
      if (pipe(fd[i])==-1) 
          { 
              printf("Pipe Failed\n");
              return 1;
          }
  }

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        int arrPart = array_size / pnum;
        struct MinMax min_max0 = GetMinMax(array, i*arrPart, (i+1)*arrPart);

        if (with_files) {
            FILE * fp1;
            FILE * fp2;
            fp1 = fopen ("min.txt", "a");
            fp2 = fopen ("max.txt",  "a");
            if (fp1 != NULL && fp2 != NULL)
            {
                fprintf(fp1, " %d", min_max0.min);
                fprintf(fp2, " %d", min_max0.max);
                fclose(fp1);
                fclose(fp2);
            }
            else 
            {
                printf("Could not open files for writing\n");
                return 1;
            }
        } 
        else 
        {
          // use pipe here

          struct MinMax min_max0 = GetMinMax(array, i*arrPart, (i+1)*arrPart);
          int max_len = 30;
          char buff[max_len];
          char strMin[max_len/2];
          char strMax[max_len/2];
          sprintf(strMin, "%d", min_max0.min);
          sprintf(strMax, "%d", min_max0.max);
          strcpy(buff, strMin);
          strcat(buff, " ");
          strcat(buff, strMax);
          close(fd[i][0]);
          write(fd[i][1], buff, max_len);
          close(fd[i][1]);
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  pid_t wpid;
  int status = 0;
  while ((wpid = wait(&status)) > 0);
  /*while (active_child_processes > 0) {
    active_child_processes -= 1;
  }*/
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      FILE * fp1;
      FILE * fp2;
      fp1 = fopen ("min.txt", "r");
      fp2 = fopen ("max.txt",  "r");
      if (fp1 != NULL && fp2 != NULL)
      {
          fscanf(fp1, "%d", &min);
          fscanf(fp2, "%d", &max);
          fclose(fp1);
          fclose(fp2);
      }
      else
      {
          printf("Could not open files for reading\n");
          return 1;
      }

    } else {
        char buff[30];
            if ( read( fd[i][0], buff, sizeof(buff)) <= 0) //read from pipe
                {
                    perror( "read failed" );
                    return 1;
                }
                char * token = strtok(buff, " ");
                min = atoi(token);
                token = strtok(NULL, " ");
                max = atoi(token);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(fd);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
