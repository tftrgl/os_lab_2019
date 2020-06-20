#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  char ip[20];
  int port = 0;
  int buffsize = 1024;

    while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"ip", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {"buffsize", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        memcpy(ip, optarg, strlen(optarg));
        break;
      case 1:
        port = atoi(optarg);
        if (port < 1024 || port > 49151)
        {
            printf("Invalid port\n");
            return 1;
        }
        break;
      case 2:
        buffsize = atoi(optarg);
        if (buffsize < 1)
        {
            printf("Buffsize must be positive\n");
            return 1;
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (argc != 4) {
    printf("usage: client <IPaddress of server>\n");
    exit(1);
  }

  char sendline[buffsize], recvline[buffsize + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;


  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, buffsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, buffsize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
